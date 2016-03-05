//This was supposed to be a bit more simple but it turned out that blending is quite a complex subject
//So in the end this is just a visualization tool and not good sample code in itself

//string and array representation of all blend equations (for display and looping)
static const char* be_names[] = { "ADD", "MIN", "MAX", "SUBTRACT", "REVERSE_SUBTRACT" };
static ZL_Display::BlendEquation bes[] = { ZL_Display::BLENDEQUATION_ADD, ZL_Display::BLENDEQUATION_MIN, ZL_Display::BLENDEQUATION_MAX, ZL_Display::BLENDEQUATION_SUBTRACT, ZL_Display::BLENDEQUATION_REVERSE_SUBTRACT };

//string and array representation of all blend funcs (for display and looping)
static const char* bf_names[] = { "SRCALPHA", "INVSRCALPHA", "SRCCOLOR", "INVSRCCOLOR", "DESTCOLOR", "INVDESTCOLOR", "ZERO", "ONE", "DESTALPHA", "INVDESTALPHA", "CONSTCOLOR", "INVCONSTCOLOR", "CONSTALPHA", "INVCONSTALPHA", "SRCALPHASATURATE" };
static ZL_Display::BlendFunc bfs[] =  { ZL_Display::BLEND_SRCALPHA, ZL_Display::BLEND_INVSRCALPHA, ZL_Display::BLEND_SRCCOLOR, ZL_Display::BLEND_INVSRCCOLOR, ZL_Display::BLEND_DESTCOLOR, ZL_Display::BLEND_INVDESTCOLOR, ZL_Display::BLEND_ZERO, ZL_Display::BLEND_ONE, ZL_Display::BLEND_DESTALPHA, ZL_Display::BLEND_INVDESTALPHA, ZL_Display::BLEND_CONSTCOLOR, ZL_Display::BLEND_INVCONSTCOLOR, ZL_Display::BLEND_CONSTALPHA, ZL_Display::BLEND_INVCONSTALPHA, ZL_Display::BLEND_SRCALPHASATURATE };

//mode states, color states, surfaces and used font
static int bf_rgb = 0, bf_alpha = 0, bm_alpha_src = -1, bm_alpha_dst = -1, bf_offset_src = 0, bf_offset_dst = 0, use_render_to_texture = 0;
static float CD = 12.0f;
static int colHues[] = { 0, 10, 8, 0 }, colSats[] = { 0, 12, 12, 0 }, colVals[] = { 6, 12, 12, 12 }, colAlphas[] = { 12, 12, 12, 12 };
static ZL_Color GetHueSatVal(int num) { return ZLHSVA(colHues[num]/CD, colSats[num]/CD, colVals[num]/CD, colAlphas[num]/CD); }
static ZL_Surface srfTests[5], *srfLayers[3], srfRenderTarget;
static ZL_Font fnt;

struct sMain : public ZL_Application
{
	sMain() : ZL_Application(0) {}

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Blend Modes", 1280, 720);
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png").SetScale(0.6f).SetColor(ZLBLACK);

		//the render target with alpha channel that is used to demonstrate modes that rely on a target with alpha
		srfRenderTarget = ZL_Surface(1024, 1024, true);

		//generate a test texture with one filled circle with 50% alpha
		srfTests[0] = ZL_Surface(64, 64, true).SetTextureRepeatMode().SetOrigin(ZL_Origin::Center);
		srfTests[0].RenderToBegin(true);
		ZL_Display::FillCircle(32, 32, 32, ZL_Color(1,1,1,0.5));
		srfTests[0].RenderToEnd();

		//generate a test texture with a gradient from black to white
		srfTests[1] = ZL_Surface(64, 64).SetTextureRepeatMode().SetOrigin(ZL_Origin::Center);
		srfTests[1].RenderToBegin(true);
		for (float f = 0; f < 64; f += 1.0) ZL_Display::FillRect(0, f, 64, f+1, ZLLUM(f/64));
		srfTests[1].RenderToEnd();

		//generate a test texture with three circles
		srfTests[2] = ZL_Surface(64, 64, true).SetTextureRepeatMode().SetOrigin(ZL_Origin::Center);
		srfTests[2].RenderToBegin(true);
		ZL_Display::FillCircle(16, 16, 16, ZL_Color::Red);
		ZL_Display::FillCircle(48, 16, 16, ZL_Color::Green);
		ZL_Display::FillCircle(32, 48, 16, ZL_Color::Blue);
		srfTests[2].RenderToEnd();

		//load a test texture from a file and scale it to be the same size as the other generated test textures
		srfTests[3] = ZL_Surface("Data/ZILLALIB.png").SetScaleTo(64, 64).SetTextureRepeatMode().SetOrigin(ZL_Origin::Center);

