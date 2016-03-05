struct sMain : public ZL_Application
{
	ZL_Font fnt;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Open Web Link", 854, 480);
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png");
	}

	void AfterFrame()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		if (Button(ZL_Rectf::BySize(277.0f, 160.0f, 300.0f, 160.0f), "Open Web Link\nhttps://zillalib.github.io/"))
			ZL_Application::OpenExternalUrl("https://zillalib.github.io/");
	}

	//extremely simple UI, draw a rectangle with text in it and return if it has been clicked
	bool Button(const ZL_Rectf& rec, const char* txt)
	{
		ZL_Display::DrawRect(rec, ZLALPHA(.8), ZLALPHA(ZL_Input::Held(rec) ? .6 : (ZL_Input::Hover(rec) ? .3 : .1)));
		fnt.Draw(rec.Center(), txt, ZL_Origin::Center);
		return (ZL_Input::Down(rec) != 0);
	}
} Main;
