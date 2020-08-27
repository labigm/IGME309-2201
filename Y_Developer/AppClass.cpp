#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(vector3(0.0f, 3.0f, 13.0f), //Position
		vector3(0.0f, 3.0f, 12.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light(0 is reserved for global light)

	m_pEntityMngr->AddEntity("Macross//Glaug.fbx");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0, 2, 1)));
	m_pEntityMngr->UsePhysicsSolver();
	m_pEntityMngr->AddEntity("Portal//CompanionCube.fbx");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(1, 2, 3)));
	m_pEntityMngr->UsePhysicsSolver();
	m_pEntityMngr->AddEntity("Portal//Wheatley.fbx");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-1, 2, 2)));
	m_pEntityMngr->UsePhysicsSolver();
	m_pEntityMngr->AddEntity("Sorted//SphereCube.fbx");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(-2, 2, 5)));
	m_pEntityMngr->UsePhysicsSolver();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Update the Entity Manager
	m_pEntityMngr->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
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
	//release GUI
	ShutdownGUI();
}