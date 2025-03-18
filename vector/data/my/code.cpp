#include "../../src/vector.hpp"

#include <iostream>

#include <vector>

void solve(int *p) {
	sjtu::vector<int *> v;
	v.push_back(p);
	std::cout << *v[0] << '\n';
}

int main()
{
	int *p = new int(5);
	solve(p);
	std::cout << *p << '\n';
	delete p;
	return 0;
}