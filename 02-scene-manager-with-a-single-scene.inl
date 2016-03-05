#define SCENE_MAIN 1

struct sSceneMain : public ZL_Scene
{
	sSceneMain() : ZL_Scene(SCENE_MAIN) { }

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Blue);
	}
} SceneMain;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Scene Manager With a Single Scene", 854, 480);
		ZL_SceneManager::Init(SCENE_MAIN);
	}
} Main;
