static ZL_Sound sndHitBall, sndBallOnWood;
static ZL_Surface srfBall;
static ZL_ParticleEffect particleSmoke, particleFire;

//A block that can be destroyed, just a rectangle with a draw function
struct sBlock : public ZL_Rectf
{
	sBlock(ZL_Vector pos) : ZL_Rectf(pos.x - 45, pos.y - 10, pos.x + 45, pos.y + 10) { }

	void Draw()
	{
		ZL_Display::DrawRect(*this, ZL_Color::Red, ZL_Color::Orange);
	}
};

//The players panel
struct sPanel : public ZL_Rectf
{
	float width;
	sPanel() : ZL_Rectf(ZLHALFW - 50, 100, ZLHALFW + 50, 120), width(100) { }

	void Draw()
	{
		ZL_Display::DrawRect(*this, ZL_Color::Yellow, ZL_Color::Green);
	}
};

//The ball, the main actor in our game based on a 2d float coordinate
struct sBall : public ZL_Vector
{
	bool glued; //If we are glued to the player panel
	ZL_Vector angle; //Movement angle vector
	float speed, radius; //Settings

	sBall() : glued(true), speed(300), radius(10) { }

	void Draw()
	{
		srfBall.Draw(x, y);
	}

	//Handle bouncing off a rectangle (player or block)
	bool Collide(const ZL_Rectf& r)
	{
		if (x <= r.left-radius || x >= r.right+radius || y <= r.low-radius || y >= r.high+radius) return false;
		while (x > r.left-radius && x < r.right+radius && y > r.low-radius && y < r.high+radius) *this += angle * -speed / 10 * ZLELAPSED;
		float colx = (x > r.right && angle.x < 0 ? x - r.right : (x < r.left && angle.x > 0 ? x - r.left : 0));
		float coly = (y > r.high  && angle.y < 0 ? y - r.high  : (y < r.low  && angle.y > 0 ? y - r.low  : 0));
		if (colx) angle.x *= -1;
		if (coly) angle.y *= -1;
		if (colx && coly) { float f = fabs(colx/coly); angle.x *= f; angle.y /= f; angle.Norm(); }
		if (angle.y < s(.2) && angle.y > s(-.2)) { angle.y = (coly < 0 ? s(-.3) : s(.3)); angle.Norm(); }
		speed = MIN(1000, speed + 10);
		return true;
	}

	//Handle moving with the player panel when glued to
	bool CalculateIsGlued(const sPanel& panel)
	{
		if (y < 0) glued = true;
		if (glued) { x = (panel.left + panel.right)/2; y = (panel.high+radius); }
		return glued;
	}

	//Update position
	void CalculateMovement()
	{
		*this += angle * speed * ZLELAPSED;
	}

	//Handle bouncing off of walls
	bool CalculateHitWall()
	{
		bool hit = false;
		if (x > ZLFROMW(radius)) { x = ZLFROMW(radius); angle.x *= -1; speed = MAX(100, speed - 10); hit = true; }
		else if (x < radius)     { x = radius;          angle.x *= -1; speed = MAX(100, speed - 10); hit = true; }
		if (y > ZLFROMH(radius)) { y = ZLFROMH(radius); angle.y *= -1; speed = MAX(100, speed - 10); hit = true; }
		return hit;
	}
};

//The world handles the main game state
static struct sWorld
{
	list<sBlock> blocks;
	sPanel panel;
	sBall ball;

	//Reset all game parts, fill playfield with blocks
	void Init()
	{
		panel = sPanel();
		ball = sBall();
		blocks.clear();
		for (float x = smod(ZLWIDTH, 100.0f) * 0.5f + 50; x < ZLFROMW(50); x += 100)
			for (float y = 250; y < ZLFROMH(70); y += 30)
				blocks.push_back(sBlock(ZL_Vector(x, y)));
	}

	//Calculate the game state updates every frame
	void Calculate()
	{
		if (ball.CalculateIsGlued(panel) || blocks.empty()) return;
		for (list<sBlock>::iterator itb = blocks.begin(); itb != blocks.end();)
		{
			if (ball.Collide(*itb))
			{
				sndBallOnWood.Play();
				particleSmoke.Spawn(40, ball);
				particleFire.Spawn(50, itb->Center(), 0, itb->Width(), itb->Height());
				itb = blocks.erase(itb);
			}
			else ++itb;
		}
		ball.CalculateMovement();
		if (ball.CalculateHitWall())
		{
			sndHitBall.Play();
			particleSmoke.Spawn(40, ball);
		}
		else if (ball.Collide(panel))
		{
			ball.angle.x += s(-1) + 2 * (ball.x - panel.left) / (panel.right - panel.left);
			ball.angle.Norm();
			if (!ball.angle.y) ball.angle.y = s(.1);
			sndHitBall.Play();
			particleSmoke.Spawn(40, ball);
		}
	}

