#pragma once

#include "base.h"

class IInputListener;

//���������ַ�ʽ, һ������������ļ�ʱ����, ��Ҫ��listener������, ��ҪѸ����Ӧ���ұ�����ͬһ���߳�
//��һ���ǻ����������, �紥��, ��Ҫ����һ�����У���ÿһ֡ȡ�������������Բ���ͬһ���߳�
class IInputReader
{
public:
	virtual ~IInputReader() {}

public:
	virtual bool acquire(E_INPUT_DEVICE device) = 0;
	virtual bool unacquire(E_INPUT_DEVICE device) = 0;
	virtual bool capture(E_INPUT_DEVICE device) = 0;
	virtual bool isKeyPressed(u8 keycode) = 0;
	virtual bool isMousePressed(E_MOUSE_BUTTON button) = 0;

	virtual void addListener(IInputListener* listener) = 0;
	virtual void removeListener(IInputListener* listener) = 0;
};