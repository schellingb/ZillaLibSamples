enum { SCENE_MAIN = 1 };
static ZL_Font fnt;

struct sSceneMain : public ZL_Scene
{
	sSceneMain() : ZL_Scene(SCENE_MAIN) { }
	ZL_String GetTextData;
	float GetNumericalData;
	vector<unsigned char> GetBinaryData;

	virtual void InitGlobal()
	{
		//write data to settings file
		unsigned char binary_data[] = { 1, 2, 3, 4 };
		ZL_Application::SettingsSet("TextData", "Hello");
		ZL_Application::SettingsSet("NumericalData", ZL_Rand::Range(1, 10));
		ZL_Application::SettingsSet("BinaryData", ZL_Base64::Encode(binary_data, sizeof(binary_data)));
		ZL_Application::SettingsSynchronize();

		//read data from settings file
		GetTextData = ZL_Application::SettingsGet("TextData");
		GetNumericalData = ZL_Application::SettingsGet("NumericalData");
		ZL_Base64::Decode(ZL_Application::SettingsGet("BinaryData"), GetBinaryData);
	}

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		//show the data read from the settings file
		fnt.Draw(100, 400, "TextData:");          fnt.Draw(400, 400, GetTextData);
		fnt.Draw(100, 250, "NumericalData:");     fnt.Draw(400, 250, ZL_String(GetNumericalData));
		fnt.Draw(100, 100, "BinaryData Length:"); fnt.Draw(400, 100, ZL_String(GetBinaryData.size()));
	}
} SceneMain;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Saving/Loading Settings", 854, 480);
		ZL_Application::SettingsInit("GameSettings");
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_MAIN);
	}
} Main;
