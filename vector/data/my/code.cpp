#include "vector.hpp"

#include <iostream>

#include <vector>

int main()
{
	sjtu::vector<long long> v;
	for (long long i = 0; i < 1LL << 3; ++i) {
		v.push_back(i);
	}
	for (long long i = 0; i < 1LL << 3; ++i) {
		std::cout << v.back() << '\n';
		v.pop_back();
	}
	return 0;
}