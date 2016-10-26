#pragma once

#include "core.h"
#include "IFileWDT.h"
#include <unordered_map>

class CWDTSceneNode;
class CMapChunk;
class CFileWDT;

//wdt�ĳ�������
class wow_wdtScene
{
public:
	explicit wow_wdtScene(CWDTSceneNode* wdtNode);
	~wow_wdtScene();

public:
	void update();
	void setCameraChunk(CMapChunk* chunk) { CamChunk = chunk; }
	const CMapChunk* getCameraChunk() const { return CamChunk; }

	s32 getRow() const { return Row; }
	s32 getCol() const { return Col; }

	void setAdtLoadSize(E_ADT_LOAD size);

	void setCurrentTile(s32 row, s32 col);
	void startLoadADT(STile* tile);
	bool processADT();
	bool updateBlocksByCamera();
	void unloadOutBlocks();

	void updateTileTransform(STile* tile);
	bool isTileNeeded(STile* tile) const;		//�Ƿ�����Ҫ���صķ�Χ��

	bool isLoading(STile* tile) const;
	void recalculateTilesNeeded(s32 row, s32 col);
	u32 getNumBlocks() const;

private:
	CWDTSceneNode* WdtSceneNode;
	CFileWDT*	FileWDT;

	//�Ѽ��غͼ����е�tile, falseΪ�����У�trueΪ�Ѽ���
	typedef std::unordered_map<STile*, bool>		T_TileMap;
	T_TileMap		TilesMap;

	CMapChunk*			CamChunk;
	CMapChunk*			LastCamChunk;

	s32	Row;			//��������row
	s32	Col;				//��������col;

	vector2di		Coords[25];
	
	//flags
	u32 TileChangeTick;
	bool TileChanging;
	
	E_ADT_LOAD		AdtLoadSize;
};
