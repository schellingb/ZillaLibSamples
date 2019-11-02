static ZL_Vector c1(820, 34), c2(500, 240);

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Collision Tests", 854, 480);
		ZL_Input::Init();
	}

	void AfterFrame()
	{
		ZL_Display::ClearFill(ZLBLACK);
		if (ZL_Input::Down()) { c1 = c2; c2 = ZL_Display::PointerPos(); }

		scalar r = 20;
		bool coll = false;
		ZL_Vector Collision, Collision2;

		ZL_Vector recvar(sin(s(ZLTICKS)/1000)*10, cos(s(ZLTICKS)/1000)*20);
		ZL_Rectf rec1(250, 350, ZL_Vector(60, 25)); rec1+=recvar; //area for line
		ZL_Rectf rec2(650, 350, ZL_Vector(60, 25)); rec2+=recvar; //area for point
		ZL_Rectf rec3(250, 150, ZL_Vector(60, 25)); rec3+=recvar; //area for aabb
		ZL_Rectf rec4(650, 150, ZL_Vector(60, 25)); rec4+=recvar; //area for rotbb

		//visualize swipe test (from c1 to c2 with radius r)
		ZL_Display::DrawLine(c1, c2, ZL_Color::Orange);
		ZL_Display::DrawCircle(c1, r, ZLRGBA(1,1,0,.3));
		ZL_Display::DrawCircle(c2, r, ZLRGBA(1,1,0,.3));
		ZL_Display::DrawLine(c1 + (c1 - c2).Perp().Norm()*r, c2 + (c1 - c2).Perp().Norm()*r, ZLRGBA(1,1,0,.3));
		ZL_Display::DrawLine(c1 - (c1 - c2).Perp().Norm()*r, c2 - (c1 - c2).Perp().Norm()*r, ZLRGBA(1,1,0,.3));
		ZL_Display::DrawLine(c2, c2 - (c1 - c2).Perp().Norm()*r - (c2 - c1).Norm()*r, ZLRGBA(1,1,0,.3));
		ZL_Display::DrawLine(c2, c2 + (c1 - c2).Perp().Norm()*r - (c2 - c1).Norm()*r, ZLRGBA(1,1,0,.3));

		//collision and swipe test on a line
		ZL_Vector p1 = rec1.LowLeft(), p2 = rec1.HighRight();
		ZL_Display::DrawLine(p1, p2, ZL_Color::White);
		int ncoll = ZL_Math::LineCircleCollision(p1, p2, c2, r, &Collision, &Collision2);
		if (ncoll >= 1) ZL_Display::DrawCircle(Collision, 5, ZL_Color::Green);
		if (ncoll >= 2) ZL_Display::DrawCircle(Collision2, 5, ZL_Color::Green);
		if (ncoll) coll = true;
		ZL_Vector CollisionLine;
		if (ZL_Math::CircleLineSweep(c1, c2, r, p1, p2, &CollisionLine))
			ZL_Display::DrawCircle(CollisionLine, r, ZL_Color::Red);

		//collision and swipe test on a single point
		ZL_Vector pp = rec2.Center();
		ZL_Display::DrawCircle(pp, 15, ZL_Color::White);
		ZL_Display::DrawLine(pp.x, pp.y-20, pp.x, pp.y+20, ZL_Color::White);
		ZL_Display::DrawLine(pp.x-20, pp.y, pp.x+20, pp.y, ZL_Color::White);
		if (ZL_Math::CirclePointSweep(c1, c2, r, pp, &Collision))
			ZL_Display::DrawCircle(Collision, r, ZL_Color::Red);
		if (pp-c2 <= r) coll = true;

		//collision and swipe test on axis aligned bounding box
		ZL_AABB aabb(rec3);
		ZL_Display::DrawRect(rec3, ZL_Color::White, ZLALPHA(.4));
		if (ZL_Math::CircleAABBSweep(c1, c2, r, aabb, &Collision))
			ZL_Display::DrawCircle(Collision, r, ZL_Color::Red);
		if (aabb.Overlaps(c2, r)) coll = true;

		//collision and swipe test on rotating bounding box
		ZL_RotBB bb(rec4, s(ZLTICKS)/s(600)); //s(.3)
		ZL_Display::PushMatrix();
		ZL_Display::Translate(bb.P);
		ZL_Display::Rotate(bb.A);
		ZL_Display::Translate(-bb.P);
		ZL_Display::DrawRect(rec4, ZL_Color::White, ZLALPHA(.4));
		ZL_Display::PopMatrix();
		if (ZL_Math::CircleRotBBSweep(c1, c2, r, bb, &Collision))
			ZL_Display::DrawCircle(Collision, r, ZL_Color::Red);
		if (bb.Overlaps(c2, r)) coll = true;

		//draw collision state at mouse cursor
		ZL_Display::DrawCircle(c2, r, (coll ? ZL_Color::Red : ZL_Color::White));
	}
} Main;
