static ZL_Camera Camera;
static ZL_ParticleEmitter Particle;
static ZL_RenderList RenderList;

struct sMain : public ZL_Application
{
	sMain() : ZL_Application() {}

	void Load(int argc, char *argv[])
	{
		//Initialize the game with depth buffer and 3d rendering
		ZL_Display::Init("3D Particles", 1280, 720, ZL_DISPLAY_DEPTHBUFFER);
		ZL_Display3D::Init();
		ZL_Input::Init();

		//Setup the particle effect with random initial velocity, animation sheet, fixed color and size over lifetime
		Particle = ZL_ParticleEmitter(1.f);
		Particle.SetSpawnVelocityRanges(ZLV3(-1,-1,1.5), ZLV3(1,1,2.5));
		Particle.SetAnimationSheet(ZL_Surface("Data/Fire.png"), 4, 4);
		Particle.SetColor(ZLLUM(.3));
		Particle.SetLifetimeSize(.35f, 1.9f);
	}

	void AfterFrame()
	{
		if (ZL_Input::Down(ZLK_ESCAPE)) ZL_Application::Quit();

		//Update the camera position every frame referencing the mouse coordinates and use the mouse wheel to zoom
		static float CameraDistance = 3.0f;
		if (ZL_Input::MouseWheel()) CameraDistance = ZL_Math::Clamp(CameraDistance * (ZL_Input::MouseWheel() > 0 ? .8f : 1.25f), 2.f, 20.f);
		float HoirzontalAngleRad = (ZL_Display::PointerX-ZLHALFW)/ZLHALFW*PI;
		float VerticalAngleRad = -((ZL_Display::PointerY-ZLHALFH)/ZLHALFH-.6f)*PIHALF*0.5f;
		Camera.SetLookAt(ZL_Vector3::FromRotation(HoirzontalAngleRad, VerticalAngleRad) * CameraDistance, ZL_Vector3::Zero);

		//Spawn a single particle every frame at a fixed position and then step the entire particle system
		Particle.Spawn(ZL_Vector3(0, 0, -1.f));
		Particle.Update(Camera); //pass camera to have the particle polygons 'look' at the camera

		//Setup and draw our dynamic render list with our particle effect on a black background
		RenderList.Reset();
		RenderList.Add(Particle, ZL_Matrix::Identity);
		ZL_Display::ClearFill(ZL_Color::Black);
		ZL_Display3D::DrawList(RenderList, Camera);
	}
} Main;
