#pragma once

#include "ISceneNode.h"

//����map�е���գ����գ���Ȼ�������
class ISkySceneNode : public ISceneNode
{
public:
	ISkySceneNode() : ISceneNode(NULL_PTR){ Type = EST_SKY; }

	virtual ~ISkySceneNode() { }

public:

};