#pragma once

#include "base.h"
#include "SMaterial.h"

//���ݲ�ͬ��material���ò�ͬ����Ⱦ״̬
class IMaterialRenderer
{
public:
	virtual ~IMaterialRenderer() {}

public:
	virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderStates ) {}

	virtual void OnUnsetMaterial() {}

	//for fp
	virtual u32 getNumPasses() const { return 1; }

	virtual void OnRender(const SMaterial& material, u32 pass) {}

public:
	SRenderStateBlock		MaterialBlock;
};

