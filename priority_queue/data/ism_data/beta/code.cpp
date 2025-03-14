// #include "priority_queue.hpp"
#include "../../src/priority_queue.hpp"

#include <iostream>
#include <vector>
#include <chrono>

template <class Func, class... Args>
void TestFuncTimeMicro(const std::string &namecode, Func func, Args... args) {
  auto beg = std::chrono::high_resolution_clock::now();
  func(std::forward<Args>(args)...);
  auto end = std::chrono::high_resolution_clock::now();
  auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count();
  std::cout << "Function \"" << namecode << "\" has taken " << dur << " us.\n";
}

template <class T>
using vec_t = std::vector<T>;
template <class T>
using pq_t = sjtu::priority_queue<T>;

struct stat_error: std::exception {};

// std::thread is an example.
class MoveType {
  static int amount;
  int unique_id_;
  bool stat_;
public:
  MoveType()
    : unique_id_(++amount), stat_(false) {}
  MoveType(const MoveType&) = delete;
  MoveType(MoveType &&other)
    : unique_id_(other.unique_id_), stat_(other.stat_) {}
  MoveType& operator=(const MoveType&) = delete;
  MoveType& operator=(MoveType &&other) {
    if(this == &other)
      return *this;
    unique_id_ = other.unique_id_;
    stat_ = other.stat_;
    return *this;
  }
  bool operator<(const MoveType &other) const {
    if(stat_ != other.stat_)
      throw stat_error();
    return stat_ ^ (unique_id_ < other.unique_id_);
  }
  int unique_id() const {
    return unique_id_;
  }
  MoveType& activate() {
    if(stat_)
      throw stat_error();
    stat_ = true;
    return *this;
  }
};
int MoveType::amount = 0;

void MovePushTest() {
  pq_t<MoveType> pq;

  for(int i = 0; i < 10; ++i)
    pq.push(MoveType());
  while(!pq.empty()) {
    std::cout << pq.top().unique_id() << ' ';
    pq.pop();
  }
  std::cout << '\n';

  int exception_cnt = 0;

  for(int i = 0; i < 10; ++i) {
    MoveType th;
    // Exception-safety protections should protect data structure from transient emergencies, not timed landmines.
    // Or to say, if a crash can happen between the new element and one existing element in the data structure,
    // I think the programmer should confirm that this crash happens between the new guy and any of the existing dudes.
    if(th.unique_id() % 2 == 0)
      th.activate();
    try {
      pq.push(std::move(th));
    } catch(stat_error&) {
      ++exception_cnt;
    }
  }
  if(exception_cnt != 5)
    std::cout << "Exception-safety not enough\n";
  else
    std::cout << "Exception-safety enough... for now.\n";
  while(!pq.empty()) {
    std::cout << pq.top().unique_id() << ' ';
    pq.pop();
  }
  std::cout << '\n';
}

void MoveAssignTest() {
  pq_t<int> pq1, pq2;
  for(int i = 0; i < 10000; ++i)
    pq1.push(i);
  for(int i = 0; i < 10; ++i) {
    pq2 = std::move(pq1);
    std::cout << pq2.top() << ' ';
    pq2.pop();
    pq1 = std::move(pq2);
    std::cout << pq1.top() << ' ';
    pq1.pop();
  }
  std::cout << '\n';
}

int main() {
  MovePushTest();
  TestFuncTimeMicro("MoveAssignTest", MoveAssignTest);
  return 0;
}