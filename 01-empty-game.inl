struct sEmptyGame : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Empty Game", 854, 480);
	}
} EmptyGame;
