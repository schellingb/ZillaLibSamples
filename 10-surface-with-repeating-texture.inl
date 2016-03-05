#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	ZL_Surface srfPattern;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitGlobal()
	{
		srfPattern = ZL_Surface("Data/PATTERN.png").SetTextureRepeatMode();
	}

	//Clear screen and draw the surface many times with batch rendering enabled
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		srfPattern.DrawTo(0,0,ZLWIDTH,ZLHEIGHT);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Surface with Repeating Texture", 854, 480);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