		//generate a test texture that is fully white
		srfTests[4] = ZL_Surface(64, 64, true).SetTextureRepeatMode().SetOrigin(ZL_Origin::Center);
		srfTests[4].RenderToBegin(true);
		ZL_Display::ClearFill(ZLWHITE);
		srfTests[4].RenderToEnd();

		//set the default surfaces for background, layer 1 and layer 2
		srfLayers[0] = &srfTests[4];
		srfLayers[1] = srfLayers[2] = &srfTests[0];
	}

	void AfterFrame()
	{
		ZL_Display::ClearFill(ZL_Color::Gray);

		//draw the main 8x8 grid of blended textures either to a texture or directly to the screen
		if (use_render_to_texture) srfRenderTarget.RenderToBegin(true);
		srfLayers[0]->DrawTo(137.0f, 5.0f, 921.0f, 573.0f, GetHueSatVal(0));
		ZL_Display::SetBlendEquationSeparate(bes[bf_rgb], bes[bf_alpha]);
		ZL_Display::SetBlendConstantColor(GetHueSatVal(3));
		for (int x = 0; x < 8 && bf_offset_src+x < (int)COUNT_OF(bfs); x++)
		{
			for (int y = 0; y < 8 && bf_offset_dst+y < (int)COUNT_OF(bfs); y++)
			{
				ZL_Display::SetBlendModeSeparate(bfs[bf_offset_src+x], bfs[bf_offset_dst+y], bfs[bm_alpha_src < 0 ? bf_offset_src+x : bm_alpha_src], bfs[bm_alpha_dst < 0 ? bf_offset_dst+y : bm_alpha_dst]);
				srfLayers[1]->Draw(170.0f+98.0f*x,    540.0f-71.0f*y,   GetHueSatVal(1));
				srfLayers[2]->Draw(170.0f+98.0f*x+32, 540.0f-71.0f*y-5, GetHueSatVal(2));
			}
		}
		ZL_Display::ResetBlendFunc();
		ZL_Display::ResetBlendEquation();
		if (use_render_to_texture) srfRenderTarget.RenderToEnd();
		if (use_render_to_texture) srfRenderTarget.Draw(0, 0);

		//Draw the grid lines and the blend mode labels
		for (int i = 0; i <= 8; i++)
		{
			if (i < 8 && bf_offset_src+i < (int)COUNT_OF(bfs)) fnt.Draw(185.0f+98.0f*i, 590, bf_names[bf_offset_src+i], ZL_Origin::Center);
			if (i < 8 && bf_offset_dst+i < (int)COUNT_OF(bfs)) fnt.Draw(125, 535.0f-71.0f*i, bf_names[bf_offset_dst+i], ZL_Origin::CenterRight);
			if (bf_offset_src+i <= (int)COUNT_OF(bfs)) ZL_Display::DrawLine(137.0f+98.0f*i,              0, 137.0f+98.0f*i,            610, fnt.GetColor());
			if (bf_offset_dst+i <= (int)COUNT_OF(bfs)) ZL_Display::DrawLine(             0, 573.0f-71.0f*i,         921.0f, 573.0f-71.0f*i, fnt.GetColor());
		}

		//Draw the render texture toggle
		if (Button(ZL_Rectf(10.0f, 620.0f, 160.0f, 620.0f+69.0f), "Use render to texture\n\n(with empty alpha channel)", (use_render_to_texture!=0))) use_render_to_texture ^= 1;

		//Draw the from/to toggles
		if (Button(ZL_Rectf(70.0f, 580.0f, 130.0f, 610.0f), "FROM:", (bf_offset_src!=0))) bf_offset_src = (bf_offset_src ? 0 : 8);
		if (Button(ZL_Rectf(10.0f, 580.0f,  65.0f, 610.0f), "TO:"  , (bf_offset_dst!=0))) bf_offset_dst = (bf_offset_dst ? 0 : 8);

		//Draw the surface selection for background and the two layers
		for (int layer = 0; layer <= 2; layer ++)
		{
			fnt.Draw(180.0f+170.0f+layer*370.0f, 705, (layer == 0 ? "BACKGROUND" : (layer == 1 ? "LAYER1" : "LAYER2")), ZL_Origin::Center);
			for (int srf = 0; srf < (int)COUNT_OF(srfTests); srf++)
				if (Button(ZL_Rectf(180.0f+layer*370.0f+srf*70.0f, 620.0f, 180.0f+layer*370.0f+srf*70.0f+69.0f, 620.0f+69.0f), NULL, (srfLayers[layer] == &srfTests[srf]), &srfTests[srf]))
					srfLayers[layer] = &srfTests[srf];
		}

		//Draw the color selection grid for the 4 colors
		for (int c = 0; c < 4; c++)
		{
			float x = 1020.0f, y = 610.0f - 60.0f - c * 68.0f, bx = x;
			fnt.Draw(x-50.0f, y+30, (c == 0 ? "BACK\nGROUND" : (c == 1 ? "LAYER1" : (c == 2 ? "LAYER2" : "CONST"))), ZL_Origin::CenterRight);
			fnt.Draw(x-10.0f, y+52, "Hue",   ZL_Origin::CenterRight);
			fnt.Draw(x-10.0f, y+37, "Sat",   ZL_Origin::CenterRight);
			fnt.Draw(x-10.0f, y+22, "Val",   ZL_Origin::CenterRight);
			fnt.Draw(x-10.0f, y+ 7, "Alpha", ZL_Origin::CenterRight);
			for (int a = 0; a <= 12; a++, bx += 19.0f)
			{
				if (Button(ZL_Rectf(bx, y+45.0f, bx+18.0f, y+59.0f), NULL, (colHues[c] == a), NULL, ZLHSV(a/CD,colSats[c]/CD,colVals[c]/CD))) colHues[c] = a;
				if (Button(ZL_Rectf(bx, y+30.0f, bx+18.0f, y+44.0f), NULL, (colSats[c] == a), NULL, ZLHSV(colHues[c]/CD,a/CD,colVals[c]/CD))) colSats[c] = a;
				if (Button(ZL_Rectf(bx, y+15.0f, bx+18.0f, y+29.0f), NULL, (colVals[c] == a), NULL, ZLHSV(colHues[c]/CD,colSats[c]/CD,a/CD))) colVals[c] = a;
				if (Button(ZL_Rectf(bx, y+ 0.0f, bx+18.0f, y+14.0f), NULL, (colAlphas[c] == a), NULL, ZLLUM(a/CD))) colAlphas[c] = a;
			}
		}

		//Draw the separate blend func selections
		fnt.Draw(1030.0f, 330.0f, "Separate Src Alpha Func",  ZL_Origin::Center);
		fnt.Draw(1195.0f, 330.0f, "Separate Dest Alpha Func", ZL_Origin::Center);
		for (int i = -1; i < (int)COUNT_OF(bfs); i++)
		{
			if (Button(ZL_Rectf( 960.0f, 293.0f-i*14.0f, 1100.0f, 293.0f-i*14.0f+13.0f), (i < 0 ? "SAME AS RGB" : bf_names[i]), (bm_alpha_src == i))) bm_alpha_src = i;
			if (Button(ZL_Rectf(1125.0f, 293.0f-i*14.0f, 1265.0f, 293.0f-i*14.0f+13.0f), (i < 0 ? "SAME AS RGB" : bf_names[i]), (bm_alpha_dst == i))) bm_alpha_dst = i;
		}

		//Draw the blend equation selections
		fnt.Draw(1030.0f, 83.0f, "Blend Equation RGB", ZL_Origin::Center);
		fnt.Draw(1195.0f, 83.0f, "Blend Equation Alpha", ZL_Origin::Center);
		for (int i = 0; i < (int)COUNT_OF(bes); i++)
		{
			if (Button(ZL_Rectf( 960.0f, 5+i*14.0f, 1100.0f, 5+i*14.0f+13.0f), be_names[i], (bf_rgb == i))) bf_rgb = i;
			if (Button(ZL_Rectf(1125.0f, 5+i*14.0f, 1265.0f, 5+i*14.0f+13.0f), be_names[i], (bf_alpha == i))) bf_alpha = i;
		}
	}

	//extremely simple UI, draw a rectangle with text, image or color in it and return if it has been clicked
	bool Button(const ZL_Rectf& rec, const char* txt, bool toggled = false, ZL_Surface* surface = NULL, const ZL_Color& color_add = ZLTRANSPARENT)
	{
		ZL_Color fill = ZLALPHA(ZL_Input::Held(rec) ? .6 : (ZL_Input::Hover(rec) ? .3 : .1));
		if (color_add.a) fill -= ZLRGBA(1-color_add.r,1-color_add.g,1-color_add.b,-.5);
		else if (surface) fill -= ZLRGBA(1,.5,1,-.5);
		else if (toggled) fill -= ZLRGBA(.6,.6,.3,0);
		ZL_Display::DrawRect(rec, (toggled ? ZLRGBA(1,0,0,.8) : ZLALPHA(.8)), fill);
		if (txt && txt[0]) fnt.Draw(rec.Center(), txt, ZL_Origin::Center);
		if (surface) surface->Draw(rec.Center());
		return (ZL_Input::Down(rec) != 0);
	}
} Main;
