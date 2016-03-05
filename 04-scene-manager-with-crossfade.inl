#define SCENE_GAME 1
#define SCENE_MENU 2
ZL_Font fnt;

struct sSceneMenu : public ZL_Scene
{
	sSceneMenu() : ZL_Scene(SCENE_MENU) { }
	int InitTransitionEnter(ZL_SceneType SceneTypeFrom, void* data) { return -500; }
	int DeInitTransitionLeave(ZL_SceneType SceneTypeTo) { return -500; }
	virtual void DrawCrossfade(scalar f, bool IsLeaveTransition, ZL_Scene* pOtherScene)
	{
		f = ZL_Easing::InOutCubic(f);
		ZL_Display::ClearFill(ZLBLACK);
		ZL_Display::Translate(ZLWIDTH * (1-f), -ZLHEIGHT * (1-f));
		pOtherScene->Draw();
		ZL_Display::Translate(-ZLWIDTH * (1-f), ZLHEIGHT * (1-f));
		ZL_Display::Translate(0, ZLHEIGHT * f);
		Draw();
		ZL_Display::Translate(0, -ZLHEIGHT * f);
	}
	void Draw()
	{
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZL_Color::Blue);
		fnt.Draw(ZLHALFW, ZLHALFH, "THIS IS THE MENU SCENE", ZL_Origin::Center);
	}
} sSceneMenu;

struct sSceneGame : public ZL_Scene
{
	sSceneGame() : ZL_Scene(SCENE_GAME) { }
	void Draw()
	{
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZL_Color::Green);
		fnt.Draw(ZLHALFW, ZLHALFH, "GAME SCENE", ZL_Color::Black, ZL_Origin::Center);
	}
} sSceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Scene Manager With Crossfade", 854, 480);
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_GAME);
		ZL_Display::sigPointerDown.connect(this, &sMain::OnPointerDown);
	}
	void OnPointerDown(ZL_PointerPressEvent& e)
	{
		ZL_SceneManager::GoToScene(ZL_SceneManager::GetCurrent()->SceneType == SCENE_GAME ? SCENE_MENU : SCENE_GAME);
	}
} Main;
