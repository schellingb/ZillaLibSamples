extern TImcSongData imcSongData, imcSFXData, imcDataBOOM;
static ZL_SynthImcTrack imcSong(&imcSongData), imcSFX(&imcSFXData);
static ZL_Sound sndConverted = ZL_SynthImcTrack::LoadAsSample(&imcDataBOOM);

struct sMain : public ZL_Application
{
	ZL_Font fnt;

	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("ImcSynthesizer Sound", 854, 480);
		ZL_Audio::Init();
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png");
	}

	void AfterFrame()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		if (Button(ZL_Rectf::BySize( 80.0f, 270.0f, 300.0f, 160.0f), "Play IMC Music\n(embedded inside\nsource code)\n\n(sample song 1 by Reaby)"))
			(imcSong.IsPlaying() ? imcSong.Stop() : imcSong.Play());
		if (Button(ZL_Rectf::BySize(474.0f, 270.0f, 300.0f, 160.0f), "Play regular wave sample\nconverted from IMC\n(easier on CPU)"))
			sndConverted.Play();
		if (Button(ZL_Rectf::BySize( 80.0f, 50.0f, 300.0f, 160.0f), "Play IMC based\nsound effect"))
			imcSFX.NoteOn(0, 72);
		if (Button(ZL_Rectf::BySize(474.0f, 50.0f, 300.0f, 160.0f), "Play IMC based\nsound effect in\na random note"))
			imcSFX.NoteOn(0, ZL_Rand::Int(48,71));
	}

	//extremely simple UI, draw a rectangle with text in it and return if it has been clicked
	bool Button(const ZL_Rectf& rec, const char* txt)
	{
		ZL_Display::DrawRect(rec, ZLALPHA(.8), ZLALPHA(ZL_Input::Held(rec) ? .6 : (ZL_Input::Hover(rec) ? .3 : .1)));
		fnt.Draw(rec.Center(), txt, ZL_Origin::Center);
		return (ZL_Input::Down(rec) != 0);
	}
} Main;

