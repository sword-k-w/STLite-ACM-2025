// binomial heap
// Reference : https://www.cnblogs.com/luanxm/p/10847925.html

#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "utility.hpp"
#include "exceptions.hpp"
#include "../../vector/src/vector.hpp"

namespace sjtu {
/**
 * @brief a container like std::priority_queue which is a heap internal.
 * **Exception Safety**: The `Compare` operation might throw exceptions for certain data.
 * In such cases, any ongoing operation should be terminated, and the priority queue should be restored to its original state before the operation began.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
  /**
   * @brief default constructor
   */
  priority_queue() {
    size_ = 0;
    head_ = static_cast<node *>(operator new(sizeof(node)));
    head_->nxt_ = nullptr;
    max_ = nullptr;
    cmp_ = Compare();
  }

  priority_queue(const T &e) {
    size_ = 1;
    head_ = static_cast<node *>(operator new(sizeof(node)));
    head_->nxt_ = new node(e);
    max_ = head_->nxt_;
    cmp_ = Compare();
  }

  /**
   * @brief copy constructor
   * @param other the priority_queue to be copied
   */
  priority_queue(const priority_queue &other) {
    size_ = other.size_;
    head_ = static_cast<node *>(operator new(sizeof(node)));
    if (other.empty()) {
      head_->nxt_ = nullptr;
      max_ = nullptr;
      return;
    }
    head_->nxt_ = new node(other.head_->nxt_->val_);
    DfsCopy(head_->nxt_, other.head_->nxt_);
    node *tmp = other.head_;
    max_ = head_;
    while (tmp != other.max_) {
      tmp = tmp->nxt_;
      max_ = max_->nxt_;
    }
    cmp_ = Compare();
  }

  /**
   * @brief deconstructor
   */
  ~priority_queue() {
    if (head_->nxt_ != nullptr) {
      DfsDeconstruct(head_->nxt_);
    }
    operator delete(head_);
  }

  /**
   * @brief Assignment operator
   * @param other the priority_queue to be assigned from
   * @return a reference to this priority_queue after assignment
   */
  priority_queue &operator=(const priority_queue &other) {
    if (this == &other) {
      return *this;
    }
    if (head_->nxt_ != nullptr) {
      DfsDeconstruct(head_->nxt_);
    }
    size_ = other.size_;
    if (other.empty()) {
      head_->nxt_ = nullptr;
      max_ = nullptr;
      return *this;
    }
    head_->nxt_ = new node(other.head_->nxt_->val_);
    DfsCopy(head_->nxt_, other.head_->nxt_);
    node *tmp = other.head_;
    max_ = head_;
    while (tmp != other.max_) {
      tmp = tmp->nxt_;
      max_ = max_->nxt_;
    }
    return *this;
  }

  /**
   * @brief get the top element of the priority queue.
   * @return a reference of the top element.
   * @throws container_is_empty if empty() returns true
   */
  const T & top() const {
    if (empty()) {
      throw container_is_empty();
    }
    return max_->val_;
  }

  /**
   * @brief push new element to the priority queue.
   * @param e the element to be pushed
   */
  void push(const T &e) {
    if (empty()) {
      size_ = 1;
      head_->nxt_ = new node(e);
      max_ = head_->nxt_;
      return;
    }
    priority_queue tmp(e);
    vector<node *> root_val;
    if (CheckMergeException(tmp, root_val)) {
      throw sjtu::runtime_error();
    }
    MergeWithCheck(tmp, root_val, false);
  }

