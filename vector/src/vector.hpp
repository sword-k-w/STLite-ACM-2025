#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
public:
  /**
    * a type for actions of the elements of a vector, and you should write
    *   a class named const_iterator with same interfaces.
    */
  /**
    * you can see RandomAccessIterator at CppReference for help.
    */
  class const_iterator;
  class iterator {
  // The following code is written for the C++ type_traits library.
  // Type traits is a C++ feature for describing certain properties of a type.
  // For instance, for an iterator, iterator::value_type is the type that the
  // iterator points to.
  // STL algorithms and containers may use these type_traits (e.g. the following
  // typedef) to work properly. In particular, without the following code,
  // @code{std::sort(iter, iter1);} would not compile.
  // See these websites for more information:
  // https://en.cppreference.com/w/cpp/header/type_traits
  // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
  // About iterator_category: https://en.cppreference.com/w/cpp/iterator
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::output_iterator_tag;

  private:
    T *begin_, *ptr_;
  public:
    iterator() = delete;
    iterator(T *begin, T *ptr) : begin_(begin), ptr_(ptr) {}
    iterator(const iterator &rhs) : begin_(rhs.begin_), ptr_(rhs.ptr_) {}
    /**
      * return a new iterator which pointer n-next elements
      * as well as operator-
      */
    iterator operator + (const int &n) const {
      return iterator(begin_, ptr_ + n);
    }
    iterator operator - (const int &n) const {
      return iterator(begin_, ptr_ - n);
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw invaild_iterator.
    int operator - (const iterator &rhs) const {
      if (begin_ != rhs.begin_) {
        throw invalid_iterator();
      }
      return ptr_ - rhs.ptr_;
    }

    iterator& operator += (const int &n) {
      ptr_ += n;
      return *this;
    }
    iterator& operator -= (const int &n) {
      ptr_ -= n;
      return *this;
    }

    iterator operator ++ (int) {
      auto tmp = *this;
      ++ptr_;
      return tmp;
    }
    iterator& operator ++ () {
      ++ptr_;
      return *this;
    }
    iterator operator -- (int) {
      auto tmp = *this;
      --ptr_;
      return tmp;
    }
    iterator& operator -- () {
      --ptr_;
      return *this;
    }

    T& operator * () const{
      return *ptr_;
    }
    /**
      * a operator to check whether two iterators are same (pointing to the same memory address).
      */
    bool operator == (const iterator &rhs) const {
      return ptr_ == rhs.ptr_;
    }
    bool operator == (const const_iterator &rhs) const {
      return ptr_ == rhs.ptr_;
    }
    /**
      * some other operator for iterator.
      */
    bool operator != (const iterator &rhs) const {
      return ptr_ != rhs.ptr_;
    }
    bool operator != (const const_iterator &rhs) const {
      return ptr_ != rhs.ptr_;
    }
  };

  class const_iterator
  {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::output_iterator_tag;

  private:
    const T *begin_, *ptr_;
  public:
    const_iterator() = delete;
    const_iterator(T *begin, T *ptr) : begin_(begin), ptr_(ptr) {}
    const_iterator(const const_iterator &rhs) : begin_(rhs.begin_), ptr_(rhs.ptr_) {}

    const_iterator operator + (const int &n) const {
      return const_iterator(begin_, ptr_ + n);
    }
    const_iterator operator - (const int &n) const {
      return const_iterator(begin_, ptr_ - n);
    }

    int operator - (const const_iterator &rhs) const {
      if (begin_ != rhs.begin_) {
        throw invalid_iterator();
      }
      return ptr_ - rhs.ptr_;
    }

    const_iterator& operator += (const int &n) {
      ptr_ += n;
      return *this;
    }
    const_iterator& operator -= (const int &n) {
      ptr_ -= n;
      return *this;
    }

    const_iterator operator ++ (int) {
      auto tmp = *this;
      ++ptr_;
      return tmp;
    }
    const_iterator& operator ++ () {
      ++ptr_;
      return *this;
    }
    const_iterator operator -- (int) {
      auto tmp = *this;
      --ptr_;
      return tmp;
    }
    const_iterator& operator -- () {
      --ptr_;
      return *this;
    }

    const T operator * () const {
      return *ptr_;
    }
    
    bool operator == (const iterator &rhs) const {
      return ptr_ == rhs.ptr_;
    }
    bool operator == (const const_iterator &rhs) const {
      return ptr_ == rhs.ptr_;
    }
    bool operator != (const iterator &rhs) const {
      return ptr_ != rhs.ptr_;
    }
    bool operator != (const const_iterator &rhs) const {
      return ptr_ != rhs.ptr_;
    }
  };
  vector() {
    size_ = 0;
    capacity_ = 3;
    array_ = static_cast<T *>(operator new [] (capacity_ * sizeof(T)));
  }
  vector(const vector &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    array_ = static_cast<T *>(operator new [] (capacity_ * sizeof(T)));
    for (size_t i = 0; i < size_; ++i) {
      new(&array_[i]) T(other.array_[i]);
    }
  }
  ~vector() {
    for (size_t i = 0; i < size_; ++i) {
      array_[i].~T();
    }
    operator delete [] (array_);
  }
  
  vector &operator = (const vector &other) {
    if (this == &other) {
      return *this;
    }
    for (size_t i = 0; i < size_; ++i) {
      array_[i].~T();
    }
    operator delete [] (array_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    array_ = static_cast<T *>(operator new [] (capacity_ * sizeof(T)));;
    for (size_t i = 0; i < size_; ++i) {
      new(&array_[i]) T(other.array_[i]);
    }
    return *this;
  }
  /**
    * assigns specified element with bounds checking
    * throw index_out_of_bound if pos is not in [0, size)
    */
  T &at(const size_t &pos) {
    if (pos >= size_) {
      throw index_out_of_bound();
    }
    return array_[pos];
  }
  const T &at(const size_t &pos) const {
    if (pos >= size_) {
      throw index_out_of_bound();
    }
    return array_[pos];
  }
  /**
    * assigns specified element with bounds checking
    * throw index_out_of_bound if pos is not in [0, size)
    * !!! Pay attentions
    *   In STL this operator does not check the boundary but I want you to do.
    */
  T &operator [] (const size_t &pos) {
    if (pos >= size_) {
      throw index_out_of_bound();
    }
    return array_[pos];
  }
  const T &operator [] (const size_t &pos) const {
    if (pos >= size_) {
      throw index_out_of_bound();
    }
    return array_[pos];
  }
  /**
    * access the first element.
    * throw container_is_empty if size == 0
    */
  const T &front() const {
    if (size_ == 0) {
      throw container_is_empty();
    }
    return array_[0];
  }
  /**
    * access the last element.
    * throw container_is_empty if size == 0
    */
  const T &back() const {
    if (size_ == 0) {
      throw container_is_empty();
    }
    return array_[size_ - 1];
  }
  /**
    * returns an iterator to the beginning.
    */
  iterator begin() {
    return iterator(array_, array_);
  }
  const_iterator begin() const {
    return const_iterator(array_, array_);
  }
  const_iterator cbegin() const {
    return const_iterator(array_, array_);
  }
  /**
    * returns an iterator to the end.
    */
  iterator end() {
    return iterator(array_, array_ + size_);
  }
  const_iterator end() const {
    return const_iterator(array_, array_ + size_);
  }
  const_iterator cend() const {
    return const_iterator(array_, array_ + size_);
  }
  /**
    * checks whether the container is empty
    */
  bool empty() const {
    return size_ == 0;
  }
  /**
    * returns the number of elements
    */
  size_t size() const {
    return size_;
  }
  /**
    * clears the contents
    */
  void clear() {
    capacity_ = 3;
    for (size_t i = 0; i < size_; ++i) {
      array_[i].~T();
    }
    size_ = 0;
    operator delete [] (array_);
    array_ = static_cast<T *>(operator new [] (capacity_ * sizeof(T)));
  }
  /**
    * inserts value before pos
    * returns an iterator pointing to the inserted value.
    */
  iterator insert(iterator pos, const T &value) {
    return insert(pos - begin(), value);
  }
  /**
    * inserts value at index ind.
    * after inserting, this->at(ind) == value
    * returns an iterator pointing to the inserted value.
    * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
    */
  iterator insert(const size_t &ind, const T &value) {
    if (ind > size_) {
      throw index_out_of_bound();
    }
    for (size_t i = size_; i > ind; --i) {
      new(&array_[i]) T(array_[i - 1]);
      array_[i - 1].~T();
    }
    new(&array_[ind]) T(value);
    ++size_;
    if (size_ == capacity_) {
      DoubleCapacity();
    } 
    return iterator(array_, array_ + ind);
  }
  /**
    * removes the element at pos.
    * return an iterator pointing to the following element.
    * If the iterator pos refers the last element, the end() iterator is returned.
    */
  iterator erase(iterator pos) {
    return erase(pos - begin());
  }
  /**
    * removes the element with index ind.
    * return an iterator pointing to the following element.
    * throw index_out_of_bound if ind >= size
    */
  iterator erase(const size_t &ind) {
    if (ind >= size_) {
      throw index_out_of_bound();
    }
    if (size_ == 1) {
      clear();
      return end();
    }
    for (size_t i = ind; i < size_; ++i) {
      array_[i].~T();
      new(&array_[i]) T(array_[i + 1]);
    }
    array_[size_--].~T();
    if (size_ * 3 <= capacity_) {
      ShrinkCapacity();
    }
    return iterator(array_, array_ + ind);
  }
  /**
    * adds an element to the end.
    */
  void push_back(const T &value) {
    new(&array_[size_++]) T(value);
    if (size_ == capacity_) {
      DoubleCapacity();
    }
  }
  /**
    * remove the last element from the end.
    * throw container_is_empty if size() == 0
    */
  void pop_back() {
    if (size_ == 1) {
      return clear();
    }
    --size_;
    array_[size_].~T();
    if (size_ * 3 <= capacity_) {
      ShrinkCapacity();
    }
  }

private:
  size_t size_, capacity_;
  T *array_;
  void Adjust(size_t new_capacity) {
    capacity_ = new_capacity;
    T *new_array = static_cast<T *>(operator new [] (capacity_ * sizeof(T)));;
    for (size_t i = 0; i < size_; ++i) {
      new(&new_array[i]) T(array_[i]);
      array_[i].~T();
    }
    operator delete [] (array_);
    array_ = new_array;
  }
  void DoubleCapacity() {
    Adjust(size_ * 2);
  }
  void ShrinkCapacity() {
    Adjust((capacity_ + 2) / 3 * 2);
  }
};

}

#endif