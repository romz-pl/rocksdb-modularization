#pragma once

#include <atomic>

namespace rocksdb {


struct Node {
  // Accessors/mutators for links.  Wrapped in methods so we can
  // add the appropriate barriers as necessary.
  Node* Next() {
    // Use an 'acquire load' so that we observe a fully initialized
    // version of the returned Node.
    return next_.load(std::memory_order_acquire);
  }
  void SetNext(Node* x) {
    // Use a 'release store' so that anybody who reads through this
    // pointer observes a fully initialized version of the inserted node.
    next_.store(x, std::memory_order_release);
  }
  // No-barrier variants that can be safely used in a few locations.
  Node* NoBarrier_Next() {
    return next_.load(std::memory_order_relaxed);
  }

  void NoBarrier_SetNext(Node* x) { next_.store(x, std::memory_order_relaxed); }

  // Needed for placement new below which is fine
  Node() {}

 private:
  std::atomic<Node*> next_;

  // Prohibit copying due to the below
  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;

 public:
  char key[1];
};


}
