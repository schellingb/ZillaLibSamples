#define SCENE_GAME 1
ZL_Font fntMain;

struct sSceneGame : public ZL_Scene
{
	ZL_String msg;
	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void OnKeyDown(ZL_KeyboardEvent& e)
	{
		if (e.key == ZLK_SPACE) msg = "Pressed the Space key";
		else msg = ZL_String("Pressed another key: ") << ZL_Display::KeyScancodeName(e.key);
	}

	void OnPointerMove(ZL_PointerMoveEvent& e)
	{
		msg = ZL_String::format("Pointer Num: %d - X: %d - Y: %d - RelX: %.1f - RelY: %.1f - State: %d", e.which, (int)e.x, (int)e.y, e.xrel, e.yrel, e.state);
	}

	void OnResize(ZL_WindowResizeEvent& e)
	{
		msg = ZL_String::format("Resized to: %d x %d", (int)ZLWIDTH, (int)ZLHEIGHT);
	}

	//Set up the event listeners
	void InitAfterTransition()
	{
		ZL_Display::sigKeyDown.connect(this, &sSceneGame ::OnKeyDown);
		ZL_Display::sigPointerMove.connect(this, &sSceneGame ::OnPointerMove);
		ZL_Display::sigResized.connect(this, &sSceneGame ::OnResize);
	}

	//Stop listening to the events and clear the message buffer
	void DeInitLeave(ZL_SceneType SceneTypeTo)
	{
		ZL_Display::AllSigDisconnect(this);
		msg = ZL_String();
	}

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Blue);
		fntMain.Draw(ZLHALFW, ZLHALFH, msg, ZL_Origin::Center);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Input and Other Events", 854, 480);
		fntMain = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
