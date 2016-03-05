#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	ZL_Surface srfLogo;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitGlobal()
	{
		srfLogo = ZL_Surface("Data/ZILLALIB.png").SetDrawOrigin(ZL_Origin::Center);
	}

	//Clear screen and draw the surface
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		scalar rotation = s(ZLTICKS)/s(1000);
		scalar scale = 1 + (s(0.2)*ssin(rotation*3));
		srfLogo.SetRotate(rotation);
		srfLogo.SetScale(scale );
		srfLogo.Draw(ZLHALFW, ZLHALFH);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Rotating and Scaling Surfaces", 854, 480);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
