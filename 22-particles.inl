enum { SCENE_PARTICLES = 1 };

struct sSceneParticles : public ZL_Scene
{
	//Construct the scene with its identifier
	sSceneParticles() : ZL_Scene(SCENE_PARTICLES) { }
	ZL_ParticleEffect sparks;

	//Set up the particle effect when entering the scene
	void InitEnter(ZL_SceneType SceneTypeFrom, void* data)
	{
		sparks = ZL_ParticleEffect(3000, 500, s(0.1));
		sparks.AddParticleImage(ZL_Surface("Data/SPARK.png").SetColor(ZLRGB(1,.8,.1)), 10000); //max 10000 particles
		sparks.AddBehavior(new ZL_ParticleBehavior_LinearMove(300, 50)); //move at a speed of 300 pixel per seconds with random variation 50 (250 to 350)
		sparks.AddBehavior(new ZL_ParticleBehavior_LinearImageProperties(1, 0, 1, 3)); //fade from 1 to 0, scale from 1 to 3
	}

	//Clean up after scene is done (not relevant to this sample)
	void DeInitAfterTransition()
	{
		sparks = ZL_ParticleEffect();
	}

	//Spawn some particles once a frame (before doing any drawing)
	void Calculate()
	{
		sparks.Spawn(ZLELAPSEDTICKS, ZL_Display::PointerX, ZL_Display::PointerY);
	}

	//clear the screen and draw the particles
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		sparks.Draw();
	}
} SceneParticles;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Particles", 854, 480);
		ZL_SceneManager::Init(SCENE_PARTICLES);
	}
} Main;