	//Return if the player has won the game when all blocks are gone
	bool HasWon()
	{
		return blocks.empty() && !particleFire.CountParticles();
	}

	//Draw the game state (blocks, player panel, ball, particles)
	void Draw()
	{
		ZL_Display::SetThickness(3);
		for (list<sBlock>::iterator itb = blocks.begin(); itb != blocks.end(); ++itb)
			itb->Draw();
		panel.Draw();
		ball.Draw();
		particleSmoke.Draw();
		particleFire.Draw();
		ZL_Display::SetThickness(1);
	}

	//Handle input movement
	void MovePanel(float x)
	{
		panel.left = x - panel.width/2;
		panel.right = panel.left + panel.width;
	}

	//Handle firing the ball when glued to the player panel
	void Fire(const ZL_Vector& atpos)
	{
		if (ball.glued)
		{
			ball.angle = (atpos - ball).Norm();
			ball.glued = false;
		}
	}
} World;

enum { SCENE_GAME = 1 };

static struct sSceneDemoGame : public ZL_Scene
{
	sSceneDemoGame() : ZL_Scene(SCENE_GAME) { }

	void InitEnter(ZL_SceneType SceneTypeFrom, void* data)
	{
		sndHitBall = ZL_Sound("Data/HitBall.ogg");
		sndBallOnWood = ZL_Sound("Data/BallOnWood.ogg");
		srfBall = ZL_Surface("Data/ball.png").SetDrawOrigin(ZL_Origin::Center);
		particleSmoke = ZL_ParticleEffect(300, 150);
		particleSmoke.AddParticleImage(ZL_Surface("Data/SPARK.png").SetColor(ZLLUM(.5)), 1000);
		particleSmoke.AddBehavior(new ZL_ParticleBehavior_LinearMove(30, 25));
		particleSmoke.AddBehavior(new ZL_ParticleBehavior_LinearImageProperties(1, 0, s(1.1), s(.5)));
		particleFire = ZL_ParticleEffect(500, 200);
		particleFire.AddParticleImage(ZL_Surface("Data/SPARK.png").SetColor(ZLRGB(1,.8,.1)), 1000);
		particleFire.AddBehavior(new ZL_ParticleBehavior_LinearMove(30, 10));
		particleFire.AddBehavior(new ZL_ParticleBehavior_LinearImageProperties(1, 0, 1, 3));
	}

	void DeInitAfterTransition()
	{
		sndHitBall = ZL_Sound();
		sndBallOnWood = ZL_Sound();
		srfBall = ZL_Surface();
		particleFire = ZL_ParticleEffect();
		particleSmoke = ZL_ParticleEffect();
	}

	void StartGame()
	{
		World.Init();
	}

	void InitAfterTransition()
	{
		ZL_Display::sigPointerDown.connect(this, &sSceneDemoGame::OnMouseDown);
		ZL_Display::sigPointerMove.connect(this, &sSceneDemoGame::OnMouseMove);
		ZL_Display::sigKeyDown.connect(this, &sSceneDemoGame::OnKeyDown);
		StartGame();
	}

	void DeInitLeave(ZL_SceneType SceneTypeTo)
	{
		ZL_Display::AllSigDisconnect(this);
	}

	void OnMouseDown(ZL_PointerPressEvent& e)
	{
		if (e.y < 200) World.MovePanel(e.x);
		else World.Fire(e);
	}

	void OnMouseMove(ZL_PointerMoveEvent& e)
	{
		if (e.state) World.MovePanel(e.x);
	}

	void OnKeyDown(ZL_KeyboardEvent& e)
	{
		if (e.key == ZLK_ESCAPE) ZL_Application::Quit();
	}

	void Calculate()
	{
		World.Calculate();
		if (World.HasWon()) World.Init();
	}

	void Draw()
	{
		ZL_Display::FillGradient(0, 120, ZLWIDTH, ZLHEIGHT, ZLRGB(0,0,.3), ZLRGB(0,0,.3), ZLRGB(.4,.4,.4), ZLRGB(.4,.4,.4));
		ZL_Display::FillGradient(0, 80, ZLWIDTH, 140, ZLRGB(0,0,0), ZLRGB(0,0,0), ZLRGB(.1,.1,.1), ZLRGB(.1,.1,.1));
		ZL_Display::FillGradient(0, 0, ZLWIDTH, 80, ZLRGB(0,.2,.4), ZLRGB(0,.2,.4), ZLRGB(0,0,.3), ZLRGB(0,0,.3));
		World.Draw();
	}
} SceneGame;

struct sSimpleGame : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Simple Game", 854, 480);
		ZL_Audio::Init();
		ZL_SceneManager::Init(SCENE_GAME);
	}
} SimpleGame;
