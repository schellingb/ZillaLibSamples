static ZL_Camera Camera;
static ZL_Light Light;
static ZL_Mesh mshWall, mshGround;
static ZL_RenderList RenderList;

struct sMain : public ZL_Application
{
	sMain() : ZL_Application() {}

	void Load(int argc, char *argv[])
	{
		//Initialize the game with depth buffer, 3d rendering and shadow mapping
		ZL_Display::Init("Advanced 3D Materials", 1280, 720, ZL_DISPLAY_DEPTHBUFFER);
		ZL_Display3D::Init();
		ZL_Display3D::InitShadowMapping();
		ZL_Input::Init();

		using namespace ZL_MaterialModes;

		//Create a material for the wall with parallax and normal mapping
		ZL_Material matWall = ZL_Material(MM_DIFFUSEMAP | MM_SPECULARSTATIC | MM_NORMALMAP | MM_PARALLAXMAP | MO_PRECISIONTANGENT)
			.SetDiffuseTexture(ZL_Surface("Data/BRICKS.png").SetTextureRepeatMode())
			.SetNormalTexture(ZL_Surface("Data/BRICKS_N.png").SetTextureRepeatMode())
			.SetParallaxTexture(ZL_Surface("Data/BRICKS_D.png").SetTextureRepeatMode())
			.SetUniformFloat(Z3U_SPECULAR, 1.f)
			.SetUniformFloat(Z3U_PARALLAXSCALE, .1f);

		//Create a material with a custom diffuse color function that renders grass with a fake depth
		ZL_Material matGround(MM_DIFFUSEFUNC | MR_TEXCOORD | MR_NORMAL | MR_CAMERATANGENT | MR_TIME | MO_PRECISIONTANGENT, 
			ZL_GLSL_IMPORTSNOISE()

			"float Grass(vec2 p, float z)"
			"{"
				"p *= 150.;"
				"return (max(snoise(p) - z, 0.) + max(snoise(p+1234.) - z, 0.) + max(snoise(p+5678.) - z, 0.) + max(snoise(p+9999.) - z, 0.)) / (1. - z) / 4.;"
			"}"

			"vec4 CalcDiffuse()"
			"{"
				"float dist = length(" Z3U_VIEWPOS " - " Z3V_POSITION ");"
				"vec2 view_offset = (" Z3S_CAMERATANGENT ".xy / " Z3S_CAMERATANGENT ".z) * 0.025;"

				"float n = 0.;"
				"for (float f = 0.; f < 1.005; f+=0.02)"
					"n = n * .8 + Grass(" Z3V_TEXCOORD " - f * view_offset, f);"
				"n = min(n * 7., 1.);"
				"if (dist > 30.0) n = n / (dist / 30.0);"

				"return vec4(vec3(n * (0.25 + snoise(" Z3V_TEXCOORD " * 100.) * .5), .2 + n * .75 ,0.), 1.);"
			"}"
		);

		//Create a box mesh for the wall and a plane for the ground with the materials set up above
		mshWall = ZL_Mesh::BuildBox(ZLV3(2, .5, 3), matWall, ZLV3(0,0,3), ZLV(10, 10));
		mshGround = ZL_Mesh::BuildPlane(ZLV(10, 10), matGround, ZL_Vector3::Up, ZL_Vector3::Zero, ZLV(2, 2));

		//set up the light position, direction and cover area size
		Light.SetLookAt(ZL_Vector3(0, 15.0f, 10.f), ZL_Vector3::Zero).SetDirectionalLight(10);
	}

	void AfterFrame()
	{
		if (ZL_Input::Down(ZLK_ESCAPE)) ZL_Application::Quit();

		//Update the camera position every frame referencing the mouse coordinates and use the mouse wheel to zoom
		static float CameraDistance = 5.f;
		if (ZL_Input::MouseWheel()) CameraDistance = ZL_Math::Clamp(CameraDistance * (ZL_Input::MouseWheel() > 0 ? .8f : 1.25f), 1.6384f, 19.073486328125f);
		float HoirzontalAngleRad = (ZL_Display::PointerX-ZLHALFW)/ZLHALFW*PI+PIHALF;
		float VerticalAngleRad = -((ZL_Display::PointerY-ZLHALFH)/ZLHALFH-.6f)*PIHALF*0.5f;
		Camera.SetLookAt(ZLV3(0,0,3) + ZL_Vector3::FromRotation(HoirzontalAngleRad, VerticalAngleRad) * CameraDistance, ZLV3(0,0,3));

		//Setup and draw our dynamic render list with our three meshes
		RenderList.Reset();
		RenderList.Add(mshGround, ZL_Matrix::Identity);
		RenderList.Add(mshWall, ZL_Matrix::MakeRotateZ(ZLTICKS*.0005f));
		ZL_Display::ClearFill(ZL_Color::DarkBlue);
		ZL_Display3D::DrawListWithLight(RenderList, Camera, Light);
	}
} Main;
