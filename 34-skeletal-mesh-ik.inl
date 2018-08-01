static ZL_Camera Camera;
static ZL_Light Light = ZL_Light(ZL_Vector3(2.f, 5.f, 8.f));
static ZL_SkeletalMesh SkeletalMesh;
static ZL_RenderList RenderList;

struct sMain : public ZL_Application
{
	sMain() : ZL_Application() {}

	void Load(int argc, char *argv[])
	{
		//Initialize the game with depth buffer and 3d rendering
		ZL_Display::Init("Skeletal Mesh", 1280, 720, ZL_DISPLAY_DEPTHBUFFER);
		ZL_Display3D::Init();
		ZL_Input::Init();

		//Load the skeletal mesh model file
		SkeletalMesh = ZL_SkeletalMesh::FromGLTF("Data/human.glb.zip");
	}

	void AfterFrame()
	{
		if (ZL_Input::Down(ZLK_ESCAPE)) ZL_Application::Quit();

		//Update the camera position every frame referencing the mouse coordinates
		float HoirzontalAngleRad = (ZL_Display::PointerX-ZLHALFW)/ZLHALFW*PI+PIHALF;
		float VerticalAngleRad = ((ZL_Display::PointerY-ZLHALFH)/ZLHALFH-.2f)*PIHALF*0.8f;
		Camera.SetLookAt(ZL_Vector3::FromRotation(HoirzontalAngleRad, VerticalAngleRad) * 3.f + ZL_Vector3(0,0,1), ZL_Vector3(0,0,1));

		//Update the foot positions with inverse kinematics (IK)
		SkeletalMesh.TwoBoneIK(12, ZL_Vector3(-.1f, 0, .23f - .15f * ssin(ZLSECONDS * 5)), ZL_Vector3::Forward); //Left foot
		SkeletalMesh.TwoBoneIK(15, ZL_Vector3( .1f, 0, .23f - .15f * scos(ZLSECONDS * 5)), ZL_Vector3::Forward); //Right foot
		SkeletalMesh.Update();

		//Setup and draw our dynamic render list with our skeletal mesh on a black background
		RenderList.Reset();
		RenderList.Add(SkeletalMesh, ZL_Matrix::Identity);
		ZL_Display::ClearFill(ZL_Color::Black);
		ZL_Display3D::DrawListWithLight(RenderList, Camera, Light);
	}
} Main;
