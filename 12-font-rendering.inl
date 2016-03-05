#define SCENE_GAME 1
ZL_Font fntTex;
ZL_Font fntTTF;

struct sSceneGame : public ZL_Scene
{
	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	//Clear screen and draw some fonts
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		fntTex.Draw(20, 450, "Bitmap Font Text");
		fntTex.Draw(ZLHALFW, 420, "Centered Text", ZL_Origin::BottomCenter);
		fntTex.Draw(ZLFROMW(10), 390, "Right Aligned Text", ZL_Origin::BottomRight);
		fntTex.Draw(20, 310, "Scaled Text", 4, 2);
		fntTex.Draw(20, 270, "Colored Text", ZL_Color::Red);
		fntTex.Draw(20, 250, "Colored Text", ZL_Color::Green);
		fntTex.Draw(20, 230, "Colored Text", ZL_Color::Blue);
		fntTTF.Draw(20, 140, "TRUE TYPE FONT TEXT");

		ZL_Display::DrawRect(18, 18, 202, 102, ZL_Color::White);
		fntTex.CreateBuffer("Hello World! Automatically word-wrapped text is supported.", 180, true).Draw(20, 100, ZL_Origin::TopLeft);

		ZL_Display::DrawRect(ZLFROMW(202), 18, ZLFROMW(18), 102, ZL_Color::White);
		fntTex.CreateBuffer(s(1), "Word-wrapped text can be aligned horizontally, too.", 180, true).Draw(ZLFROMW(20), 100, ZL_Origin::TopRight);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Font Rendering", 854, 480);
		fntTex = ZL_Font("Data/fntMain.png");
		fntTTF = ZL_Font("Data/alphabot.ttf.zip", 55).SetCharSpacing(5);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
