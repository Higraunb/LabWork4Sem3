#include <iostream>
#include <TLists.h>
int main()
{
	TLists<int> a(5);
	a.push_back(10);
	a.push_front(20);
	a.push_back(30);
	cout << a;
	return 0;
}