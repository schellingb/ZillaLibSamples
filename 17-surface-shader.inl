#define SCENE_GAME 1

struct sSceneGame : public ZL_Scene
{
	ZL_Shader shader_both, shader_fragment_only, shader_vertex_only;
	ZL_Surface srfLogo;

	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitGlobal()
	{
		const char shader_vertex_shader_src[] = ZL_SHADER_SOURCE_HEADER(ZL_GLES_PRECISION_LOW)
			"uniform mat4 u_mvpMatrix;"
			"attribute vec4 a_position;"
			"attribute vec4 a_color;"
			"attribute vec2 a_texcoord;"
			"varying vec4 v_color;"
			"varying vec2 v_texcoord;"
			"uniform float shear;"
			"void main()"
			"{"
				"v_color = a_color;"
				"v_texcoord = a_texcoord;"
				"gl_Position = u_mvpMatrix * (a_position + vec4(a_texcoord.y*shear,0.0,0.0,0.0));"
			"}";
		const char shader_fragment_shader_src[] = ZL_SHADER_SOURCE_HEADER(ZL_GLES_PRECISION_LOW)
			"uniform sampler2D u_texture;"
			"varying vec4 v_color;"
			"varying vec2 v_texcoord;"
			"uniform float brightness;"
			"void main()"
			"{"
				"gl_FragColor = texture2D(u_texture, v_texcoord);"
				"gl_FragColor.rgb = brightness*vec3((gl_FragColor.r+gl_FragColor.g+gl_FragColor.b)/3.0);"
			"}";
		shader_both          = ZL_Shader(shader_fragment_shader_src, shader_vertex_shader_src, "shear", "brightness");
		shader_fragment_only = ZL_Shader(shader_fragment_shader_src, NULL,                     "brightness");
		shader_vertex_only   = ZL_Shader(NULL,                       shader_vertex_shader_src, "shear");
		srfLogo = ZL_Surface("Data/ZILLALIB.png");
	}

	void Draw()
	{
		float t = ZLSINCESECONDS(0);
		ZL_Display::ClearFill(ZL_Color::Black);

		shader_both.Activate();
		shader_both.SetUniform(50.0f+50.0f*scos(t), 0.5f+0.5f*ssin(t));
		srfLogo.Draw( 50, 250);
		shader_both.Deactivate();

		shader_fragment_only.Activate();
		shader_fragment_only.SetUniform(0.5f+0.5f*ssin(t));
		srfLogo.Draw(450, 250);
		shader_fragment_only.Deactivate();

		shader_vertex_only.Activate();
		shader_vertex_only.SetUniform(50.0f+50.0f*scos(t));
		srfLogo.Draw(850, 250);
		shader_vertex_only.Deactivate();
	}
} SceneGame;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Surface Shader", 1280, 720);
		ZL_SceneManager::Init(SCENE_GAME);
	}
} Main;
