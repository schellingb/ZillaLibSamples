#include <ZL_Display3D.h>

enum { SCENE_GAME = 1 };
static ZL_Mesh mshPlanet, mshSun, mshSky;
static ZL_Camera Camera;
static ZL_Light Light;
static ZL_RenderList RenderList;
static float CameraDistance = 3.0f;

struct sSceneGame : public ZL_Scene
{
	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	virtual void InitGlobal()
	{
		//Load planet model data
		mshPlanet = ZL_Mesh::FromPLY("Data/lowpolyplanet.ply.zip");

		//Create a material for the planet which uses the vertex colors defined in the model data and renders it with the shadow mapping with a lot of shininess
		using namespace ZL_MaterialModes;
		mshPlanet.SetMaterial(ZL_Material(MM_VERTEXCOLOR | MM_LIT | MM_SPECULARSTATIC | MM_SHADOWMAP).SetUniformFloat(Z3U_SPECULAR, 1.0f).SetUniformFloat(Z3U_SHININESS, 16.0f));

		//Make a sphere mesh for the sun and set its material as unlit glowing orange
		mshSun = ZL_Mesh::BuildSphere(1, 23).SetMaterial(0, ZL_Material(MM_STATICCOLOR).SetUniformVec4(Z3U_COLOR, ZL_Color::Orange));

		//Make an inverted sphere for the background sky map with a custom shaded material that draws noise as stars
		mshSky = ZL_Mesh::BuildSphere(20, 20, true).SetMaterial(0, ZL_Material(MM_DIFFUSEFUNC | MR_TEXCOORD,
			ZL_GLSL_IMPORTSNOISE()
			"vec4 CalcDiffuse()"
			"{"
				"float s = clamp((snoise(" Z3V_TEXCOORD "*250.0)-.95)*15.,0.,1.);"
				"return vec4(s,s,s,1);"
			"}"
		));

		//set up the light position direction and color
		Light.SetPosition(ZL_Vector3(0, 0, 15.0f)).SetDirection(ZL_Vector3(0, 0, -1.0f)).SetColor(ZLRGB(1,1,.9));
	}

	virtual void Draw()
	{
		if (ZL_Input::Down(ZLK_ESCAPE)) ZL_Application::Quit();

		//Update the camera position every frame referencing the mouse coordinates and use the mouse wheel to zoom
		if (ZL_Input::MouseWheel()) CameraDistance = ZL_Math::Clamp(CameraDistance * (ZL_Input::MouseWheel() > 0 ? .8f : 1.25f), 2.f, 20.f);
		Camera.SetPosition(ZL_Vector3::FromRotation((ZL_Display::PointerX-ZLHALFW)/ZLHALFW*PI, -(ZL_Display::PointerY-ZLHALFH)/ZLHALFH*PIHALF*0.99f) * CameraDistance);
		Camera.SetDirection(-Camera.GetPosition().VecNorm());

		//When clicking with the left mouse button, apply the current camera location/direction to the light
		if (ZL_Input::Down()) Light.SetPosition(Camera.GetPosition()).SetDirection(Camera.GetDirection());

		//Setup and draw our dynamic render list with our three meshes
		RenderList.Reset();
		RenderList.Add(mshSky, ZL_Matrix()); //always untransformed at the center
		RenderList.Add(mshPlanet, ZL_Matrix::MakeRotateY(ZLSECONDS*.3f)); //at the center with a rotation based on time
		RenderList.Add(mshSun, ZL_Matrix::MakeTranslate(Light.GetPosition())); //draw the sun at the lights position
		ZL_Display3D::DrawListWithShadowMapping(RenderList, Camera, Light); //draw the list with shadow mapping
	}
} SceneGame;

struct sMain : public ZL_Application
{
	sMain() : ZL_Application() {}

	void Load(int argc, char *argv[])
	{
		//Initialize the game with depth buffer, 3d rendering and shadow mapping
		ZL_Display::Init("Basic 3D", 1280, 720, ZL_DISPLAY_DEPTHBUFFER);
		ZL_Display3D::Init();
		ZL_Display3D::InitShadowMapping();
		ZL_Input::Init();
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
