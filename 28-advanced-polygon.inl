struct sMain : public ZL_Application
{
	sMain() : ZL_Application() {}

	ZL_Polygon Poly, PolyExtrude;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Advanced Polygon Usage", 854, 480);

		vector<ZL_Polygon::PointList> PointLists(3);

		//make an outline in a circle consisting of 20 points with positive winding (counterclockwise)
		for (float a = 0; a < PI2; a+=(PI2/20)) PointLists[0].push_back(ZL_Vector::FromAngle(a) * 200 + ZL_Vector::Up*0);

		//make two thin quads that go across the circle but with negative winding (clockwise)
		float Angles[] = { 0, PIHALF/2, PIHALF/2*3 }, AngleSpread = PI2/50;
		for (int i = 1; i <= 2; i++)
		{
			PointLists[i].push_back(ZL_Vector::FromAngle(Angles[i]+AngleSpread   )*300);
			PointLists[i].push_back(ZL_Vector::FromAngle(Angles[i]-AngleSpread   )*300);
			PointLists[i].push_back(ZL_Vector::FromAngle(Angles[i]+AngleSpread+PI)*300);
			PointLists[i].push_back(ZL_Vector::FromAngle(Angles[i]-AngleSpread+PI)*300);
		}

		//Tesselate a polygon with a border and filled area that filters to positive winding (curring out the thin quads from the circle)
		Poly = ZL_Polygon(ZL_Polygon::BORDER_FILL).Add(PointLists, ZL_Polygon::POSITIVE);

		//Create a textured 20 wide outside extrude from the generated borders
		PolyExtrude = ZL_Polygon(ZL_Surface("Data/extrude.png").SetTextureRepeatMode()).ExtrudeFromBorder(Poly, 20.0f);
	}

	void AfterFrame()
	{
		//Clear screen to black
		ZL_Display::ClearFill();

		//define the display to be horizontally -500 to 500 and vertically fitting by screen aspect ratio
		ZL_Display::PushOrtho(-500, 500, -500/ZLASPECTR, 500/ZLASPECTR);

		//Draw the main poly filled with white
		Poly.Fill(ZL_Color::White);

		//Draw the textured extrude polygon
		PolyExtrude.Draw();

		//Reset view matrix to default
		ZL_Display::PopOrtho();
	}
} Main;