// -------------------------------------------------- Sound data for Song --------------------------------------------------
static const unsigned int IMCSONG_OrderTable[] = {
	0x011111103, 0x022211104, 0x011111103, 0x022211104, 0x011111253, 0x022211264, 0x011111253, 0x022211264,
	0x011111213, 0x022211224, 0x011111233, 0x022211244, 0x011111213, 0x022211224, 0x011111233, 0x022211244,
	0x011111253, 0x022211264, 0x011111253, 0x022211264, 0x033311275, 0x044411286, 0x011111291, 0x0222112A2,
	0x033311275, 0x044411286, 0x011111291, 0x0222112A2, 0x011111103, 0x022211104, 0x011111103, 0x022211104,
	0x011111103, 0x022211104, 0x011111103, 0x022211104,
};
static const unsigned char IMCSONG_PatternData[] = {
	0x30, 0, 0x30, 0x30, 0x30, 0, 0x30, 0x30, 0x28, 0, 0x28, 0x28, 0x28, 0, 0x28, 0x28,
	0x33, 0, 0x33, 0x33, 0x33, 0, 0x33, 0x33, 0x2A, 0, 0x2A, 0x2A, 0x2A, 0, 0x2A, 0x2A,
	0x30, 0, 0x40, 0, 0x30, 0, 0x40, 0, 0x28, 0, 0x38, 0, 0x28, 0, 0x38, 0,
	0x33, 0, 0x43, 0, 0x33, 0, 0x43, 0, 0x2A, 0, 0x3A, 0, 0x2A, 0, 0x3A, 0,
	0x33, 0, 0x43, 0, 0x33, 0, 0x43, 0, 0x2A, 0, 0x3A, 0, 0x2A, 0, 0x3A, 0,
	0x38, 0, 0x48, 0, 0x38, 0, 0x48, 0, 0x30, 0, 0x40, 0, 0x3A, 0, 0x4A, 0,
	0x60, 0, 0, 0, 0x57, 0, 0, 0x58, 0, 0, 0x53, 0, 0x50, 0x52, 0x53, 0,
	0, 0, 0, 0, 0x53, 0x55, 0x57, 0, 0x57, 0, 0, 0x55, 0, 0, 0x50, 0x52,
	0x53, 0, 0, 0, 0, 0, 0x52, 0, 0x53, 0, 0x55, 0x57, 0, 0, 0x58, 0,
	0x5A, 0, 0, 0x53, 0, 0, 0x4A, 0, 0x57, 0, 0, 0, 0x55, 0, 0, 0,
	0x60, 255, 0x57, 255, 0x50, 255, 0x47, 255, 0x63, 255, 0x58, 255, 0x50, 255, 0x48, 255,
	0x63, 255, 0x5A, 255, 0x57, 255, 0x53, 255, 0x4A, 255, 0x52, 255, 0x55, 255, 0x5A, 255,
	0x63, 0, 0, 0, 0x5A, 0, 0, 0, 0x57, 0, 0x55, 0x55, 0, 0, 0x50, 0x52,
	0x53, 0, 0x55, 0, 0x57, 0, 0x58, 0, 0x57, 0, 0x55, 0x55, 0, 0, 0x50, 0x52,
	0x53, 0, 0, 0, 0, 0, 0x52, 0, 0x53, 0, 0x55, 0x57, 0, 0, 0x57, 0x58,
	0x5A, 0, 0x60, 0, 0x62, 0, 0x63, 0, 0x5A, 0, 0, 0, 0, 0, 0, 0,
	0x50, 0, 0, 0, 0x50, 0, 0, 0, 0x50, 0, 0, 0, 0x50, 0, 0x50, 0,
	0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50,
	0, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0, 0, 0x40, 0, 0, 0,
	0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50,
	0x50, 255, 0x50, 255, 0, 0x50, 255, 0, 0x48, 255, 0, 0x53, 255, 0, 0x48, 255,
	0x43, 255, 0x43, 255, 0, 0x43, 255, 0, 0x4A, 255, 0, 0x4A, 255, 0, 0x4A, 255,
	0x43, 255, 0x43, 255, 0, 0x43, 255, 0, 0x3A, 255, 0, 0x3A, 255, 0, 0x3A, 255,
	0x48, 255, 0x48, 255, 0, 0x48, 255, 0, 0x40, 255, 0, 0x40, 255, 0, 0x4A, 255,
	0x53, 255, 0x53, 255, 0, 0x53, 255, 0, 0x53, 255, 0, 0x58, 255, 0, 0x53, 255,
	0x57, 255, 0x57, 255, 0, 0x57, 255, 0, 0x52, 255, 0, 0x52, 255, 0, 0x52, 255,
	0x57, 255, 0x57, 255, 0, 0x57, 255, 0, 0x55, 255, 0, 0x55, 255, 0, 0x55, 255,
	0x58, 255, 0x58, 255, 0, 0x58, 255, 0, 0x57, 255, 0, 0x55, 255, 0, 0x55, 255,
	0x40, 0x47, 0x50, 0x60, 0x50, 0x40, 0x47, 0x50, 0x40, 0x48, 0x53, 0x63, 0x53, 0x48, 0x40, 0x48,
	0x43, 0x4A, 0x53, 0x63, 0x57, 0x53, 0x43, 0x47, 0x42, 0x4A, 0x52, 0x62, 0x5A, 0x55, 0x52, 0x4A,
	0x43, 0x53, 0x5A, 0x63, 0x43, 0x53, 0x5A, 0x63, 0x42, 0x52, 0x55, 0x62, 0x42, 0x52, 0x55, 0x62,
	0x40, 0x50, 0x58, 0x60, 0x40, 0x50, 0x58, 0x60, 0x40, 0x50, 0x57, 0x60, 0x42, 0x52, 0x5A, 0x62,
};
static const unsigned char IMCSONG_PatternLookupTable[] = { 0, 6, 16, 18, 19, 20, 24, 28, };
static const TImcSongEnvelope IMCSONG_EnvList[] = {
	{ 0, 256, 434, 8, 16, 0, true, 255, }, { 0, 256, 130, 8, 16, 255, true, 255, }, { 0, 256, 64, 6, 18, 255, true, 255, },
	{ 0, 256, 152, 8, 16, 255, true, 255, }, { 0, 256, 9, 8, 255, 255, false, 3, }, { 0, 256, 523, 1, 23, 255, true, 255, },
	{ 128, 256, 174, 8, 16, 16, true, 255, }, { 0, 256, 871, 8, 16, 16, true, 255, }, { 0, 256, 523, 8, 16, 255, true, 255, },
	{ 0, 256, 64, 8, 16, 255, true, 255, }, { 0, 256, 228, 8, 16, 255, true, 255, }, { 0, 256, 136, 8, 16, 255, true, 255, },
	{ 128, 512, 2179, 0, 255, 255, true, 255, }, { 0, 256, 871, 24, 16, 255, true, 255, }, { 0, 256, 379, 8, 16, 255, true, 255, },
	{ 32, 256, 196, 8, 16, 255, true, 255, }, { 0, 256, 1089, 8, 255, 255, true, 255, }, { 0, 256, 182, 8, 16, 13, true, 255, },
	{ 0, 256, 1089, 8, 16, 16, true, 255, }, { 0, 256, 726, 8, 255, 255, true, 255, }, { 0, 256, 544, 8, 255, 255, true, 255, },
	{ 0, 256, 1089, 0, 255, 255, true, 255, }, { 0, 256, 209, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSONG_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 256 }, { 2, 1, 248 }, { 3, 1, 256 }, { 4, 1, 256 }, { 5, 2, 158 }, { 6, 2, 256 },
	{ 7, 2, 256 }, { 8, 2, 256 }, { 9, 3, 256 }, { 10, 3, 256 }, { 11, 3, 256 }, { 12, 3, 320 }, { 13, 3, 0 }, { 14, 4, 256 },
	{ 15, 4, 256 }, { 16, 5, 256 }, { 17, 5, 256 }, { 18, 5, 256 }, { 19, 5, 256 }, { 20, 5, 256 }, { 21, 6, 128 }, { 17, 6, 256 },
	{ 18, 6, 256 }, { 18, 6, 256 }, { 19, 6, 256 }, { 20, 6, 256 }, { 22, 7, 256 }, { 17, 7, 256 }, { 18, 7, 256 }, { 18, 7, 256 },
	{ 19, 7, 256 }, { 20, 7, 256 },
};
static const TImcSongOscillator IMCSONG_OscillatorList[] = {
	{ 8, 2, IMCSONGOSCTYPE_SAW, 0, -1, 122, 1, 1 }, { 7, 0, IMCSONGOSCTYPE_SAW, 0, -1, 255, 1, 1 }, { 6, 0, IMCSONGOSCTYPE_SINE, 0, -1, 68, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SQUARE, 0, 0, 26, 1, 1 }, { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 1, 1 }, { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 66, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 24, 1, 1 }, { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 88, 4, 1 }, { 8, 0, IMCSONGOSCTYPE_SINE, 1, 5, 36, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_NOISE, 1, 7, 48, 1, 1 }, { 5, 15, IMCSONGOSCTYPE_SINE, 2, -1, 72, 1, 7 }, { 8, 0, IMCSONGOSCTYPE_NOISE, 2, -1, 204, 8, 1 },
	{ 5, 227, IMCSONGOSCTYPE_SINE, 2, -1, 126, 9, 1 }, { 7, 93, IMCSONGOSCTYPE_SINE, 3, -1, 255, 11, 1 }, { 9, 162, IMCSONGOSCTYPE_SAW, 3, -1, 180, 12, 1 },
	{ 8, 0, IMCSONGOSCTYPE_NOISE, 3, -1, 108, 14, 1 }, { 8, 0, IMCSONGOSCTYPE_SAW, 3, 14, 196, 1, 13 }, { 8, 0, IMCSONGOSCTYPE_NOISE, 4, -1, 127, 1, 16 },
	{ 9, 0, IMCSONGOSCTYPE_SQUARE, 5, -1, 255, 18, 1 }, { 8, 127, IMCSONGOSCTYPE_SQUARE, 5, -1, 255, 1, 1 }, { 8, 0, IMCSONGOSCTYPE_SQUARE, 5, -1, 255, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SQUARE, 5, -1, 242, 19, 20 }, { 8, 0, IMCSONGOSCTYPE_SQUARE, 5, 21, 255, 21, 1 }, { 9, 2, IMCSONGOSCTYPE_SQUARE, 6, -1, 255, 23, 1 },
	{ 8, 2, IMCSONGOSCTYPE_SQUARE, 6, -1, 255, 1, 1 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 6, -1, 255, 1, 1 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 6, -1, 228, 24, 1 },
	{ 8, 2, IMCSONGOSCTYPE_SQUARE, 6, -1, 142, 25, 26 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 6, 26, 255, 1, 1 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 6, 27, 255, 27, 1 },
	{ 9, 2, IMCSONGOSCTYPE_SQUARE, 7, -1, 255, 29, 1 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 7, -1, 255, 1, 1 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 7, -1, 255, 1, 1 },
	{ 8, 2, IMCSONGOSCTYPE_SQUARE, 7, -1, 228, 30, 1 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 7, -1, 142, 31, 32 }, { 8, 2, IMCSONGOSCTYPE_SQUARE, 7, 33, 255, 1, 1 },
	{ 8, 2, IMCSONGOSCTYPE_SQUARE, 7, 34, 255, 33, 1 },
};
static const TImcSongEffect IMCSONG_EffectList[] = {
	{ 2921, 1671, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 }, { 236, 156, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 2, 1 }, { 218, 247, 1, 1, IMCSONGEFFECTTYPE_RESONANCE, 1, 5 },
	{ 124, 0, 1, 1, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 }, { 71, 0, 16536, 1, IMCSONGEFFECTTYPE_DELAY, 0, 0 }, { 2286, 2666, 1, 1, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
	{ 2286, 3669, 1, 2, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 }, { 76, 0, 1, 2, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 }, { 241, 175, 1, 3, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
	{ 159, 0, 1, 3, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 }, { 255, 110, 1, 4, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 }, { 227, 0, 1, 4, IMCSONGEFFECTTYPE_HIGHPASS, 1, 0 },
	{ 87, 0, 1, 5, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 }, { 243, 187, 1, 5, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 }, { 5842, 656, 1, 5, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
	{ 87, 0, 1, 6, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 }, { 243, 187, 1, 6, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 }, { 5842, 656, 1, 6, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
	{ 87, 0, 1, 7, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 }, { 243, 187, 1, 7, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 }, { 5842, 656, 1, 7, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
	{ 128, 0, 16536, 7, IMCSONGEFFECTTYPE_DELAY, 0, 0 },
};
static unsigned char IMCSONG_ChannelVol[8] = { 171, 255, 255, 255, 255, 99, 102, 48 };
static const unsigned char IMCSONG_ChannelEnvCounter[8] = { 0, 3, 6, 10, 15, 17, 22, 28 };
static const bool IMCSONG_ChannelStopNote[8] = { false, true, true, true, true, true, true, true };
TImcSongData imcSongData = {
	/*LEN*/ 0x24, /*ROWLENSAMPLES*/ 5512, /*ENVLISTSIZE*/ 23, /*ENVCOUNTERLISTSIZE*/ 34, /*OSCLISTSIZE*/ 37, /*EFFECTLISTSIZE*/ 22, /*VOL*/ 28,
	IMCSONG_OrderTable, IMCSONG_PatternData, IMCSONG_PatternLookupTable, IMCSONG_EnvList, IMCSONG_EnvCounterList, IMCSONG_OscillatorList, IMCSONG_EffectList,
	IMCSONG_ChannelVol, IMCSONG_ChannelEnvCounter, IMCSONG_ChannelStopNote };

// -------------------------------------------------- Sound data for SFX --------------------------------------------------
static const TImcSongEnvelope SFX_EnvList[] = {
	{ 0, 256, 64, 8, 16, 255, true, 255, }, { 0, 256, 64, 8, 16, 255, true, 255, }, { 200, 256, 64, 8, 16, 255, true, 255, },
	{ 0, 256, 87, 8, 16, 255, true, 255, }, { 0, 256, 348, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter SFX_EnvCounterList[] = { { 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 256 }, { 2, 0, 256 }, { 3, 0, 256 }, { 4, 0, 256 }, };
static const TImcSongOscillator SFX_OscillatorList[] = {
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 158, 1, 1 }, { 5, 15, IMCSONGOSCTYPE_SINE, 0, -1, 218, 2, 3 },
	{ 7, 0, IMCSONGOSCTYPE_SINE, 0, -1, 120, 4, 5 }, { 10, 0, IMCSONGOSCTYPE_SINE, 0, 0, 136, 1, 1 },
};
static const TImcSongEffect SFX_EffectList[] = { { 64, 0, 11024, 0, IMCSONGEFFECTTYPE_DELAY, 0, 0 }, };
static unsigned char SFX_ChannelVol[8] = { 255, 176, 100, 100, 100, 100, 100, 100 };
static const unsigned char SFX_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool SFX_ChannelStopNote[8] = { true, false, false, false, false, false, false, false };
TImcSongData imcSFXData = { /*LEN*/ 0, /*ROWLENSAMPLES*/ 0, /*ENVLISTSIZE*/ 5, /*ENVCOUNTERLISTSIZE*/ 6, /*OSCLISTSIZE*/ 4, /*EFFECTLISTSIZE*/ 1, /*VOL*/ 100,
	NULL, NULL, NULL, SFX_EnvList, SFX_EnvCounterList, SFX_OscillatorList, SFX_EffectList, SFX_ChannelVol, SFX_ChannelEnvCounter, SFX_ChannelStopNote };

// -------------------------------------------------- Sound data for BOOM --------------------------------------------------
static const unsigned int BOOM_OrderTable[] = { 0x000000011, };
static const unsigned char BOOM_PatternData[] = { 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
static const unsigned char BOOM_PatternLookupTable[] = { 0, 1, 2, 2, 2, 2, 2, 2, };
static const TImcSongEnvelope BOOM_EnvList[] = {
	{ 0, 386, 65, 8, 16, 255, true, 255, }, { 0, 256, 174, 8, 16, 255, true, 255, }, { 128, 256, 173, 8, 16, 255, true, 255, },
	{ 0, 128, 2615, 8, 16, 255, true, 255, }, { 0, 256, 348, 5, 19, 255, true, 255, }, { 0, 256, 418, 8, 16, 255, true, 255, },
	{ 0, 256, 87, 8, 16, 255, true, 255, }, { 0, 256, 228, 8, 16, 255, true, 255, }, { 0, 256, 1046, 24, 15, 255, true, 255, },
	{ 256, 512, 1046, 8, 16, 255, true, 255, }, { 0, 256, 523, 8, 16, 255, true, 255, }, { 0, 512, 11073, 0, 255, 255, true, 255, },
};
static TImcSongEnvelopeCounter BOOM_EnvCounterList[] = {
	{ 0, 0, 386 }, { 1, 0, 256 }, { 2, 0, 256 }, { 3, 0, 128 }, { -1, -1, 258 }, { 4, 0, 238 }, { -1, -1, 256 }, { 5, 0, 256 },
	{ 6, 1, 256 }, { 7, 1, 256 }, { 8, 1, 0 }, { 9, 1, 512 }, { 10, 1, 256 }, { 11, 1, 256 },
};
static const TImcSongOscillator BOOM_OscillatorList[] = {
	{ 5, 150, IMCSONGOSCTYPE_SINE, 0, -1, 255, 1, 2 }, { 9, 15, IMCSONGOSCTYPE_NOISE, 0, -1, 255, 3, 4 }, { 5, 200, IMCSONGOSCTYPE_SINE, 0, -1, 170, 5, 6 },
	{ 5, 174, IMCSONGOSCTYPE_SINE, 0, -1, 230, 7, 6 }, { 6, 238, IMCSONGOSCTYPE_SINE, 1, -1, 255, 9, 6 }, { 8, 0, IMCSONGOSCTYPE_NOISE, 1, -1, 142, 10, 11 },
	{ 8, 213, IMCSONGOSCTYPE_SAW, 1, -1, 38, 12, 6 }, { 8, 0, IMCSONGOSCTYPE_SAW, 1, 6, 90, 6, 13 }, { 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 }, { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 }, { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 }, { 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect BOOM_EffectList[] = {
	{ 113, 0, 1, 0, IMCSONGEFFECTTYPE_LOWPASS, 6, 0 }, { 220, 168, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 6, 6 },
	{ 241, 175, 1, 1, IMCSONGEFFECTTYPE_RESONANCE, 6, 6 }, { 159, 0, 1, 1, IMCSONGEFFECTTYPE_LOWPASS, 6, 0 },
};
static unsigned char BOOM_ChannelVol[8] = { 230, 128, 100, 100, 100, 100, 100, 100 };
static const unsigned char BOOM_ChannelEnvCounter[8] = { 0, 8, 0, 0, 0, 0, 0, 0 };
static const bool BOOM_ChannelStopNote[8] = { true, true, false, false, false, false, false, false };
TImcSongData imcDataBOOM = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 5512, /*ENVLISTSIZE*/ 12, /*ENVCOUNTERLISTSIZE*/ 14, /*OSCLISTSIZE*/ 14, /*EFFECTLISTSIZE*/ 4, /*VOL*/ 100,
	BOOM_OrderTable, BOOM_PatternData, BOOM_PatternLookupTable, BOOM_EnvList, BOOM_EnvCounterList, BOOM_OscillatorList, BOOM_EffectList,
	BOOM_ChannelVol, BOOM_ChannelEnvCounter, BOOM_ChannelStopNote };
