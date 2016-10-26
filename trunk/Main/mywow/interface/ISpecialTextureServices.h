#pragma once

#include "core.h"

class ITexture;

enum E_SPECIAL_TEXTURES
{
	EST_ARMORREFLECT = 0,
	EST_ARMORREFLECT_RAINBOW,

	EST_TEXTURE_COUNT
};

//������;������
class ISpecialTextureServices
{
public:
	virtual ~ISpecialTextureServices() {}

public:
	virtual ITexture* getTexture(E_SPECIAL_TEXTURES tex) const = 0;
};