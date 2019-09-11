#include <rock/memory_allocator/NewJemallocNodumpAllocator.h>


namespace rocksdb {


Status NewJemallocNodumpAllocator(
    JemallocAllocatorOptions& options,
    std::shared_ptr<MemoryAllocator>* memory_allocator) {
  *memory_allocator = nullptr;
  Status unsupported = Status::NotSupported(
      "JemallocNodumpAllocator only available with jemalloc version >= 5 "
      "and MADV_DONTDUMP is available.");
#ifndef ROCKSDB_JEMALLOC_NODUMP_ALLOCATOR
  (void)options;
  return unsupported;
#else
  if (!HasJemalloc()) {
    return unsupported;
  }
  if (memory_allocator == nullptr) {
    return Status::InvalidArgument("memory_allocator must be non-null.");
  }
  if (options.limit_tcache_size &&
      options.tcache_size_lower_bound >= options.tcache_size_upper_bound) {
    return Status::InvalidArgument(
        "tcache_size_lower_bound larger or equal to tcache_size_upper_bound.");
  }

  // Create arena.
  unsigned arena_index = 0;
  size_t arena_index_size = sizeof(arena_index);
  int ret =
      mallctl("arenas.create", &arena_index, &arena_index_size, nullptr, 0);
  if (ret != 0) {
    return Status::Incomplete("Failed to create jemalloc arena, error code: " +
                              ToString(ret));
  }
  assert(arena_index != 0);

  // Read existing hooks.
  std::string key = "arena." + ToString(arena_index) + ".extent_hooks";
  extent_hooks_t* hooks;
  size_t hooks_size = sizeof(hooks);
  ret = mallctl(key.c_str(), &hooks, &hooks_size, nullptr, 0);
  if (ret != 0) {
    JemallocNodumpAllocator::DestroyArena(arena_index);
    return Status::Incomplete("Failed to read existing hooks, error code: " +
                              ToString(ret));
  }

  // Store existing alloc.
  extent_alloc_t* original_alloc = hooks->alloc;
  extent_alloc_t* expected = nullptr;
  bool success =
      JemallocNodumpAllocator::original_alloc_.compare_exchange_strong(
          expected, original_alloc);
  if (!success && original_alloc != expected) {
    JemallocNodumpAllocator::DestroyArena(arena_index);
    return Status::Incomplete("Original alloc conflict.");
  }

  // Set the custom hook.
  std::unique_ptr<extent_hooks_t> new_hooks(new extent_hooks_t(*hooks));
  new_hooks->alloc = &JemallocNodumpAllocator::Alloc;
  extent_hooks_t* hooks_ptr = new_hooks.get();
  ret = mallctl(key.c_str(), nullptr, nullptr, &hooks_ptr, sizeof(hooks_ptr));
  if (ret != 0) {
    JemallocNodumpAllocator::DestroyArena(arena_index);
    return Status::Incomplete("Failed to set custom hook, error code: " +
                              ToString(ret));
  }

  // Create cache allocator.
  memory_allocator->reset(
      new JemallocNodumpAllocator(options, std::move(new_hooks), arena_index));
  return Status::OK();
#endif  // ROCKSDB_JEMALLOC_NODUMP_ALLOCATOR
}



}
