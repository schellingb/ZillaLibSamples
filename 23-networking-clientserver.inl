enum { SCENE_MAIN = 1 };
static ZL_Font fnt;

struct sSceneMain : public ZL_Scene
{
	//Construct the scene with its identifier
	sSceneMain() : ZL_Scene(SCENE_MAIN) { }

#ifdef ZL_NO_SOCKETS
	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);
		fnt.Draw(20.0f, 240.0f,
			"On platforms with no direct udp socket access (web) the macro ZL_NO_SOCKETS is set." "\n"
			"On there we can't use the client/server model. Async HTTP connections are available everywhere");
	}
#else

	ZL_Server server;
	ZL_Client client;

	//Events listening to the networking events from server and client
	void OnServerConnect(ZL_Peer &peer)            { Message(ZL_String::format("Server: Connect From: %x", peer.host)); }
	void OnServerDisconnect(ZL_Peer &peer)         { Message(ZL_String::format("Server: Disconnected Client: %x", peer.host)); }
	void OnServerReceive(ZL_Peer &p, ZL_Packet &d) { Message(ZL_String::format("Server: Got: [%.*s] From: %x", d.length, d.data, p.host)); }
	void OnClientDisconnect()                      { Message("Client: Disconnected"); }

	//As soon as the client is connected to the server, send a hello mesasge
	void OnClientConnect()
	{
		const char* pcHelloMsg = "Hello From ZL_Network";
		Message(ZL_String("Client: Connected - Sending Data [") << pcHelloMsg << "]");
		client.Send((void*)pcHelloMsg, strlen(pcHelloMsg));
	}

	//Cleanup the networking objects when leaving the scene (although not relevant to this sample)
	void DeInitAfterTransition()
	{
		server = ZL_Server();
		client = ZL_Client();
	}

	void Draw()
	{
		ZL_Display::ClearFill(ZL_Color::Black);

		//Draw the message buffer
		ZL_Display::DrawRect(40.0f, ZLFROMH(75.0f + 13*20), ZLFROMW(40.0f), ZLFROMH(60.0f), ZL_Color::White, ZLALPHA(.5));
		for (vector<ZL_String>::iterator it = msgs.begin(); it != msgs.end(); ++it)
			fnt.Draw(50.0f, ZLFROMH(87.0f + (it - msgs.begin())*20), *it);

		if (Button(ZL_Rectf::BySize(80.0f, 40.0f, 300.0f, 60.0f), "Start Server"))
		{
			if (server.IsOpened())
			{
				server.Close();
				Message("Server: Stopped");
			}
			else
			{
				server = ZL_Server(5234, 4);
				server.sigConnected().connect(this, &sSceneMain::OnServerConnect);
				server.sigReceived().connect(this, &sSceneMain::OnServerReceive);
				server.sigDisconnected().connect(this, &sSceneMain::OnServerDisconnect);
				Message("Server: Started");
			}
		}

		if (Button(ZL_Rectf::BySize(474.0f, 40.0f, 300.0f, 60.0f), "Connect Client"))
		{
			if (client.IsConnected())
			{
				Message("Client: Disconnecting...");
				client.Disconnect();
			}
			else
			{
				Message("Client: Connecting to server...");
				client = ZL_Client("localhost", 5234);
				client.sigConnected().connect(this, &sSceneMain::OnClientConnect);
				client.sigDisconnected().connect(this, &sSceneMain::OnClientDisconnect);
			}
		}
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
#endif
} SceneMain;

struct sMain : public ZL_Application
{
	void Load(int argc, char *argv[])
	{
		ZL_Display::Init("Networking with Client/Server", 854, 480);
		ZL_Network::Init();
		ZL_Input::Init();
		fnt = ZL_Font("Data/fntMain.png");
		ZL_SceneManager::Init(SCENE_MAIN);
	}
} Main;
