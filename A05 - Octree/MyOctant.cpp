#include "MyOctant.h"
using namespace Simplex;

//Static fields
int MyOctant::octantCount = 0;
int MyOctant::maxLevel = 0;
int MyOctant::idealCount = 5;
//Init
void MyOctant::Init(void) 
{
	numChildren = 0;
	size = 0.0f;
	id = octantCount;
	level = 0;

	center = vector3(0.0f);
	max = vector3(0.0f);
	min = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	root = nullptr;
	parent = nullptr;
	for (int i = 0; i < 8; i++)
	{
		children[i] = nullptr;
	}
}
void MyOctant::Swap(MyOctant& other)
{
	std::swap(numChildren, other.numChildren);
	std::swap(size, other.size);
	std::swap(id, other.id);
	std::swap(level, other.level);

	std::swap(center, other.center);
	std::swap(max, other.max);
	std::swap(min, other.min);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::swap(root, other.root);
	std::swap(parent, other.parent);
	std::swap(listChildren, other.listChildren);
	for (int i = 0; i < 8; i++)
	{
		std::swap(children[i],other.children[i]);
	}
}

void MyOctant::Release(void)
{
	//Check if root
	if (level == 0)
	{
		DeleteBranches();
	}
	numChildren = 0;
	size = 0.0f;
	entityList.clear();
	listChildren.clear();
}

//Big 3
MyOctant::MyOctant(int maximumLevel, int ideal)
{
	//Initialize
	Init();

	octantCount = 0;
	maxLevel = maximumLevel;
	idealCount = ideal;
	id = octantCount;

	root = this;
	listChildren.clear();

	//Holds a list of the max and the min vectors from BO
	std::vector<vector3> extremes;

	//Iterate through adding the max and the min for all objects
	int numObjects = m_pEntityMngr->GetEntityCount();
	for (int i = 0; i < numObjects; i++)
	{
		MyEntity* entity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* rigidBody = entity->GetRigidBody();
		extremes.push_back(rigidBody->GetMinGlobal());
		extremes.push_back(rigidBody->GetMaxGlobal());
	}

	//Assign a new rigid body with a list of all the extremes
	//MyRigidBody will find the max and the min among this
	//And use them for its box
	MyRigidBody* rigidBody = new MyRigidBody(extremes);

	//Find the max of the components
	vector3 halfWidth = rigidBody->GetHalfWidth();
	float maxWidth = halfWidth.x;
	for (int i = 0; i < 3; i++)
	{
		if (maxWidth < halfWidth[i])
			maxWidth = halfWidth[i];
	}
	//Find the center of the rigid body
	vector3 newCenter = rigidBody->GetCenterLocal();
	//Clear and delete
	extremes.clear();
	SafeDelete(rigidBody);

	//Set size, and the vectors
	size = maxWidth * 2.0f;
	center = newCenter;
	max = center + (vector3(maxWidth));
	min = center - (vector3(maxWidth));

	octantCount++;
	//Construct the tree
	ConstructTree(maxLevel);
}
//Given a size and a center, construct a new octant
MyOctant::MyOctant(vector3 centerOther, float sizeOther)
{
	//Initialize
	Init();
	center = centerOther;
	size = sizeOther;
	max = center + (vector3(size)/2.0f);
	min = center - (vector3(size) / 2.0f);
	octantCount++;
}
MyOctant::MyOctant(MyOctant const& other)
{
	numChildren = other.numChildren;
	size = other.size;
	id = other.id;
	level = other.level;

	center = other.center;
	max = other.max;
	min = other.min;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	root = other.root;
	parent = other.parent;
	for (int i = 0; i < 8; i++)
	{
		children[i] = other.children[i];
	}
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant()
{ 
	Release(); 
}

//Properties
float MyOctant::GetSize(void)
{
	return size;
}
vector3 MyOctant::GetCenterGlobal(void)
{
	return center;
}
vector3 MyOctant::GetMaxGlobal(void)
{
	return max;
}
vector3 MyOctant::GetMinGlobal(void)
{
	return min;
}
int MyOctant::GetOctantCount(void) 
{ 
	return octantCount; 
}
MyOctant* MyOctant::GetParent(void)
{
	return parent;
}
MyOctant* MyOctant::GetChild(int index)
{
	if (index > 7)
		return nullptr;
	return children[index];
}

//Methods
//Helper Methods
bool MyOctant::Contains(int index)
{
	//Number of all entitites
	int count = m_pEntityMngr->GetEntityCount();

	//If index is greater than the number of entities
	if (index >= count)
		return false;

	//Only need a simple test

	//Entity
	MyEntity* entity = m_pEntityMngr->GetEntity(index);
	//Rigid body
	MyRigidBody* rb = entity->GetRigidBody();
	//Max
	vector3 maxGlobal = rb->GetMaxGlobal();
	//Min
	vector3 minGlobal = rb->GetMinGlobal();

	//X
	if (max.x < minGlobal.x)
		return false;
	if (min.x > maxGlobal.x)
		return false;
	//Y
	if (max.y < minGlobal.y)
		return false;
	if (min.y > maxGlobal.y)
		return false;
	//Z
	if (max.z < minGlobal.z)
		return false;
	if (min.z > maxGlobal.z)
		return false;

	//Otherwise return true
	return true;
}
bool MyOctant::ContainsMoreThan(int entities)
{
	//Count of how mant entities are in the octant
	int count = 0;
	//Total object count
	int objectCount = m_pEntityMngr->GetEntityCount();
	for (int i = 0; i < objectCount; i++)
	{
		//If contained, increment count
		if (Contains(i))
			count++;
		//If count is greater than passed value, return
		//Prevents needing to check all entities
		if (count > entities)
			return true;
	}
	return false;
}

//Display Methods
void MyOctant::Display(int index, vector3 color)
{
	//This version of display is passed an index
	//If the index matches this id, then draw the cube
	if (id == index)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) *
			glm::scale(vector3(size)), color, RENDER_WIRE);
		//break if drawing cube
		return;
	}
	//Otherwise, check all the children against the index
	for (int i = 0; i < numChildren; i++)
	{
		children[i]->Display(index);
	}
}
void MyOctant::Display(vector3 color)
{
	//This version of display only uses a color
	//Recursive, calls display on all of its children, then draws for itself
	for (int i = 0; i < numChildren; i++)
	{
		children[i]->Display(color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) *
		glm::scale(vector3(size)), color, RENDER_WIRE);
}

