#pragma once

#include "base.h"
#include "wow_particle.h"
#include "S3DVertex.h"
#include "VertexIndexBuffer.h"

//������ϵͳ�ṩ���ӳغ�Ӳ������
class IParticleSystemServices
{
public:
	IParticleSystemServices() : CurrentOffset(0) { BufferParam.clear(); BufferParam.vType = EVT_BASICTEX_M; }
	virtual ~IParticleSystemServices() {}

	virtual void updateVertices(u32 numVertices, s32& baseVertIndex) = 0;
	virtual Particle* getParticle()= 0;
	virtual void putParticle(Particle* p) = 0;

	virtual void setParticleDensity(float density) = 0;		//���ӷ����ܶ�
	virtual float getParticleDensity() const = 0;

	virtual u32 getActiveParticlesCount() const = 0;

public:
	SBufferParam		BufferParam;

	SGVertex_PC*		GVertices;
	STVertex_1T*		TVertices;

	u32		CurrentOffset;
	
};