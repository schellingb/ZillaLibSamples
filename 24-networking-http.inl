enum { SCENE_MAIN = 1 };
static ZL_Font fnt;

struct sSceneMain : public ZL_Scene
{
	//Construct the scene with its identifier
	sSceneMain() : ZL_Scene(SCENE_MAIN) { }

	ZL_HttpConnection http;
	ZL_Surface srfReceivedImage;

	//When receiving HTTP response, print header and content
	void OnHttpResponseString(int http_status, const ZL_String& html)
	{
		if (!IsActive()) return; //ignore if scene was switched
		Message(ZL_String("HTTP HEADER: ") << "Status: " << http_status << " - Length: " << html.size());
		if (http_status != 200 || !html.length()) return;
		vector<ZL_String> lines = html.split("\n");
		for (vector<ZL_String>::iterator it = lines.begin(); it != lines.end(); ++it) Message(ZL_String("HTTP: ") << *it);
	}

	//When receiving image data, load it into a surface (which is displayed in Draw())
	void OnHttpResponsePng(int http_status, const char* data, size_t size)
	{
		if (!IsActive()) return; //ignore if scene was switched
		Message(ZL_String("HTTP HEADER: ") << "Status: " << http_status << " - Length: " << size);
		if (http_status != 200 || !size || !data) return;
		srfReceivedImage = ZL_Surface(ZL_File(data, size)).SetOrigin(ZL_Origin::Center);
	}

	//Cleanup the networking connection object when leaving the scene (although not relevant to this sample)
	void DeInitAfterTransition()
	{
		http = ZL_HttpConnection();
	}

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);

		//Draw the message buffer
		ZL_Display::DrawRect(40.0f, ZLFROMH(75.0f + 13*20), ZLFROMW(40.0f), ZLFROMH(60.0f), ZL_Color::White, ZLALPHA(.5));
		for (vector<ZL_String>::iterator it = msgs.begin(); it != msgs.end(); ++it)
			fnt.Draw(50.0f, ZLFROMH(87.0f + (it - msgs.begin())*20), *it);

		if (Button(ZL_Rectf::BySize(80.0f, 40.0f, 300.0f, 60.0f), "Make regular HTTP request"))
		{
			//HTTP request txt
			Message("Requesting text data...");
			http = ZL_HttpConnection("http://zillalib.github.io/TEST.TXT");
			http.sigReceivedString().connect(this, &sSceneMain::OnHttpResponseString);
			http.Connect();
		}

		if (Button(ZL_Rectf::BySize(474.0f, 40.0f, 300.0f, 60.0f), "Request PNG image over HTTP"))
		{
			//HTTP request png
			Message("Requesting png data...");
			http = ZL_HttpConnection("http://zillalib.github.io/TEST.PNG");
			http.sigReceivedData().connect(this, &sSceneMain::OnHttpResponsePng);
			http.Connect();
		}

		srfReceivedImage.Draw(427.0f, 70.0f);
	}

	//extremely simple UI, draw a rectangle with text in it and return if it has been clicked
	bool Button(const ZL_Rectf& rec, const char* txt)
	{
		ZL_Display::DrawRect(rec, ZLALPHA(.8), ZLALPHA(ZL_Input::Held(rec) ? .6 : (ZL_Input::Hover(rec) ? .3 : .1)));
		fnt.Draw(rec.Center(), txt, ZL_Origin::Center);
		return (ZL_Input::Down(rec) != 0);
	}

	//Simple message buffer
	vector<ZL_String> msgs;
	void Message(const ZL_String& s) { msgs.push_back(s); if (msgs.size() > 13) msgs.erase(msgs.begin()); }
} SceneMain;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Networking HTTP Requests", 854, 480);
		ZL_Network::Init();
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_MAIN);
	}
} Main;
