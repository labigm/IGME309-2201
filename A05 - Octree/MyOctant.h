#pragma once
#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{
		static int octantCount;//Number of octants currently made
		static int maxLevel;//max level octant can go to
		static int idealCount;//ideal number of entities

		int id = 0;//Id of octant
		int level = 0;//current level of octant
		int numChildren = 0;//number of children
		float size = 0.0f; //size of octant

		MeshManager* m_pMeshMngr = nullptr;//Mesh manager
		MyEntityManager* m_pEntityMngr = nullptr;//Entity manager

		vector3 center = vector3(0.0f);//Center point of octant
		vector3 max = vector3(0.0f);//Max vector of octant
		vector3 min = vector3(0.0f);//Min vector of octant

		MyOctant* parent = nullptr;//Parent of octant
		MyOctant* children[8];//Children of octant

		std::vector<int> entityList;//List of entity indicies in octant
		MyOctant* root = nullptr;//Root
		std::vector<MyOctant*> listChildren;//Used for root

	public:
		//Base constructor
		MyOctant(int maxLevel = 3, int idealCount = 5);
		//Normal constructor
		MyOctant(vector3 center, float size);
		//Copy Constructor
		MyOctant(MyOctant const& other);
		//Copy Assign Constructor
		MyOctant& operator=(MyOctant const& other);
		//Destructor
		~MyOctant(void);
		//Swap
		void Swap(MyOctant& other);
		//Get size
		float GetSize(void);
		//Get Center Global
		vector3 GetCenterGlobal(void);
		//Get Max Global
		vector3 GetMaxGlobal(void);
		//Get Min Global
		vector3 GetMinGlobal(void);
		//Get Child
		MyOctant* GetChild(int index);
		//Get Parent
		MyOctant* GetParent(void);
		//Gets total octants
		int GetOctantCount(void);
		//If contains object 
		bool Contains(int index);
		//Display
		void Display(int index, vector3 color = C_YELLOW);
		void Display(vector3 color = C_YELLOW);
		//Clear entity list
		void ClearEntityList(void);
		//Subdivide
		void Subdivide(void);
		//Contains more than(checks if it has more than that many objects)
		bool ContainsMoreThan(int entities);
		//Delete Branches
		void DeleteBranches(void);
		//Construct Tree
		void ConstructTree(int max = 3);
		//Assign an ID to the Object
		void AssignIDtoEntity(void);
	private:
		//Deallocates fields
		void Release(void);
		//Allocates fields
		void Init(void);
		//Creates a list of leafs containing objects
		void ConstructList(void);
	};
}