#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	ZL_Surface srfBlock;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitGlobal()
	{
		srfBlock = ZL_Surface("Data/PATTERN.png");
	}

	//Clear screen and draw the surface many times with batch rendering enabled
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		srfBlock.BatchRenderBegin(true);
		for (scalar x = 10, maxX = ZLFROMW(34), maxY = ZLFROMH(34); x < maxX; x += s(24))
			for (scalar y = 10; y < maxY; y += s(24))
				srfBlock.Draw(x, y, ZL_Color::LUM(RAND_FACTOR));
		srfBlock.BatchRenderEnd();
	}
} SceneGame;

struct sMain : public ZL_Application
{
	//we set fps limit to 0 to have unlocked frame rate
	sMain() : ZL_Application(0) {}

	ZL_Font fntMain;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Surface Batch Rendering", 854, 480);
		fntMain = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_GAME);
	}

	//display fps
	void AfterFrame()
	{
		fntMain.Draw(ZLFROMW(30), ZLFROMH(30), (const char*)ZL_String::format("%d FPS", FPS), ZL_Color::White, ZL_Origin::CenterRight);
	}
} Main;
