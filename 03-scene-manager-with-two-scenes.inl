#define SCENE_TITLE 1
#define SCENE_GAME 2
ZL_Font fnt;

struct sSceneGame : public ZL_Scene
{
	sSceneGame() : ZL_Scene(SCENE_GAME) { }
	int InitTransitionEnter(ZL_SceneType SceneTypeFrom, void* data) { return 500; }
	int DeInitTransitionLeave(ZL_SceneType SceneTypeTo) { return 500; }
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Blue);
		fnt.Draw(ZLHALFW, ZLHALFH, "THIS IS THE GAME SCENE", ZL_Origin::Center);
	}
	void DrawTransition(scalar f, bool IsLeaveTransition)
	{
		Draw();
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLRGBA(0, 0, 0, f));
	}
} sSceneGame;

struct sSceneTitle : public ZL_Scene
{
	sSceneTitle() : ZL_Scene(SCENE_TITLE) { }
	int InitTransitionEnter(ZL_SceneType SceneTypeFrom, void* data) { return 500; }
	int DeInitTransitionLeave(ZL_SceneType SceneTypeTo) { return 500; }
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Cyan);
		fnt.Draw(ZLHALFW, ZLHALFH, "TITLE SCENE", ZL_Color::Black, ZL_Origin::Center);
	}
	void DrawTransition(scalar f, bool IsLeaveTransition)
	{
		Draw();
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLRGBA(0, 0, 0, f));
	}
} sSceneTitle;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Scene Manager With Two Scenes", 854, 480);
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_TITLE);
		ZL_Display::sigPointerDown.connect(this, &sMain::OnPointerDown);
	}
	void OnPointerDown(ZL_PointerPressEvent& e)
	{
		ZL_SceneManager::GoToScene(ZL_SceneManager::GetCurrent()->SceneType == SCENE_TITLE ? SCENE_GAME : SCENE_TITLE);
	}
} Main;