  /**
   * @brief delete the top element from the priority queue.
   * @throws container_is_empty if empty() returns true
   */
  void pop() {
    if (empty()) {
      throw container_is_empty();
    }
    if (size() == 1) { // Remove the only node
      delete max_;
      size_ = 0;
      head_->nxt_ = nullptr;
      max_ = nullptr;
      return;
    }
    node *las = head_;
    while (las->nxt_ != max_) {
      las = las->nxt_;
    }
    las->nxt_ = max_->nxt_;
    size_ ^= (1 << max_->size_);
    if (max_->size_ == 0) { // Remove a tree with a single node
      if (empty()) {
        delete max_;
        max_ = nullptr;
        head_->nxt_ = nullptr;
        return;
      }
      node *rec = max_;
      if (FindMax()) {
        max_ = rec;
        las->nxt_ = max_;
        size_ ^= (1 << max_->size_);
        throw sjtu::runtime_error();
      }
      delete rec;
    } else if (head_->nxt_ == nullptr) { // Remove the root of the only tree
      node *rec = max_;
      head_->nxt_ = Next(max_->son_);
      max_->son_->nxt_ = nullptr;
      if (FindMax()) {
        rec->son_ = Next(head_->nxt_);
        head_->nxt_ = max_ = rec;
        size_ ^= (1 << max_->size_);
        throw sjtu::runtime_error();
      }
      size_ = (1 << rec->size_) - 1;
      delete rec;
    } else {
      // clock_t begin = clock();
      priority_queue tmp(max_->son_, (1 << max_->size_) - 1);
      // clock_t end = clock();
      // tot += end - begin;
      vector<node *> root_val;
      if (CheckMergeException(tmp, root_val)) {
        las->nxt_ = max_;
        size_ ^= (1 << max_->size_);
        max_->son_ = Next(tmp.head_->nxt_);
        tmp.head_->nxt_->nxt_ = nullptr;
        tmp.head_->nxt_ = nullptr;
        throw sjtu::runtime_error();
      }
      delete max_;
      MergeWithCheck(tmp, root_val, false);
    }
  }

  /**
   * @brief return the number of elements in the priority queue.
   * @return the number of elements.
   */
  size_t size() const {
    return size_;
  }

  /**
   * @brief check if the container is empty.
   * @return true if it is empty, false otherwise.
   */
  bool empty() const {
    return size_ == 0;
  }

  /**
   * @brief merge another priority_queue into this one.
   * The other priority_queue will be cleared after merging.
   * The complexity is at most O(logn).
   * @param other the priority_queue to be merged.
   */
  void merge(priority_queue &other) {
    vector<node *> root_val;
    MergeWithCheck(other, root_val); 
  }
  
private:
  size_t size_;
  struct node {
    T val_;
    size_t size_ = 0; // actual size is 2 ^ size_
    node *nxt_ = nullptr, *son_ = nullptr;
    node() = delete;
    node(const T &val, node *son = nullptr) : val_(val), son_(son) {}
  };
  node *head_, *max_;
  Compare cmp_;

