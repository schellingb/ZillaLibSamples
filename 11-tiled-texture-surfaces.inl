#define SCENE_GAME 1

char tilemap[] = {
0, 0, 0, 0, 5, 1, 6, 0,
1, 6, 0, 0, 2, 0, 2, 0,
0, 3, 1, 1, 4, 0, 2, 0,
0, 0, 0, 0, 0, 0, 2, 0 };

struct sSceneGame : public ZL_Scene
{
	ZL_Surface srfTiles, srfTilesUnclipped;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitGlobal()
	{
		srfTilesUnclipped = ZL_Surface("Data/TILES.png");
		srfTiles = ZL_Surface("Data/TILES.png");
		srfTiles.SetTilesetClipping(4, 4);
	}

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		//draw the unclipped original surface for demonstration
		ZL_Display::DrawRect(199, 99, 201+128, 101+128, ZL_Color::Red, ZL_Color::White);
		srfTilesUnclipped.Draw(200, 100);
		//draw the tilemap of fixed size at fixed position
		for (int i = 0; i < 8*4; i++)
			srfTiles.SetTilesetIndex(tilemap[i]).Draw((i%8)*32.0f+200.0f, (i/8)*32.0f+300.0f);
		//animate the last 4 tiles of the tilemap use global ticks as timer
		srfTiles.SetTilesetIndex(12+((ZLTICKS/100)%4));
		srfTiles.Draw(400, 150);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Tiled Texture Surfaces (Tilesets)", 854, 480);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
