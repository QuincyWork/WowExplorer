#include "mywow.h"
#include "CMiniDump.h"
#include "CMemDbg.h"
#include "game.h"

#pragma comment(lib, "mywow.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

int main()
{
	CMiniDump::begin();

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//_CrtSetBreakAlloc(123909);

	CMemDbg globalDbg;
	globalDbg.beginCheckPoint();

	SWindowInfo wndInfo = Engine::createWindow("App10", dimension2du(800,600));
	HWND hwnd = wndInfo.hwnd;

	SEngineInitParam param;
	param.ignoreSetting = true;
	param.useCompress = false;
	//param.outputFileName = true;
	createEngine(param, wndInfo);

	bool success = g_Engine->initDriver(EDT_OPENGL, 0, false, true, 1, true);
	g_bExit = !success;

	MyMessageHandler handler;
	g_Engine->setMessageHandler(&handler);

	//g_Engine->getWowDatabase()->buildItems();
	//g_Engine->getWowDatabase()->buildNpcs("npcs.csv");
	//g_Engine->getWowDatabase()->buildMaps();

	g_Engine->initSceneManager();

	ISceneManager* sceneMgr = g_Engine->getSceneManager();

	ICamera* cam = sceneMgr->addCamera(vector3df(80, 240, -50), vector3df(0,220,0), vector3df(0,1,0), 1.0f, 2500.0f, PI/4.0f);

	createScene();

	if (g_mapTileSceneNode)
	{
		vector3df vCenter = g_mapTileSceneNode->getCenter();
		cam->setPosition(cam->getPosition() + vCenter);
		cam->recalculateAll();

		cam->setLookat(vCenter);
	}

	createInput();

	MSG msg;
	while (!g_bExit)	
	{
		if ( ::PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage(&msg);
		}
		else
		{
			bool active = !g_bBackMode && ::GetActiveWindow() == hwnd;
			if (active)
			{
				processInput();
			}

			if (sceneMgr->beginFrame(active))
			{
				sceneMgr->drawAll(!g_bBackMode);

				sceneMgr->endFrame();
			}

			::Sleep(1);
		}
	}

	destroyInput();

	destroyScene();

	destroyEngine();

	bool safe = globalDbg.endCheckPoint();
	_ASSERT(safe);

	globalDbg.outputMaxMemoryUsed();

	CMiniDump::end();

	return 0;
}

