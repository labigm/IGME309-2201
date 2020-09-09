#include "Alberto.h"

//  AlbertoClass
void AlbertoClass::Init(void)
{
	m_nAge = 36;
}
void AlbertoClass::Swap(AlbertoClass& other)
{
	std::swap(m_nAge, other.m_nAge);
}
void AlbertoClass::Release(void)
{
}
//The big 3
AlbertoClass::AlbertoClass(){Init();}
AlbertoClass::AlbertoClass(int a_nAge) { m_nAge = a_nAge; }
AlbertoClass::AlbertoClass(AlbertoClass const& other)
{
	m_nAge = other.m_nAge;
}
AlbertoClass& AlbertoClass::operator=(AlbertoClass const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		AlbertoClass temp(other);
		Swap(temp);
	}
	return *this;
}
AlbertoClass::~AlbertoClass(){Release();};

bool AlbertoClass::operator>(AlbertoClass const& other)
{
	return this->m_nAge > other.m_nAge;
}

bool AlbertoClass::operator<(AlbertoClass const& other)
{
	return this->m_nAge < other.m_nAge;
}

//Accessors
void AlbertoClass::SetAge(int a_nAge){ m_nAge = a_nAge; }
int AlbertoClass::GetAge(void){ return m_nAge; }
//--- Non Standard Singleton Methods

