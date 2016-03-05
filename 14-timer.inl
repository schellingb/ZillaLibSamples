#define SCENE_MAIN 1
static ZL_Font fnt;

struct sSceneMain : public ZL_Scene
{
	bool FlagA, FlagD, FlagF;
	float ValB, ValC;
	int CountE;
	sSceneMain() : ZL_Scene(SCENE_MAIN), FlagA(true), FlagD(true), FlagF(true), ValB(0), ValC(0), CountE(0) { }

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);

		float x = 40;
		ZL_Display::FillCircle(x+95, 300, 50, (FlagA ? ZL_Color::Red : ZL_Color::Green));
		if (Button(ZL_Rectf::BySize(x, 100, 190, 100), "[A]\nClick here\nto start a 1s timer"))
			ZL_Timer::AddSingleTimer(1000)->sigDone.connect(this, &sSceneMain::CallbackBasicTimer);

		x += 200;
		ZL_Display::FillCircle(x+95, 300+ValB, 50, ZLWHITE);
		if (Button(ZL_Rectf::BySize(x, 100, 190, 100), "[B]\nClick here to\nstart a transition"))
			ZL_Timer::AddTransitionFloat(&(ValB = 0.0f), 300.0f, 1000, 0, ZL_Timer::NoEasing);

		x += 200;
		ZL_Display::FillCircle(x+95, 300+ValC, 50, ZLWHITE);
		if (Button(ZL_Rectf::BySize(x, 100, 190, 100), "[C]\nClick here to start\na bouncy transition"))
			ZL_Timer::AddTransitionFloat(&(ValC = 0.0f), 300.0f, 1000, 0, ZL_Easing::OutBounce);

		x += 200;
		ZL_Display::FillCircle(x+95, 300, 50, (FlagD ? ZL_Color::Red : ZL_Color::Green));
		if (Button(ZL_Rectf::BySize(x, 100, 190, 100), "[D]\nClick here to start a\ntimer that is fired\n4 times every 250 ms"))
			ZL_Timer::AddMultiTimer(250, 4)->sigCall.connect(this, &sSceneMain::CallbackMultiTimer);

		x += 200;
		fnt.Draw(x+95, 300, ZL_String::format("%d calls", CountE), ZL_Origin::Center);
		if (Button(ZL_Rectf::BySize(x, 100, 190, 100), "[E]\nClick here to\nstart a 1s ticker"))
			ZL_Timer::AddLimitedTicker(1000)->sigCall.connect(this, &sSceneMain::CallbackTicker);

#ifdef ZL_LAMBDA_SUPPORT
		x += 200;
		ZL_Display::FillCircle(x+95, 300, 50, (FlagF ? ZL_Color::Red : ZL_Color::Green));
		if (Button(ZL_Rectf::BySize(x, 100, 190, 100), "[F]\nClick here to start\na 1s lambda timer"))
		{
			ZL_Timer::AddSingleTimer(1000)->sigDone.connect_lambda([this]()
			{
				FlagF ^= 1;
			});
		}
#endif
	}

	//[A] timer callback function
	void CallbackBasicTimer()
	{
		FlagA ^= 1;
	}

	//[D] multi timer callback function
	void CallbackMultiTimer(ZL_RepeatingTimer* t)
	{
		FlagD ^= 1;
	}

	//[E] ticker callback function
	void CallbackTicker(ZL_RepeatingTimer*)
	{
		CountE++;
	}

	//extremely simple UI, draw a rectangle with text in it and return if it has been clicked
	bool Button(const ZL_Rectf& rec, const char* txt)
	{
		ZL_Display::DrawRect(rec, ZLALPHA(.8), ZLALPHA(ZL_Input::Hover(rec) ? .3 : .1));
		fnt.Draw(rec.Center(), txt, ZL_Origin::Center);
		return (ZL_Input::Clicked(rec) != 0);
	}
} SceneMain;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Timers, Tickers, Transitions", 1280, 720);
		ZL_Timer::Init();
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_MAIN);
	}
} Main;
