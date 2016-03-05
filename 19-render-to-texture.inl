struct sMain : public ZL_Application
{
	ZL_Surface srfBuffer;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Render To Texture", 854, 480);
		ZL_Input::Init();

		srfBuffer = ZL_Surface(256, 256); //initialize a 256x256 render target texture
		srfBuffer.RenderToBegin(); //start drawing onto the render target texture
		ZL_Display::ClearFill(ZL_Color::Red); //clear the texture to fully red
		ZL_Surface srfLogo("Data/ZILLALIB.png"); //load another surface texture
		srfLogo.DrawTo( 10.0f,  10.0f,  90.0f,  90.0f); //draw the newly loaded surface texture into the buffer
		srfLogo.DrawTo( 70.0f,  70.0f, 150.0f, 150.0f); //again
		srfLogo.DrawTo(130.0f, 130.0f, 210.0f, 210.0f); //again
		srfBuffer.RenderToEnd(); //end drawing to the texture
	}

	void AfterFrame()
	{
		ZL_Vector SurfacePosition(299.0f, 112.0f);
		if (ZL_Input::Held())
		{
			//Draw into the buffer while the mouse is pressed, draw in 1 pixel steps from the old mouse position to the current position
			ZL_Vector MousePosInImage = ZL_Input::Pointer() - SurfacePosition, MouseMoveDir = ZL_Input::PointerDelta().VecNorm();
			scalar MouseMoveTotal = (ZL_Input::Down() ? 0.0f : ZL_Math::Max(ZL_Input::PointerDelta().GetLength() - 1.0f, 0.0f));
			srfBuffer.RenderToBegin();
			for (scalar i = 0; i <= MouseMoveTotal; i++)
				ZL_Display::FillCircle(MousePosInImage - MouseMoveDir*i, 2.0f, ZL_Color::Blue);
			srfBuffer.RenderToEnd();
		}

		ZL_Display::ClearFill(ZL_Color::Black); //clear whole screen
		srfBuffer.Draw(SurfacePosition);
	}
} Main;
