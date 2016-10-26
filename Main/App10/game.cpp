#include "game.h"

bool g_bExit = false;
bool g_bBackMode = false;
IFileWDT* fileWDT = NULL;
IMapTileSceneNode* g_mapTileSceneNode = NULL;

void MyMessageHandler::onSize(window_type hwnd, int width, int height)
{
	if (width == 0 || height == 0)					//minimized, stop
	{ 
		g_bBackMode = true;
	}
	else
	{
		g_bBackMode = false;
		IVideoDriver* driver = g_Engine->getDriver();
		if (driver)
			driver->setDisplayMode( dimension2du((u32)width, (u32)height) );

		IFontServices* fontServices = g_Engine->getFontServices();
		if (fontServices)
			fontServices->onWindowSizeChanged( dimension2du((u32)width, (u32)height) );

		ISceneManager* smgr = g_Engine->getSceneManager();
		if(smgr)
		{
			smgr->onWindowSizeChanged( dimension2du((u32)width, (u32)height) );

			ICamera* cam = smgr->getActiveCamera();
			if(cam)
				cam->recalculateAll();
		}
	}
}


void createScene()
{
	const SMapRecord* mapRecord = g_Engine->getWowDatabase()->getMap(1);

	g_Engine->getEngineSetting()->setViewDistance(EL_FAIR);

	
	//const c8* path = "World\\wmo\\Northrend\\Dalaran\\ND_Dalaran.wmo";
	//	const c8* path ="world\\wmo\\Dungeon\\IceCrown_dungeon\\Icecrown_Halls_of_reflection.wmo";
	const c8* path = "WORLD\\WMO\\AZEROTH\\BUILDINGS\\STORMWIND\\STORMWIND.WMO";
	//const c8* path = "world\\wmo\\kalimdor\\ogrimmar\\orgrimmar2.wmo";
	IFileWMO* wmo = g_Engine->getResourceLoader()->loadWMO(path);
	IWMOSceneNode* wmoSceneNode = g_Engine->getSceneManager()->addWMOSceneNode(wmo, NULL);

	matrix4 mat;
	mat.setScale(g_Engine->getSceneRenderServices()->SCENE_SCALE);
	wmoSceneNode->setRelativeTransformation(mat);
}

void destroyScene()
{
	g_Engine->getSceneManager()->removeAllSceneNodes();

	if(fileWDT)
		fileWDT->drop();
}

void idleTick()
{
}