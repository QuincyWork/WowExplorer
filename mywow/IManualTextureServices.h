#pragma once

#include "core.h"

class IRenderTarget;
class ITexture;
class IImage;

//�ֹ�������texture, �򴴽�render target
class IManualTextureServices : public ILostResetCallback
{
public:
	virtual ~IManualTextureServices() {}

public:
	virtual ITexture* getManualTexture(const c8* name) = 0;
	virtual ITexture* addTexture( const c8* name , IImage* img) = 0;
	virtual void removeTexture(const c8* name) = 0;

	virtual IRenderTarget* addRenderTarget( dimension2du size, ECOLOR_FORMAT format ) = 0;
	virtual void removeRenderTarget( IRenderTarget* texture ) = 0;

	virtual ITexture* createTextureFromData(dimension2du size, ECOLOR_FORMAT format, void* data, bool mipmap) = 0;
	virtual ITexture* createTextureFromImage(IImage* image, bool mipmap) = 0;
	virtual ITexture* createEmptyTexture(dimension2du size, ECOLOR_FORMAT format) = 0;
};