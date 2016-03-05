#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	ZL_PostProcess postproc;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitEnter(ZL_SceneType SceneTypeFrom, void* data)
	{
		const char postproc_fragment_shader_src[] = ZL_SHADER_SOURCE_HEADER(ZL_GLES_PRECISION_HIGH)
			"uniform sampler2D u_texture;"
			"uniform float randx, randy;"
			"varying vec2 v_texcoord;"
			"float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }"
			"void main()"
			"{"
				"if (v_texcoord.y > 0.5) { gl_FragColor = texture2D(u_texture, v_texcoord); return; }"
				"vec2 v_texcoordmirror = vec2(v_texcoord.x, 1.0-v_texcoord.y);"
				"v_texcoordmirror.x += (rand(v_texcoord+vec2(randx)) - 0.5) * 0.02;"
				"v_texcoordmirror.y += (rand(v_texcoord+vec2(randy)) - 0.5) * 0.02;"
				"gl_FragColor = texture2D(u_texture, v_texcoordmirror);"
				"gl_FragColor.rgb *= v_texcoord.y;"
				"gl_FragColor.b += 0.3;"
			"}";
		postproc = ZL_PostProcess(postproc_fragment_shader_src, false, "randx", "randy");
	}

	void Draw()
	{
		postproc.Start();
		ZL_Display::ClearFill(ZL_Color::Black);
		ZL_Display::FillRect(50.0f, 200.0f, ZLFROMW(50.0f), ZLFROMH(50.0f), ZLRGB(.1,.5,.1));
		ZL_Display::FillRect(80.0f, 200.0f, ZLFROMW(80.0f), ZLFROMH(80.0f), ZLRGB(.1,.3,.1) );
		ZL_Display::FillCircle(ZLHALFW + ssin(ZLSINCESECONDS(0)) * 400.0f, 350 + scos(ZLSINCESECONDS(0)) * 50.0f, 10, ZL_Color::Yellow);
		postproc.Apply(ssin(ZLSINCESECONDS(0))*0.1f, scos(ZLSINCESECONDS(0))*0.1f);
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Post Process Effect", 854, 480);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
