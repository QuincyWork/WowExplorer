#pragma once

#include "core.h"
#include "IResourceCache.h"
#include "wow_adt_structs.h"

#define TILENUM		64

class MPQFile;
class IFileADT;
class CMapEnvironment;

struct STile	
{
	STile() : fileAdt(NULL), heightmapOuter(NULL),  heightmapInner(NULL){ }
	u8  row;
	u8  col;
	IFileADT*	fileAdt;
	s16*	heightmapOuter;
	s16*	heightmapInner;
};



//WDT����һ���ͼ�Ļ�����Ϣ��WDT�������Tile, ÿ��Tile���ܰ���ADT�ļ���
//������ADT�ļ���Tile���ǲ�ʹ��Ҳ���ɼ�
//ADT�ļ��������Tile�ľ�����Ϣ: ģ��,����,����...
//WDL�ļ�����һ���ͷֱ��ʵĵ���ģ�ͣ�������������Զ���ĵ�������
class IFileWDT : public IReferenceCounted<IFileWDT>
{
protected:
	virtual void onRemove() {}
	
public:
	IFileWDT() 
	{
		WmoFileNameIndices = NULL;
		WmoFileNameBlock = NULL;
		WmoInstances = NULL;
		TileOffsets = NULL;

		::memset(GlobalWmoFileName, 0, MAX_PATH);
		::memset(&GlobalWmoInstance, 0, sizeof(SWmoInstance));

		TextureInfos = NULL;
		TextureFileNameBlock = NULL;

		NumWmoFileNames = NumWmoInstance = 0;
	}
	virtual ~IFileWDT() { }

public:
	struct SWmoInstance
	{
		u32 wmoIndex;
		s32 id;
		vector3df pos;
		vector3df dir;
		matrix4		mat;
		aabbox3df		box;
		f32		scale;
	};

public:
	virtual bool loadFile(MPQFile* file, s32 mapid) = 0;
	virtual u8* getFileData() const = 0;

	virtual s32 getMapId() const = 0;
	virtual const c16* getMapName() const = 0;
	virtual CMapEnvironment*	 getMapEnvironment() const  = 0;

	virtual bool getPositionByTile(s32 row, s32 col, vector3df& pos) = 0;
	virtual bool getTileByPosition(vector3df pos, s32& row, s32& col) = 0;
	virtual STile* getTile(u8 row, u8 col) = 0;
	virtual STile* getTile(u32 index) = 0;
	virtual u32 getTileCount() const = 0;

	virtual bool loadADT(STile* tile) = 0;
	virtual bool unloadADT(STile* tile) = 0;

	virtual void getADTFileName(u8 row, u8 col, c8* name, u32 size) = 0;
	virtual const c8* getWMOFileName(u32 index) const = 0;

public:
	//wmo
	u32		NumWmoFileNames;
	u32*		WmoFileNameIndices;
	c8*		WmoFileNameBlock;
	u32		NumWmoInstance;
	SWmoInstance*		WmoInstances;
	s32*		TileOffsets;

	c8		GlobalWmoFileName[MAX_PATH];
	SWmoInstance		GlobalWmoInstance;
		 
	//tex
	u32 NumTextures;
	ADT::TXBT*	TextureInfos;
	c8* TextureFileNameBlock;

};
