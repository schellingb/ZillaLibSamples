enum { SCENE_GAME = 1, EASING_FUNC_COUNT = 30 };

typedef scalar (*EasingFunc)(scalar);
static EasingFunc funcs[EASING_FUNC_COUNT] = {
	ZL_Easing::InSine,    ZL_Easing::OutSine,    ZL_Easing::InOutSine,    ZL_Easing::InQuad,   ZL_Easing::OutQuad,   ZL_Easing::InOutQuad,
	ZL_Easing::InCubic,   ZL_Easing::OutCubic,   ZL_Easing::InOutCubic,   ZL_Easing::InQuart,  ZL_Easing::OutQuart,  ZL_Easing::InOutQuart,
	ZL_Easing::InQuint,   ZL_Easing::OutQuint,   ZL_Easing::InOutQuint,   ZL_Easing::InExpo,   ZL_Easing::OutExpo,   ZL_Easing::InOutExpo,
	ZL_Easing::InCirc,    ZL_Easing::OutCirc,    ZL_Easing::InOutCirc,    ZL_Easing::InBack,   ZL_Easing::OutBack,   ZL_Easing::InOutBack,
	ZL_Easing::InElastic, ZL_Easing::OutElastic, ZL_Easing::InOutElastic, ZL_Easing::InBounce, ZL_Easing::OutBounce, ZL_Easing::InOutBounce,
};
static const char* funcnames[EASING_FUNC_COUNT] = {
	"InSine",    "OutSine",    "InOutSine",    "InQuad",   "OutQuad",   "InOutQuad",
	"InCubic",   "OutCubic",   "InOutCubic",   "InQuart",  "OutQuart",  "InOutQuart",
	"InQuint",   "OutQuint",   "InOutQuint",   "InExpo",   "OutExpo",   "InOutExpo",
	"InCirc",    "OutCirc",    "InOutCirc",    "InBack",   "OutBack",   "InOutBack",
	"InElastic", "OutElastic", "InOutElastic", "InBounce", "OutBounce", "InOutBounce",
};
static ZL_Font fnt;

struct sSceneGame : public ZL_Scene
{
	int bigfuncindex;
	sSceneGame() : ZL_Scene(SCENE_GAME), bigfuncindex(-1) { }

	void Draw()
	{
		scalar t = sabs(smod(ZLSINCESECONDS(0), 2.0f) - 1.0f);
		ZL_Display::ClearFill(ZLBLACK);
		if (bigfuncindex >= 0)
		{
			scalar f = funcs[bigfuncindex](t);
			fnt.Draw(360, 660, funcnames[bigfuncindex], ZL_Origin::Center);
			ZL_Display::DrawRect(40, 40, 680, 640, ZLALPHA(.3));
			DrawCurve(ZL_Rectf(40, 40, 680, 640), funcs[bigfuncindex], 100);
			ZL_Display::FillCircle(40+t*640, 40+f*600, 20, ZLWHITE);
			ZL_Display::DrawRect(730, 40, 830, 40+640, ZLALPHA(.3));
			ZL_Display::FillCircle(780, 60+f*600, 20, ZLWHITE);
			ZL_Display::DrawRect(880, 40, 1240, 335, ZLALPHA(.3));
			ZL_Display::FillRect(880, 40, 880+f*360, 335, ZLWHITE);
			ZL_Display::DrawRect(880, 385, 1240, 680, ZLALPHA(.3));
			ZL_Display::FillRect(880, 385, 1240, 680, ZLBLACK+ZLWHITE*f);
			if (ZL_Input::Clicked()) bigfuncindex = -1;
			return;
		}
		for (int funccount = 0; funccount < EASING_FUNC_COUNT; funccount++)
		{
			ZL_Rectf rec = ZL_Rectf::BySize(30.0f + 130.0f * s(funccount % 9) + 40.0f * s((funccount % 9) / 3), 480.0f - 150.0f * s(funccount / 9), 100, 100);
			fnt.Draw(rec.left, rec.high, funcnames[funccount]);
			ZL_Display::DrawRect(rec, ZLALPHA(.3), (ZL_Input::Hover(rec) ? ZLALPHA(.1) : ZLTRANSPARENT));
			DrawCurve(rec, funcs[funccount], 20);
			if (ZL_Input::Hover(rec))
			{
				float fY = funcs[funccount](t)*rec.Height();
				ZL_Display::DrawLine(rec.left, rec.low+fY, rec.right, rec.low+fY, ZLALPHA(.3));
				ZL_Display::FillCircle(rec.left+t*rec.Width(), rec.low+fY, 4, ZLWHITE);
			}
			if (ZL_Input::Clicked(rec)) bigfuncindex = funccount;
		}
	}

	void DrawCurve(const ZL_Rectf area, EasingFunc func, int steps)
	{
		for (scalar w = area.Width(), h = area.Height(), step = 0, fa = 0, fb = 1.0f/s(steps); fa < 1.0f; fa = fb, fb = (++step)/s(steps))
			ZL_Display::DrawLine(area.left+fa*w, area.low+func(fa)*h, area.left+fb*w, area.low+func(fb)*h, ZLWHITE);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Easing Functions", 1280, 720);
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
