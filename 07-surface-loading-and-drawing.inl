#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	ZL_Surface srfLogo;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	//Load surface texture on entering the scene
	void InitEnter(ZL_SceneType SceneTypeFrom, void* data)
	{
		srfLogo = ZL_Surface("Data/ZILLALIB.png");
	}

	//Unload surface texture on eventual leaving of the scene
	void DeInitAfterTransition()
	{
		srfLogo = ZL_Surface();
	}

	//Clear screen and draw the surface
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		srfLogo.Draw(0, 0);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Surface Loading and Drawing", 854, 480);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
