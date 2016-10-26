#pragma once

#include "core.h"
#include "ILostResetCallback.h"

class ITexture;

//��d3d9�У�������managed����(lockable)��textureWriter�������һ������
//��d3d9ex, d3d10, d3d11�У�����������Դ�����(unlockable)��textureWriter����ר�����ڴ�����д�Դ�����Ĺ���
class ITextureWriter
{
public:
	explicit ITextureWriter(u32 numMipmap) : NumMipmaps(numMipmap), ColorFormat(ECF_UNKNOWN) {}
	virtual ~ITextureWriter() {}

public:
	virtual void* lock(u32 level, u32& pitch) = 0;
	virtual void unlock(u32 level) = 0;
	virtual bool copyToTexture(ITexture* texture, const recti* descRect = NULL_PTR) = 0;
	virtual void initEmptyData() = 0;

public:
	dimension2du	TextureSize;
	ECOLOR_FORMAT	ColorFormat;
	u32	NumMipmaps;
};

class ITextureWriteServices
{
public:
	virtual ~ITextureWriteServices() {}

public:
	virtual ITextureWriter* createTextureWriter(ITexture* texture, bool temp)= 0;
	virtual bool removeTextureWriter(ITexture* texture) = 0;
};