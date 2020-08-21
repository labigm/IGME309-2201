#pragma once
#include <iostream>

using namespace std;

template <class T>
class Queue
{
private:
	T* queue;//Array
	int size;//Current number of objects in queue
	int capacity;//Maximum number queue can hold
public:
	Queue()
	{
		queue = new T[1];
		size = 0;
		capacity = 1;
	}
	//Copy Constructor
	Queue(const Queue&other)
	{
		size = other.size;
		capacity = other.capacity;
		queue = new T[size];
		for (int i = 0; i < size; i++)
		{
			queue[i] = other.queue[i];
		}
	}
	//Copy Assignment Operator
	Queue& operator=(const Queue& other)
	{
		if (this == &other)
		{
			return *this;
		}

		size = other.size;
		capacity = other.capacity;

		queue = new T[size];
		for (int i = 0; i < size; i++)
		{
			queue[i] = other.queue[i];
		}

		return *this;
	}
	~Queue()
	{
		delete[] queue;
		queue = nullptr;
		size = 0;
		capacity = 0;
	}

	//Add data to queue
	void Push(T data)
	{
		//If size equals capacity, make new queue 
		if (size == capacity)
		{
			//Double new queue size and capacity
			T* newQueue = new T[size * 2];
			capacity = size * 2;
			//Fill in new queue
			for (int i = 0; i < size; i++)
			{
				newQueue[i] = queue[i];
			}
			//Copy queue
			queue = new T[capacity];
			for (int i = 0; i < size; i++)
			{
				queue[i] = newQueue[i];
			}
			//Point new queue to nullptr
			delete[] newQueue;
			//Assign data and increment size
			queue[size] = data;
			size++;
		}
		else
		{
			//Assign data and increment size
			queue[size] = data;
			size++;
		}
		Sort();
	}
	void Sort()
	{
		//Sort
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if (queue[i] > queue[j])
				{
					T temp = queue[i];
					queue[i] = queue[j];
					queue[j] = temp;
				}
			}
		}
	}

	//Remove first element
	T Pop()
	{
		//If elements in the array
		if (size > 0)
		{
			T data = queue[0];
			for (int i = 0; i < size; i++)
			{
				queue[i] = queue[i + 1];
			}
			size--;
			return data;
		}
		else
		{
			return NULL;
		}
	}

	//Prints all elements of queue
	void Print()
	{
		for (int i = 0; i < size; i++)
		{
			cout << "Element " << i << ": " << queue[i] << endl;
		}
	}

	//Returns size of stack
	int GetSize()
	{
		return size;
	}

	//Returns true if empty
	bool IsEmpty()
	{
		if (size == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};