  node *Next(node *cur) {
    if (cur->nxt_ == nullptr) {
      return cur;
    }
    node *tmp = Next(cur->nxt_);
    cur->nxt_->nxt_ = cur;
    return tmp;
  }
  /** 
  * Construct based on already-constructed trees
  * The order is flipped
  * max_ is not calculated
  * Only used for pop()
  */
  priority_queue(node *cur, size_t size) : size_(size) { 
    head_ = static_cast<node *>(operator new(sizeof(node)));
    head_->nxt_ = Next(cur);
    cur->nxt_ = nullptr;
    cmp_ = Compare();
  }
  /**
  * Initially, *cur only has val.
  */
  void DfsCopy(node *cur, node *other) {
    cur->size_ = other->size_;
    if (other->nxt_ != nullptr) {
      cur->nxt_ = new node(other->nxt_->val_);
      DfsCopy(cur->nxt_, other->nxt_);
    }
    if (other->son_ != nullptr) {
      cur->son_ = new node(other->son_->val_);
      DfsCopy(cur->son_, other->son_);
    }
  }
  void DfsDeconstruct(node *cur) {
    if (cur->nxt_) {
      DfsDeconstruct(cur->nxt_);
    }
    if (cur->son_) {
      DfsDeconstruct(cur->son_);
    }
    delete cur;
  }
  bool FindMax() {
    try {
      max_ = head_->nxt_;
      node *cur = max_;
      while (cur != nullptr) {
        if (cmp_(max_->val_, cur->val_)) {
          max_ = cur;
        }
        cur = cur->nxt_;
      }
    } catch (...) {
      return true;
    }
    return false;
  }
  bool CheckMergeException(const priority_queue &other, vector<node *> &root_val) {
    node *cur = head_, *cur_prime = other.head_->nxt_;
    while (cur->nxt_ != nullptr && cur_prime != nullptr) {
      if (cur->nxt_->size_ > cur_prime->size_) { 
        root_val.push_back(cur_prime);
        cur_prime = cur_prime->nxt_;
      }
      else { 
        root_val.push_back(cur->nxt_); 
        cur = cur->nxt_;
      }
    }
    while (cur->nxt_ != nullptr) { 
      root_val.push_back(cur->nxt_); 
      cur = cur->nxt_;
    }
    while (cur_prime != nullptr) {
      root_val.push_back(cur_prime);
      cur_prime = cur_prime->nxt_;
    }
    size_t size = root_val.size();
    try {
      T max = root_val[0]->val_; 
      node tmp = *root_val[0];
      for (size_t i = 0; i + 1 < size; ++i) {
        if (tmp.size_ != root_val[i + 1]->size_ || (i + 2 < size && tmp.size_ == root_val[i + 2]->size_)) {
          if (cmp_(max, root_val[i]->val_)) {
            max = root_val[i]->val_;
          }
          tmp = *root_val[i + 1];
        } else {
          if (!cmp_(tmp.val_, root_val[i + 1]->val_)) {
            tmp = *root_val[i + 1];
          }
          ++tmp.size_;
        }
      }
      cmp_(max, tmp.val_);
    } catch (...) {
      return true;
    }
    return false;
  }
  
  void MergeWithCheck(priority_queue &other, vector<node *> &root_val, bool flag = true) {
    if (other.empty()) {
      return;
    }
    if (empty()) {
      size_ = other.size_;
      head_->nxt_ = other.head_->nxt_;
      max_ = other.max_;
      other.size_ = 0;
      other.head_->nxt_ = nullptr;
      return;
    }
    
    if (flag) { // Check exceptions
      if (CheckMergeException(other, root_val)) {
        throw sjtu::runtime_error();
      }
    }
    
    // Merge the root chain
    head_->nxt_ = root_val[0];
    size_t size = root_val.size();
    for (size_t i = 0; i + 1 < size; ++i) {
      root_val[i]->nxt_ = root_val[i + 1];
    }
    other.head_->nxt_ = nullptr;
    
    node *las = head_;
    node *cur = head_->nxt_;
    max_ = cur; 
    while (cur->nxt_ != nullptr) {
      // Two different trees or three trees with same size
      if (cur->size_ != cur->nxt_->size_ || (cur->nxt_->nxt_ != nullptr && cur->size_ == cur->nxt_->nxt_->size_)) {
        if (cmp_(max_->val_, cur->val_)) {
          max_ = cur;
        }
        las = las->nxt_;
        cur = cur->nxt_;
      } else { // Two trees with same size
        if (cmp_(cur->val_, cur->nxt_->val_)) { // cur becomes cur->nxt_'s first son
          las->nxt_ = cur->nxt_;
          node *tmp = cur->nxt_->son_;
          cur->nxt_->son_ = cur;
          cur->nxt_ = tmp;
          cur = las->nxt_;
          ++cur->size_;
        } else { // cur->nxt_ becomes cur's first son
          node *tmp = cur->nxt_->nxt_;
          cur->nxt_->nxt_ = cur->son_;
          cur->son_ = cur->nxt_;
          cur->nxt_ = tmp;
          ++cur->size_;
        }
      }
    }
    if (cmp_(max_->val_, cur->val_)) {
      max_ = cur;
    }
    size_ += other.size_;
    other.size_ = 0;
  }
};

}

#endif