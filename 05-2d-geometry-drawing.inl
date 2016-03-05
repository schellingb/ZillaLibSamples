#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	sSceneGame() : ZL_Scene(SCENE_GAME) { }
	void Draw()
	{
		//use sine of current tick counter for pulsating effect
		float ticksin = ssin(s(ZLTICKS)/s(1000))*s(10);
		ZL_Display::ClearFill(ZL_Color::Black);
		ZL_Display::DrawLine(150+ticksin, 10+ticksin, 10, 250, ZL_Color::Blue);
		ZL_Display::DrawRect(200,10, 400+ticksin, 100+ticksin, ZL_Color::Red, ZL_Color::Yellow);
		ZL_Display::DrawCircle(140, 150, 100+ticksin, ZL_Color::Green, ZLRGBA(0,1,1,0.4));
		ZL_Display::DrawTriangle(ZL_Vector(330, 180), ZL_Vector(50+ticksin, 320+ticksin), ZL_Vector(190, 460),
			ZL_Color::Orange, ZL_Color::Magenta);
		ZL_Display::DrawEllipse(660, 400+ticksin, 120+ticksin, 60, ZL_Color::Red);
		ZL_Display::DrawBezier(400, 400, 500+ticksin*2, 500+ticksin*3, 600, 200, 770, 250+ticksin, ZL_Color::Blue);
		ZL_Display::FillGradient(500+ticksin, 5, 795, 80+ticksin,
			ZL_Color::Red, ZL_Color::Blue, ZL_Color::Green, ZL_Color::Yellow);
		ZL_Vector p[] = { ZL_Vector(380, 170), ZL_Vector(380, 300), ZL_Vector(470+ticksin, 200),
			ZL_Vector(780, 240+ticksin), ZL_Vector(700, 100) };
		ZL_Polygon(ZL_Polygon::BORDER_FILL).Add(p, COUNT_OF(p)).Draw(ZL_Color::Red, ZL_Color::Yellow);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("2D Geometry Drawing", 854, 480);
		ZL_Display::SetThickness(4.0f);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
