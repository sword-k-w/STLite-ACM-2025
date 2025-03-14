// NO VALIDITY CHECK IS INVOLVED IN THIS CODE.

#include "priority_queue.hpp"
// #include "../../src/priority_queue.hpp"

#include <iostream>
#include <vector>
#include <chrono>

int rand() {
  static int val = 6147;
  val ^= val << 13;
  val ^= val >> 17;
  val ^= val << 5;
  return val;
}

int rand(int n) {
  int res = rand() % n;
  if(res < 0) res += n;
  return res;
}

template <class Func, class... Args>
void TestFuncTimeMilli(const std::string &namecode, Func func, Args... args) {
  auto beg = std::chrono::high_resolution_clock::now();
  func(std::forward<Args>(args)...);
  auto end = std::chrono::high_resolution_clock::now();
  auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count();
  std::cout << "Function \"" << namecode << "\" has taken " << dur << " ms.\n";
}

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

void TestInsertion(const vec_t<int> &data, pq_t<int> &pq) {
  for(const auto &val: data)
    pq.push(val);
}

// I reckon/hope no one will use binomial heap with O(logN) find-min
void TestFindMin(const pq_t<int> &pq) {
  volatile auto res = pq.top();
}

void TestDeletion(pq_t<int> &pq) {
  while(!pq.empty())
    pq.pop();
}

void TestUnion(pq_t<int> &pq1, pq_t<int> &pq2) {
  pq1.merge(pq2);
}

// std::thread is an example.
class MoveType {
  static int amount;
  int id_;
public:
  MoveType()
    : id_(++amount) {}
  MoveType(const MoveType&) = delete;
  MoveType(MoveType &&other)
    : id_(other.id_) {}
  MoveType& operator=(const MoveType&) = delete;
  MoveType& operator=(MoveType &&other) {
    if(this == &other)
      return *this;
    id_ = other.id_;
  }
  bool operator<(const MoveType &other) const {
    return id_ < other.id_;
  }
  int id() const {
    return id_;
  }
};
int MoveType::amount = 0;

void Tester(size_t Length) {
  vec_t<int> data1(Length);
  for(int i = 0; i < Length; ++i)
    data1[i] = rand(Length);
  vec_t<int> data2(Length);
  for(int i = 0; i < Length; ++i)
    data2[i] = rand(Length);
  pq_t<int> pq1, pq2;
  TestFuncTimeMilli("TestInsertion1", TestInsertion, std::ref(data1), std::ref(pq1));
  TestFuncTimeMilli("TestInsertion2", TestInsertion, std::ref(data2), std::ref(pq2));
  TestFuncTimeMicro("TestUnion", TestUnion, std::ref(pq1), std::ref(pq2));
  TestFuncTimeMicro("TestFindMin", TestFindMin, std::ref(pq1));
  TestFuncTimeMilli("TestDeletion", TestDeletion, std::ref(pq1));
  std::cout << "\n";
}

// void MoveTest() {
//   pq_t<MoveType> pq;
//   for(int i = 10; i > 0; --i)
//     pq.push(MoveType());
//   while(!pq.empty()) {
//     std::cout << pq.top().id() << ' ';
//     pq.pop();
//   }
//   std::cout << "\n\n";
// }

int main() {
  // MoveTest();
  std::cout << "Small test:\n";
  Tester(12345);
  std::cout << "Larger test:\n";
  Tester(12345678);
  return 0;
}