#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include "Queue.h"
#include "Alberto.h"

void wrapper()
{
	//Int Queue
	Queue <int> intQueue;
	//Push out of order, shows sorting
	intQueue.Push(0);
	intQueue.Push(5);
	intQueue.Push(1);
	intQueue.Push(3);

	//Print
	intQueue.Print();
	cout << "Size: " << intQueue.GetSize() << endl;
	cout << "Is Empty: " << intQueue.IsEmpty() << endl;

	//Pop
	intQueue.Pop();
	cout << "Popped Element" << endl;

	cout << endl;//Formatting

	//Print
	intQueue.Print();
	cout << "Size: " << intQueue.GetSize() << endl;

	//Push More
	intQueue.Push(1);
	intQueue.Push(3);

	cout << endl;//Formatting

	//Print
	intQueue.Print();
	cout << "Size: " << intQueue.GetSize() << endl;

	cout << endl;//Formatting

	//Char Queue
	Queue <char> charQueue;
	//Push out of order, shows sorting
	charQueue.Push('d');
	charQueue.Push('q');
	charQueue.Push('a');
	charQueue.Push('x');

	//Print
	charQueue.Print();
	cout << "Size: " << charQueue.GetSize() << endl;
	cout << "Is Empty: " << charQueue.IsEmpty() << endl;

	//Pop
	charQueue.Pop();
	cout << "Popped Element" << endl;
	charQueue.Pop();
	cout << "Popped Element" << endl;

	cout << endl;//Formatting

	//Print
	charQueue.Print();
	cout << "Size: " << charQueue.GetSize() << endl;

	cout << endl;//Formatting

	//Alberto Class
	Queue<AlbertoClass> albertoQueue;

	//Push
	albertoQueue.Push(AlbertoClass(7));
	albertoQueue.Push(AlbertoClass(1));
	albertoQueue.Push(AlbertoClass(5));

	//Print
	albertoQueue.Print();
	cout << "Size: " << albertoQueue.GetSize() << endl;

	cout << endl;//Formatting

	albertoQueue.Pop();
	cout << "Popped Element" << endl;

	cout << endl;//Formatting

	//Print
	albertoQueue.Print();
	cout << "Size: " << albertoQueue.GetSize() << endl;
}

int main()
{
	wrapper();
	_CrtDumpMemoryLeaks();
}

