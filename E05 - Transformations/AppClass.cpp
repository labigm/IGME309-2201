#include "AppClass.h"
void Application::InitVariables(void)
{
	m_numCubes = 46;
	for (uint i = 0; i < m_numCubes; ++i)
	{
		MyMesh* pMesh = new MyMesh();
		m_meshList.push_back(pMesh);
		m_meshList[i]->GenerateCube(1.0f, C_BLACK);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Projection
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//Draw the cubes
	int offset = 0;//Used to keep the i ligned up
	static float move = 0;//Moves the objects
	move += .1f;

	//Row 1 part 1
	for (int i = 0; i < 2; i++)
	{
		m_meshList[i]->Render(m4Projection, m4View, glm::translate(vector3(3.0f+i + move,0.0f,0.0f)));
		offset++;
	}

	//Row 1 part 2
	for (int i = 0; i < 2; i++)
	{
		m_meshList[i+offset]->Render(m4Projection, m4View, glm::translate(vector3(6.0f + i + move, 0.0f, 0.0f)));
		offset++;
	}

	//Row 2
	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(0.0f + move, 1.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(2.0f + move, 1.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(8.0f + move, 1.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(10.0f + move, 1.0f, 0.0f)));
	offset++;

	//Row 3
	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(0.0f + move, 2.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(10.0f + move, 2.0f, 0.0f)));
	offset++;

	for (int i = 0; i < 7; i++)
	{
		m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(2.0f+i + move, 2.0f, 0.0f)));
		offset++;
	}

	//Row 4
	for (int i = 0; i < 11; i++)
	{
		m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(0.0f + i + move, 3.0f, 0.0f)));
		offset++;
	}

	//Row 5
	for (int i = 0; i < 2; i++)
	{
		m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(1.0f + i + move, 4.0f, 0.0f)));
		offset++;
	}

	for (int i = 0; i < 3; i++)
	{
		m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(4.0f + i + move, 4.0f, 0.0f)));
		offset++;
	}

	for (int i = 0; i < 2; i++)
	{
		m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(8.0f + i + move, 4.0f, 0.0f)));
		offset++;
	}

	//Row 6
	for (int i = 0; i < 7; i++)
	{
		m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(2.0f + i + move, 5.0f, 0.0f)));
		offset++;
	}

	//Row 7 + Row 8
	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(3.0f + move, 6.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(7.0f + move, 6.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(2.0f + move, 7.0f, 0.0f)));
	offset++;

	m_meshList[offset]->Render(m4Projection, m4View, glm::translate(vector3(8.0f + move, 7.0f, 0.0f)));
	offset++;

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	std::vector<MyMesh*> m_meshList;
	int length = m_meshList.size();
	for (uint i = 0; i < length; ++i)
	{
		if (m_meshList[i] != nullptr)
		{
			delete m_meshList[i];
			m_meshList[i] = nullptr;
		}
	}
	//release GUI
	ShutdownGUI();
}