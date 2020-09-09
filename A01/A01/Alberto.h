/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2020/08
----------------------------------------------*/
#ifndef __ALBERTOCLASS_H_
#define __ALBERTOCLASS_H_

#include <iostream>

//System Class
class AlbertoClass
{
	int m_nAge = 36; //Number of elements in the list of elements

public:
	/*
	Usage: Constructor
	Arguments: ---
	Output: class object instance
	*/
	AlbertoClass(void);
	/*
	Usage: Constructor with age indicator
	Arguments: Age of this Alberto
	Output: class object instance
	*/
	AlbertoClass(int a_nAge);
	/*
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	AlbertoClass(AlbertoClass const& other);
	/*
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	AlbertoClass& operator=(AlbertoClass const& other);
	/*
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~AlbertoClass(void);

	/*
	Usage: Changes object contents for other object's
	Arguments: other -> object to swap content from
	Output: ---
	*/
	void Swap(AlbertoClass& other);

	/*
	Usage: Larger
	Arguments: class object to verify
	Output: true if this object is larger than other
	*/
	bool operator>(AlbertoClass const& other);

	/*
	Usage: Smaller
	Arguments: class object to verify
	Output: true if this object is smaller than other
	*/
	bool operator<(AlbertoClass const& other);

	/*
	Usage: Gets data member
	Arguments: ---
	Output: data
	*/
	int GetAge(void);
	/*
	Usage: Sets data member
	Arguments: int a_nData = 1 -> data to set
	Output: ---
	*/
	void SetAge(int a_nAge);

	friend std::ostream& operator<<(std::ostream& os, AlbertoClass other)
	{
		os << other.m_nAge;
		return os;
	}

private:
	/*
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);
	/*
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);
};//class


#endif //__EXAMPLECLASS_H__

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/