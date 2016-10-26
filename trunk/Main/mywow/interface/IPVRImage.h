#pragma once

#include "core.h"
#include "IResourceCache.h"
#include "SColor.h"

class IMemFile;

//���ܵĸ�ʽ 	ECF_PVRTC1_RGB_2BPP, ECF_PVRTC1_RGBA_2BPP, 
//ECF_PVRTC1_RGB_4BPP, ECF_PVRTC1_RGBA_4BPP, A8R8G8B8, R8G8B8

class IPVRImage : public IReferenceCounted<IPVRImage>
{
protected:
	virtual void onRemove() {}
	virtual ~IPVRImage() {}

public:
	IPVRImage()
	{
		Format = ECF_UNKNOWN;
		NumMipMaps = 1;
		memset(MipmapDataSize, 0, sizeof(MipmapDataSize));
		memset(MipmapPitch, 0, sizeof(MipmapPitch));
	}

public:
	virtual bool loadFile(IMemFile* file) = 0;
	virtual bool fromImageData(const u8* src, const dimension2du& size, ECOLOR_FORMAT format, bool mipmap) = 0;
	virtual const void* getMipmapData(u32 level) const = 0;  
	virtual bool copyMipmapData(u32 level, void* dest, u32 pitch, u32 width, u32 height) = 0;

	ECOLOR_FORMAT getColorFormat() const { return Format; }
	const dimension2du& getDimension() const { return Size; }
	u32 getNumMipLevels() const { return NumMipMaps; }
	u32 getMipmapDataSize(u32 level) const { return MipmapDataSize[level]; }
	u32 getMipmapPitch(u32 level) const { return MipmapPitch[level]; }

protected:
	dimension2du Size;
	u32			NumMipMaps;	
	u32			MipmapDataSize[16];
	u32			MipmapPitch[16];
	ECOLOR_FORMAT	Format;
};
