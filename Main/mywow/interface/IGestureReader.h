#pragma once

#include "base.h"
#include "GestureDef.h"

//����gesture��Ϣ, ��ӵ�����
//��Ҫά�����recognizer, �������gesture��״̬
//����gesture���� -> recognizer���� -> �����

class IGestureReader
{
public:
	virtual ~IGestureReader() {}

public:
	virtual void tick() = 0;

	virtual void readTouchInfo(const SGesTouchInfo* arrTouches, u32 count) = 0;

	virtual bool addGestureToQueue(const SGestureInfo& gesInfo) = 0;

	virtual bool removeGestureFromQueue(SGestureInfo& gesInfo) = 0;

	virtual bool isQueueEmpty() const = 0;
};