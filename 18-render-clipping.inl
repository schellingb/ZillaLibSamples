struct sMain : public ZL_Application
{
	ZL_Surface srfLogo;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Render Clipping", 854, 480);
		srfLogo = ZL_Surface("Data/ZILLALIB.png");
	}

	void AfterFrame()
	{
		float t = ZLSINCESECONDS(0);
		ZL_Rectf ClipRect(ZLHALFW, ZLHALFH + scos(t)*50.0f, 160.0f);
		ZL_Display::ClearFill(ZL_Color::Black); //clear whole screen
		ZL_Display::FillRect(ClipRect+5.0f, ZL_Color::Green); //draw a green rectangle highliting the clipped area
		ZL_Display::SetClip(ClipRect); //clip the rendering to the rect
		ZL_Display::ClearFill(ZL_Color::Blue); //clear the screen (meaning only clear the now clipped area)
		srfLogo.Draw(300 + ssin(t)*100, 100); //draw a surface moving left and right
		ZL_Display::FillCircle(ZLCENTER + ZL_Vector::FromAngle(t)*200.0f, 50, ZL_Color::Yellow); //draw a circle moving in a circle
		ZL_Display::ResetClip(); //reset the clipping rectangle
	}
} Main;