//Subdivide
void MyOctant::Subdivide(void)
{
	//If at max, do nothing, as no more levels allowed
	if (level == maxLevel)
		return;

	//If there are already children, do nothing
	//Don't want to subdivide more than once
	if (numChildren != 0)
		return;

	//Set numChildren to 8
	//Only two options for num children are 0 or 8
	numChildren = 8;

	//Quarter size works to move between the new centers of all of the children
	//Quarter is the perfect amount to iterate in each direction
	float quarterSize = size / 4.0f;
	//The new size of the subdivided cube is half of the previous one
	float halfSize = size/ 2.0f;
	//Temp for the center
	vector3 centerTemp = center;

	//Move to bottom left back corner
	centerTemp.x -= quarterSize;
	centerTemp.y -= quarterSize;
	centerTemp.z -= quarterSize;
	//Set up different octants
	//Bottom Left Back
	children[0] = new MyOctant(centerTemp, halfSize);

	//Bottom Right Back
	centerTemp.x += halfSize;
	children[1] = new MyOctant(centerTemp, halfSize);

	//Bottom Right Front
	centerTemp.z += halfSize;
	children[2] = new MyOctant(centerTemp, halfSize);

	//Bottom Left Front
	centerTemp.x -= halfSize;
	children[3] = new MyOctant(centerTemp, halfSize);

	//Top Left Front
	centerTemp.y += halfSize;
	children[4] = new MyOctant(centerTemp, halfSize);

	//Top Left Back
	centerTemp.z -= halfSize;
	children[5] = new MyOctant(centerTemp, halfSize);

	//Top Right Back
	centerTemp.x += halfSize;
	children[6] = new MyOctant(centerTemp, halfSize);

	//Top Front Back
	centerTemp.z += halfSize;
	children[7] = new MyOctant(centerTemp, halfSize);

	//Setup parts of children
	for (int i = 0; i < 8; i++)
	{
		//Assign root, parent, and next level.
		children[i]->root = root;
		children[i]->parent = this;
		children[i]->level = level + 1;

		//Check if the child contains more than the ideal count
		//Subdividing here works, as it still caps at the max level
		//This prevents unneccesary subdivisions, like in corners
		if (children[i]->ContainsMoreThan(idealCount))
		{
			children[i]->Subdivide();
		}
	}
}

//Construct Methods
void MyOctant::ConstructTree(int maximumLevel)
{
	//if not the root
	if (level != 0)
		return;

	maxLevel = maximumLevel;
	octantCount = 1;

	//Clear entity list
	entityList.clear();

	//Remove brances
	DeleteBranches();
	listChildren.clear();

	//Initial way subdivide is called
	if (ContainsMoreThan(idealCount))
	{
		Subdivide();
	}

	//Assign ID
	AssignIDtoEntity();

	//Construct List
	ConstructList();
}
void MyOctant::AssignIDtoEntity(void)
{
	//Recursion
	for (int i = 0; i < numChildren; i++)
	{
		children[i]->AssignIDtoEntity();
	}

	//If a leaf, it should not have an ID yet
	if (numChildren == 0)
	{
		//Number of entities
		int numEntites = m_pEntityMngr->GetEntityCount();
		for (int i = 0; i < numEntites; i++)
		{
			//If contained
			if (Contains(i))
			{
				//Add to list
				entityList.push_back(i);
				//Add a dimension to it, using the id as the dimension
				m_pEntityMngr->AddDimension(i, id);
			}
		}
	}
}
void MyOctant::ConstructList(void)
{
	//Create a list for all the children
	for (int i = 0; i < numChildren; i++)
	{
		children[i]->ConstructList();
	}

	//If not the root, add this list to the children
	if (entityList.size() > 0)
	{
		root->listChildren.push_back(this);
	}
}

//Delete Methods
void MyOctant::DeleteBranches(void)
{
	//Use Recursion to iterate through all branches
	for(int i = 0; i < numChildren; i++)
	{
		children[i]->DeleteBranches();
		delete children[i];
		children[i] = nullptr;
	}
	numChildren = 0;
}
void MyOctant::ClearEntityList(void)
{
	for (int i = 0; i < numChildren; i++)
	{
		children[i]->ClearEntityList();
	}
	entityList.clear();
}
