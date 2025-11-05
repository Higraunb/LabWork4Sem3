#include <iostream>
#include "TLists.h"
using namespace std;

int main()
{
	TLists<int> list;
	list.PushBack(1);
	list.PushBack(2);
	list.PushBack(3);
	list.LamdaFunc([](int& x) { x *= x;});
	cout << "List elements after PushBack and PushFront:" << endl;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		cout << *it << " ";
	}
	return 0;
}