#pragma once

#include "core.h"
#include "wow_m2Actions.h"

#include "IResourceLoadCallback.h"
#include "ISceneNodeAddCallback.h"

class IFileM2;
class IM2SceneNode;

//�漰����m2����϶�������ͷ���Ƶ��߼����ݣ���m2ʹ��
//ʵ����Ϸ�п�����Ϊ�ļ���ȡ��������
class wow_m2Logic : public IM2LoadCallback, public IM2SceneNodeAddCallback
{
private:
	DISALLOW_COPY_AND_ASSIGN(wow_m2Logic);

public:
	wow_m2Logic();
	~wow_m2Logic();

public:
	void buildActions(IFileM2* filem2);

	void buildStates(IM2SceneNode* m2SceneNode);

	//IM2LoadedCallback
	virtual void onM2Loaded(IFileM2* filem2);

	//IM2SceneNodeAddCallback
	virtual void onM2SceneNodeAdd(IM2SceneNode* node);

private:
	bool addAction(IFileM2* filem2, E_M2_STATES act, wow_m2Action::E_PLAY_TYPE playType, s32 maxPlaytime, const SAnimationEntries& anims);

	void buildStandAction(IFileM2* fileM2);	

private:
	wow_m2Actions		M2Actions;
};