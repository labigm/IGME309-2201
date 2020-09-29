#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	std::vector<vector3> verticies;//Holds all the verticies
	float angle = 0;//Angle being changed
	float change = (2 * PI) / a_nSubdivisions;

	//Add the verticies to the vector
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		verticies.push_back(vector3(
			cos(angle) * a_fRadius, 
			sin(angle) * a_fRadius, 
			0));
		angle += change;//Increment angle
	}
	//Add the Tris
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(vector3(0.0f, 0.0f, a_fHeight/2), 
			verticies[i], 
			verticies[(i + 1) % a_nSubdivisions]);//Bottom face
		AddTri(verticies[i], 
			vector3(0.0f, 0.0f, -a_fHeight/2), 
			verticies[(i + 1) % a_nSubdivisions]);//To top point
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	std::vector<vector3> verticies;//Bottom
	std::vector<vector3> verticies2;//Top
	float angle = 0;//Angle being changed
	float change = (2 * PI) / a_nSubdivisions;

	//Add verticies to vectors
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		verticies.push_back(vector3(
			cos(angle) * a_fRadius, 
			sin(angle) * a_fRadius, 
			a_fHeight/2));//Bottom polygon
		verticies2.push_back(vector3(
			cos(angle) * a_fRadius, 
			sin(angle) * a_fRadius, 
			-a_fHeight/2));//Top polygon
		angle += change;
	}
	//Add Tris and Quads
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(vector3(0.0f, 0.0f, a_fHeight/2), 
			verticies[i], 
			verticies[(i + 1) % a_nSubdivisions]);//Top
		AddTri(verticies2[i], 
			vector3(0.0f, 0.0f, 
			-a_fHeight/2), 
			verticies2[(i + 1) % a_nSubdivisions]);//Bottom
		AddQuad(verticies2[(i + 1) % a_nSubdivisions], 
			verticies[(i + 1) % a_nSubdivisions], 
			verticies2[i], 
			verticies[i]);//Sides
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	std::vector<vector3> verticiesBottom;//Bottom outer
	std::vector<vector3> verticiesBottom2;//Bottom Inner
	std::vector<vector3> verticiesTop;//Top outer
	std::vector<vector3> verticiesTop2;//Top inner
	float angle = 0;//Angle being changed
	float change = (2 * PI) / a_nSubdivisions;

	//Add verticies to vectors
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		verticiesBottom.push_back(vector3(
			cos(angle) * a_fOuterRadius, 
			sin(angle) * a_fOuterRadius, 
			a_fHeight/2));//Bottom outer polygon 
		verticiesBottom2.push_back(vector3(
			cos(angle) * a_fInnerRadius, 
			sin(angle) * a_fInnerRadius, 
			a_fHeight/2));//Bottom inner polygon
		verticiesTop.push_back(vector3(
			cos(angle) * a_fOuterRadius, 
			sin(angle) * a_fOuterRadius, 
			-a_fHeight/2));//Top outer polygon
		verticiesTop2.push_back(vector3(
			cos(angle) * a_fInnerRadius, 
			sin(angle) * a_fInnerRadius, 
			-a_fHeight/2));//Top inner polygon
		angle += change;
	}
	//Add Quads
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(verticiesBottom2[i], 
			verticiesBottom[i], 
			verticiesBottom2[(i + 1) % a_nSubdivisions], 
			verticiesBottom[(i + 1) % a_nSubdivisions]);//Bottom Faces
		AddQuad(verticiesTop2[(i + 1) % a_nSubdivisions], 
			verticiesTop[(i + 1) % a_nSubdivisions], 
			verticiesTop2[i], 
			verticiesTop[i]);//Top Faces

		AddQuad(verticiesBottom[i], 
			verticiesTop[i], 
			verticiesBottom[(i + 1) % a_nSubdivisions], 
			verticiesTop[(i + 1) % a_nSubdivisions]);//Outer Sides
		AddQuad(verticiesBottom2[(i + 1) % a_nSubdivisions], 
			verticiesTop2[(i + 1) % a_nSubdivisions], 
			verticiesBottom2[i], 
			verticiesTop2[i]);//Inner Sides
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();
	float radiusDifference = a_fOuterRadius - a_fInnerRadius;//Difference between the two radii
	float middleRadius = (.5f * radiusDifference) + a_fInnerRadius;//Radius of the inside of the torus from the origin
	float centralRadius = .5f * radiusDifference;//Radius of the inside of the torus cross section
	float angle = 0;//Angle being changed
	float changeA = (2 * PI) / a_nSubdivisionsA;//Vertical change
	float changeB = (2 * PI) / a_nSubdivisionsB;//Horizontal change

	std::vector<vector3> verticiesHorizontal;//Middle of torus
	std::vector<std::vector<vector3>> verticiesVertical;//Vertical rings around torus

	//Horizontal ring
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		verticiesHorizontal.push_back(vector3(
			cos(angle) * middleRadius, 
			sin(angle) * middleRadius, 
			0.0f));//Horizontal Polygon
		angle += changeB;
	}

	angle = 0;//Reset angle

	//Adds a new vector to the vector of vectors
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		std::vector<vector3> temp;//Holds a vector to be inserted
		for (int j = 0; j < a_nSubdivisionsA; j++)
		{
			//Find the change around each cross section
			temp.push_back(vector3(
				(centralRadius * sin((PI / 2.0f) - angle) * cos((float)i * changeB)) + verticiesHorizontal[i].x,
				(centralRadius * sin((PI / 2.0f) - angle) * sin((float)i * changeB)) + verticiesHorizontal[i].y,
				(centralRadius * cos((PI / 2.0f) - angle)) + verticiesHorizontal[i].z
			));

			angle += changeA;
		}
		verticiesVertical.push_back(temp);
		angle = 0;
	}

	//Add quads
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		for (int j = 0; j < a_nSubdivisionsA; j++)
		{
			AddQuad(
				verticiesVertical[(i + 1) % a_nSubdivisionsB][j],
				verticiesVertical[(i + 1) % a_nSubdivisionsB][(j + 1) % a_nSubdivisionsA],
				verticiesVertical[i][j],
				verticiesVertical[i][(j + 1) % a_nSubdivisionsA]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}

	//I changed the values here, as low numbers look weird with my formula
	if (a_nSubdivisions < 5)
	{
		a_nSubdivisions = 5;
	}
	if (a_nSubdivisions > 50)
		a_nSubdivisions = 50;

	Release();
	Init();

	float changeLong = (2.0f * PI) / (float) (a_nSubdivisions+1);//Change along the longitude
	//One is added to long to balance out
	float changeLat = PI / (float) a_nSubdivisions;//Change along the latitute

	std::vector<std::vector<vector3>> verticies;//Holds the verticies

	//Add a vector to the vector of vectors
	for (int i = 0; i < a_nSubdivisions+1; i++)
	{
		std::vector<vector3> temp;//Added to verticies
		for (int j = 0; j < a_nSubdivisions+1; j++)
		{
			//Uses a parametric formula of a sphere to find each point
			temp.push_back(vector3(
				cos(changeLong * j) * sin(changeLat * i) * a_fRadius,
				cos(changeLat * i) * a_fRadius,
				sin(changeLong * j) * sin(changeLat * i) * a_fRadius
			));
		}
		verticies.push_back(temp);
	}

	//Add quads
	for (int i = 0; i < a_nSubdivisions+1; i++)
	{
		for (int j = 0; j < a_nSubdivisions+1; j++)
		{
			//For the top and bottom, this technically draws a tri
			//As the the point is repeated
			AddQuad(
				verticies[i][j],
				verticies[i][(j + 1) % (a_nSubdivisions+1)],
				verticies[(i + 1) % (a_nSubdivisions+1)][j],
				verticies[(i + 1) % (a_nSubdivisions+1)][(j + 1) % (a_nSubdivisions+1)]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}