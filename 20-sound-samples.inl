struct sMain : public ZL_Application
{
	ZL_Font fnt;
	ZL_Sound sndNormal, sndFrequency, sndStream;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Sound Samples", 854, 480);
		ZL_Audio::Init();
		ZL_Input::Init();

		fnt = ZL_Font("Data/fntMain.png");
		sndNormal = ZL_Sound("Data/ACCURACY.ogg");
		sndFrequency = sndNormal.Clone();
		sndStream = ZL_Sound("Data/ACCURACY.ogg", true);
	}

	void AfterFrame()
	{
		ZL_Display::ClearFill(ZL_Color::Black); //clear whole screen
		if (Button(ZL_Rectf::BySize( 50.0f, 190.0f, 200.0f, 100.0f), "Play Audio Sample"))
			sndNormal.Play();
		if (Button(ZL_Rectf::BySize(300.0f, 190.0f, 200.0f, 100.0f), "Play sample with\ndifferent frequency"))
			sndFrequency.SetSpeedFactor(RAND_RANGE(0.5,2.0)).Play();
		if (Button(ZL_Rectf::BySize(550.0f, 190.0f, 200.0f, 100.0f), "Stream sound\n(sounds the same, but\nstreamed from disk,\nintended for music)"))
			sndStream.Play();
	}

	//extremely simple UI, draw a rectangle with text in it and return if it has been clicked
	bool Button(const ZL_Rectf& rec, const char* txt)
	{
		ZL_Display::DrawRect(rec, ZLALPHA(.8), ZLALPHA(ZL_Input::Held(rec) ? .6 : (ZL_Input::Hover(rec) ? .3 : .1)));
		fnt.Draw(rec.Center(), txt, ZL_Origin::Center);
		return (ZL_Input::Clicked(rec) != 0);
	}
} Main;
