/*
  The samples in this directory tree are for demonstrating and testing
  the functionality of ZillaLib, and are placed in the public domain.
*/

#include <ZL_Application.h>
#include <ZL_Display.h>
#include <ZL_Surface.h>
#include <ZL_Signal.h>
#include <ZL_Audio.h>
#include <ZL_Font.h>
#include <ZL_Scene.h>
#include <ZL_Input.h>
#include <ZL_Timer.h>
#include <ZL_Particles.h>
#include <ZL_Math.h>
#include <ZL_Data.h>
#include <ZL_Network.h>
#include <ZL_SynthImc.h>

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#ifndef ZILLALIBSAMPLES_NUMBER
#define ZILLALIBSAMPLES_NUMBER 21
#endif

#if defined(NDEBUG) && !defined(__SMARTPHONE__) && !defined(__WEBAPP__) && ZILLALIBSAMPLES_HASDATA
//Override ZL_Display::Init to automatically call ZL_Application::LoadReleaseDesktopDataBundle in release builds
struct ZL_Display_Sample : public ZL_Display
{
	static inline bool Init(const char* title, int width = 640, int height = 480, int displayflags = ZL_DISPLAY_DEFAULT)
	{
		if (!ZL_Application::LoadReleaseDesktopDataBundle()) { exit(0); return false; }
		return ZL_Display::Init(title, width, height, displayflags);
	}
};
#define ZL_Display ZL_Display_Sample
#endif

#if   ZILLALIBSAMPLES_NUMBER ==  1
#include "01-empty-game.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  2
#include "02-scene-manager-with-a-single-scene.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  3
#include "03-scene-manager-with-two-scenes.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  4
#include "04-scene-manager-with-crossfade.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  5
#include "05-2d-geometry-drawing.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  6
#include "06-input-and-other-events.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  7
#include "07-surface-loading-and-drawing.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  8
#include "08-rotating-and-scaling-surfaces.inl"
#elif ZILLALIBSAMPLES_NUMBER ==  9
#include "09-surface-batch-rendering.inl"
#elif ZILLALIBSAMPLES_NUMBER == 10
#include "10-surface-with-repeating-texture.inl"
#elif ZILLALIBSAMPLES_NUMBER == 11
#include "11-tiled-texture-surfaces.inl"
#elif ZILLALIBSAMPLES_NUMBER == 12
#include "12-font-rendering.inl"
#elif ZILLALIBSAMPLES_NUMBER == 13
#include "13-easing.inl"
#elif ZILLALIBSAMPLES_NUMBER == 14
#include "14-timer.inl"
#elif ZILLALIBSAMPLES_NUMBER == 15
#include "15-collision-tests.inl"
#elif ZILLALIBSAMPLES_NUMBER == 16
#include "16-post-process-effect.inl"
#elif ZILLALIBSAMPLES_NUMBER == 17
#include "17-surface-shader.inl"
#elif ZILLALIBSAMPLES_NUMBER == 18
#include "18-render-clipping.inl"
#elif ZILLALIBSAMPLES_NUMBER == 19
#include "19-render-to-texture.inl"
#elif ZILLALIBSAMPLES_NUMBER == 20
#include "20-sound-samples.inl"
#elif ZILLALIBSAMPLES_NUMBER == 21
#include "21-ImcSynthesizer-Sound.inl"
#elif ZILLALIBSAMPLES_NUMBER == 22
#include "22-particles.inl"
#elif ZILLALIBSAMPLES_NUMBER == 23
#include "23-networking-clientserver.inl"
#elif ZILLALIBSAMPLES_NUMBER == 24
#include "24-networking-http.inl"
#elif ZILLALIBSAMPLES_NUMBER == 25
#include "25-saving-loading-settings.inl"
#elif ZILLALIBSAMPLES_NUMBER == 26
#include "26-open-web-link.inl"
#elif ZILLALIBSAMPLES_NUMBER == 27
#include "27-json-read-write.inl"
#elif ZILLALIBSAMPLES_NUMBER == 28
#include "28-advanced-polygon.inl"
#elif ZILLALIBSAMPLES_NUMBER == 29
#include "29-blend-modes.inl"
#elif ZILLALIBSAMPLES_NUMBER == 30
#include "30-simple-game.inl"
#endif

//Test compile all samples with ZILLALIBSAMPLES_NUMBER set to 0
#if ZILLALIBSAMPLES_NUMBER == 0
namespace NS01 {
#include "01-empty-game.inl"
};namespace NS02 {
#include "02-scene-manager-with-a-single-scene.inl"
};namespace NS03 {
#include "03-scene-manager-with-two-scenes.inl"
#undef SCENE_TITLE
#undef SCENE_GAME
};namespace NS04 {
#include "04-scene-manager-with-crossfade.inl"
#undef SCENE_GAME
#undef SCENE_MENU
};namespace NS05 {
#include "05-2d-geometry-drawing.inl"
#undef SCENE_GAME
};namespace NS06 {
#include "06-input-and-other-events.inl"
#undef SCENE_GAME
};namespace NS07 {
#include "07-surface-loading-and-drawing.inl"
#undef SCENE_GAME
};namespace NS08 {
#include "08-rotating-and-scaling-surfaces.inl"
#undef SCENE_GAME
};namespace NS09 {
#include "09-surface-batch-rendering.inl"
#undef SCENE_GAME
};namespace NS10 {
#include "10-surface-with-repeating-texture.inl"
#undef SCENE_GAME
};namespace NS11 {
#include "11-tiled-texture-surfaces.inl"
#undef SCENE_GAME
};namespace NS12 {
#include "12-font-rendering.inl"
#undef SCENE_GAME
};namespace NS13 {
#include "13-easing.inl"
};namespace NS14 {
#include "14-timer.inl"
#undef SCENE_MAIN
};namespace NS15 {
#include "15-collision-tests.inl"
#undef SCENE_GAME
};namespace NS16 {
#include "16-post-process-effect.inl"
#undef SCENE_GAME
};namespace NS17 {
#include "17-surface-shader.inl"
};namespace NS18 {
#include "18-render-clipping.inl"
};namespace NS19 {
#include "19-render-to-texture.inl"
};namespace NS20 {
#include "20-sound-samples.inl"
};namespace NS21 {
#include "21-ImcSynthesizer-Sound.inl"
};namespace NS22 {
#include "22-particles.inl"
};namespace NS23 {
#include "23-networking-clientserver.inl"
};namespace NS24 {
#include "24-networking-http.inl"
};namespace NS25 {
#include "25-saving-loading-settings.inl"
};namespace NS26 {
#include "26-open-web-link.inl"
};namespace NS27 {
#include "27-json-read-write.inl"
};namespace NS28 {
#include "28-advanced-polygon.inl"
};namespace NS29 {
#include "29-blend-modes.inl"
};

#endif
