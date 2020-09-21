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

	vector3 apex = vector3(0.0f, a_fHeight / 2.0f, 0.0f);
	float degIncrements = 360.0f / a_nSubdivisions;
	float deg = 90; //Deg 0 starts at the top of the circle, so add 90, which is the real value of the top of the circle. Reminder: SFML is in radians. Convert this always, John.
	vector3 prevPoint = vector3(cos(deg * PI / 180) * a_fRadius, -a_fHeight / 2, sin(deg * PI / 180) * a_fRadius);
	vector3 nextPoint;
	for (int a = 0; a < a_nSubdivisions; a++) //The top of the circle is degree 0.
	{
		deg += degIncrements; //Increment degrees around the 'circle'
		nextPoint = vector3(cos(deg * PI / 180) * a_fRadius, -a_fHeight / 2, sin(deg * PI / 180) * a_fRadius); //Calculate next point on the triangle.
		AddTri(prevPoint, apex, nextPoint); //Draw tri to the top
		AddTri(prevPoint, nextPoint, -apex); //Draw tri to the bottom. Why is apex in different positions? I don't fully know. But it's how the cone isn't inside out, so that's what we're doing.
		prevPoint = nextPoint; //Set the next point as the previous point.
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

	vector3 top = vector3(0.0f, a_fHeight / 2.0f, 0.0f);
	float degIncrements = 360.0f / a_nSubdivisions;
	float deg = 90; //Deg 0 starts at the top of the circle, so add 90, which is the real value of the top of the circle. Reminder: SFML is in radians. Convert this always, John.
	vector3 prevPoint = vector3(cos(deg * PI / 180) * a_fRadius, 0, sin(deg * PI / 180) * a_fRadius);
	vector3 nextPoint;
	for (int a = 0; a < a_nSubdivisions; a++) //The top of the circle is degree 0.
	{
		deg += degIncrements; //Increment degrees around the 'circle'
		nextPoint = vector3(cos(deg * PI / 180) * a_fRadius, 0, sin(deg * PI / 180) * a_fRadius); //Calculate next point on the cylinder.
		AddTri(vector3(prevPoint.x, a_fHeight / 2.0f, prevPoint.z), top, vector3(nextPoint.x, a_fHeight / 2.0f, nextPoint.z)); //Draw tri to the top
		AddTri(vector3(prevPoint.x, -a_fHeight / 2.0f, prevPoint.z), vector3(nextPoint.x, -a_fHeight / 2.0f, nextPoint.z), -top); //Draw tri to the bottom. Why is top in different positions? I don't fully know. But it's how the cone isn't inside out, so that's what we're doing.
		AddQuad(vector3(nextPoint.x, -a_fHeight / 2.0f, nextPoint.z), vector3(prevPoint.x, -a_fHeight / 2.0f, prevPoint.z), vector3(nextPoint.x, a_fHeight / 2.0f, nextPoint.z), vector3(prevPoint.x, a_fHeight / 2.0f, prevPoint.z)); //Draw square connecting top and bottom circles.
		prevPoint = nextPoint; //Set the next point as the previous point.
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

	// Replace this with your code
	//GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------
	vector3 top = vector3(0.0f, a_fHeight / 2.0f, 0.0f);
	float degIncrements = 360.0f / a_nSubdivisions;
	float deg = 90; //Deg 0 starts at the top of the circle, so add 90, which is the real value of the top of the circle. Reminder: SFML is in radians. Convert this always, John.
	//Variable names are self explanatory.
	vector3 prevPointInnerLower = vector3(cos(deg * PI / 180) * a_fInnerRadius, -a_fHeight / 2, sin(deg * PI / 180) * a_fInnerRadius);
	vector3 prevPointInnerUpper = vector3(cos(deg * PI / 180) * a_fInnerRadius, a_fHeight / 2, sin(deg * PI / 180) * a_fInnerRadius);
	vector3 prevPointOuterLower = vector3(cos(deg * PI / 180) * a_fOuterRadius, -a_fHeight / 2, sin(deg * PI / 180) * a_fOuterRadius);
	vector3 prevPointOuterUpper = vector3(cos(deg * PI / 180) * a_fOuterRadius, a_fHeight / 2, sin(deg * PI / 180) * a_fOuterRadius);
	vector3 nextPointInnerUpper, nextPointInnerLower, nextPointOuterUpper, nextPointOuterLower;
	for (int a = 0; a < a_nSubdivisions; a++) //The top of the circle is degree 0.
	{
		deg += degIncrements; //Increment degrees around the 'circle'
		nextPointInnerUpper = vector3(cos(deg * PI / 180) * a_fInnerRadius, a_fHeight / 2, sin(deg * PI / 180) * a_fInnerRadius); //Calculate next point on the inside top of the cylinder.
		nextPointInnerLower = vector3(cos(deg * PI / 180) * a_fInnerRadius, -a_fHeight / 2, sin(deg * PI / 180) * a_fInnerRadius); //Calculate next point on the inside bottom of the cylinder.
		nextPointOuterUpper = vector3(cos(deg * PI / 180) * a_fOuterRadius, a_fHeight / 2, sin(deg * PI / 180) * a_fOuterRadius); //Calculate next point on the outside top of the cylinder.
		nextPointOuterLower = vector3(cos(deg * PI / 180) * a_fOuterRadius, -a_fHeight / 2, sin(deg * PI / 180) * a_fOuterRadius); //Calculate next point on the outside bottom of the cylinder.
		AddQuad(nextPointOuterLower, prevPointOuterLower, nextPointOuterUpper, prevPointOuterUpper); //Outside of cylinder
		AddQuad(nextPointInnerUpper, prevPointInnerUpper, nextPointInnerLower, prevPointInnerLower); //Inside of cylinder. More random switching to not be inside out.
		AddQuad(nextPointOuterUpper, prevPointOuterUpper, nextPointInnerUpper, prevPointInnerUpper); //Top of cylinder
		AddQuad(nextPointInnerLower, prevPointInnerLower, nextPointOuterLower, prevPointOuterLower); //Bottom of cylinder. Random switching works. I'd love to know why.
		//Set next points as previous points.
		prevPointInnerUpper = nextPointInnerUpper;
		prevPointInnerLower = nextPointInnerLower;
		prevPointOuterUpper = nextPointOuterUpper;
		prevPointOuterLower = nextPointOuterLower;
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	//SubdivisionsA: number of segments around torus.
	//SubdivisionsB: number of segments of the 'tube' of the torus.
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

	// Replace this with your code
	//GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------
	/*
	How to implement:
	Find 'center' (the middle of the ring where squares connect) of a torus circle.
	Find the 'center' of the next torus circle.
	Create an array of all points that will be around each circle FROM THE CENTER.
	Rotate the circle.
	Connect each point in a loop.
	*/
	float middleRadius = (a_fOuterRadius - a_fInnerRadius) / 2;
	float toMiddleRadius = a_fInnerRadius + middleRadius;
	float degIncrements = 360.0f / a_nSubdivisionsA;
	float deg = 0;
	vector3 prevCenter = vector3(toMiddleRadius + cosf(glm::radians(deg)) * middleRadius, 0.0f, sinf(glm::radians(deg)) * middleRadius);
	vector3 nextCenter;
	std::vector<vector3> prevRing(a_nSubdivisionsB);
	std::vector<vector3> nextRing(a_nSubdivisionsB);
	for (int a = 0; a < a_nSubdivisionsA; a++)
	{
		deg += degIncrements; //Increment degrees around the 'circle'
		nextCenter = vector3(toMiddleRadius + cosf(glm::radians(deg)) * middleRadius, 0.0f, sinf(glm::radians(deg)) * middleRadius);
		float ringDeg = 0; //Degree counter for the ring.
		float ringDegIncrements = 360.0f / a_nSubdivisionsB;

		for (int b = 0; b < a_nSubdivisionsB; b++)
		{
			prevRing[b] = (vector3(cos(glm::radians(ringDeg)) * prevCenter.x, sin(glm::radians(ringDeg)) * prevCenter.x, prevCenter.z)); //If anything goes wrong, it's probably these.
			nextRing[b] = (vector3(cos(glm::radians(ringDeg)) * nextCenter.x, sin(glm::radians(ringDeg)) * nextCenter.x, nextCenter.z));
			prevRing[b] = glm::rotate(matrix4(1), glm::radians(deg - degIncrements), vector3(0, 0, 1)) * vector4(prevRing[b], 1);
			nextRing[b] = glm::rotate(matrix4(1), glm::radians(deg), vector3(0, 0, 1)) * vector4(nextRing[b], 1);
			ringDeg += ringDegIncrements;
		}
		for (int c = 0; c < a_nSubdivisionsB - 1; c++)
		{
			AddQuad(prevRing[c], prevRing[c + 1], nextRing[c], nextRing[c + 1]);
		}
		AddQuad(prevRing[a_nSubdivisionsB - 1], prevRing[0], nextRing[a_nSubdivisionsB - 1], nextRing[0]);
		prevCenter = nextCenter;
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
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	float ringDegs = 0.0f; //Degrees going around the sphere horizontally. Max 360.
	float arcDegs = 0.0f; //Degrees going up the sphere vertically. Max 180.
	float ringInc = 360.0f / a_nSubdivisions; //Degrees for subdivision increments. May need to add 2 in order to factor out the subdivisions that are lost from the base/top.
	float arcInc = ringInc / 2;
	vector3 bottom = vector3(0.0f, 0.0f, -a_fRadius); //Self explanatory. Top and bottom of sphere, for drawing tri's.
	vector3 top = vector3(0.0f, 0.0f, a_fRadius);
	std::vector<vector3> currentRingPoints(a_nSubdivisions); //Vectors of the points of the current and previous points.
	std::vector<vector3> prevRingPoints(a_nSubdivisions);

	for (int a = 0; a < a_nSubdivisions - 1; a++) //Main drawing loop. Draws 1 ring per loop.
	{
		arcDegs += arcInc; //Increment arc degrees.
		ringDegs = 0.0f; //Reset ring degrees.
		for (int b = 0; b < a_nSubdivisions; b++) //Calculates ring locations.
		{
			currentRingPoints[b] = vector3(a_fRadius * cosf(glm::radians(ringDegs)) * sinf(glm::radians(arcDegs)), a_fRadius * sinf(glm::radians(ringDegs)) * sinf(glm::radians(arcDegs)), a_fRadius * cosf(glm::radians(arcDegs))); //Generate next point.
			ringDegs += ringInc;
		}
		if (a == 0) //Bottom case
		{
			for (int c = 0; c < a_nSubdivisions; c++)
			{
				AddTri(currentRingPoints[c], currentRingPoints[(c + 1) % a_nSubdivisions], top);
			}
		}
		else//Middle cases.
		{
			for (int e = 0; e < a_nSubdivisions; e++)
			{
				AddQuad(prevRingPoints[(e + 1) % a_nSubdivisions], prevRingPoints[e], currentRingPoints[(e + 1) % a_nSubdivisions], currentRingPoints[e]);
			}
		}

		if (a == a_nSubdivisions - 2) //Top case
		{
			for (int d = 0; d < a_nSubdivisions; d++)
			{
				AddTri(currentRingPoints[d], bottom, currentRingPoints[(d + 1) % a_nSubdivisions]);
			}
		}
		
		prevRingPoints = currentRingPoints; //Copy contents of old arry into new.
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}