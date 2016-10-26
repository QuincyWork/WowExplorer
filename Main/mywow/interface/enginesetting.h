#pragma once

#include "base.h"

class IFileSystem;
class IConfigs;

class engineSetting
{
private:
	DISALLOW_COPY_AND_ASSIGN(engineSetting);

public:
	engineSetting();

	void load();
	void save();

public:
	//
	struct SVideoSetting 
	{
		SVideoSetting()
		{
			antialias = 1;
			vsync = false;
			fullscreen = false;
			textureResolution = EL_LOW;
			textureFiltering = ETF_TRILINEAR;
			textureProjection = false;
			viewDistance = envDetail = groundClutter = shadowQuality = liquidDetail =
				sunShafts = particleDensity = EL_LOW;
			SSAO = EL_DISABLE;
		}
		u32 antialias;
		bool vsync;
		bool fullscreen;
		bool textureProjection;							//����Ͷ��
		E_LEVEL	textureResolution;	//���ʷֱ���
		E_TEXTURE_FILTER	textureFiltering;		//���ʹ���
		E_LEVEL	viewDistance;			//��Ұ����
		E_LEVEL	envDetail;		//����ϸ��
		E_LEVEL	groundClutter;		//���澰��
		E_LEVEL	shadowQuality;		//��Ӱ����
		E_LEVEL	liquidDetail;	//Һ��ϸ��
		E_LEVEL	sunShafts;		//����Ч��
		E_LEVEL		particleDensity;		//�����ܶ�
		E_LEVEL		SSAO;
	};

	//
	struct SAdvancedSetting
	{
		SAdvancedSetting()
		{
			driverType = EDT_DIRECT3D9;
			tripleBuffering = false;
			reduceInputLag = false;
			hardwareCursor = false;
			maxForegroundFPS = 120;
			maxBackgroundFPS = 50;
		}

		E_DRIVER_TYPE driverType;
		s32 maxForegroundFPS;
		s32 maxBackgroundFPS;
		bool tripleBuffering;
		bool reduceInputLag;
		bool hardwareCursor;
	};

public:

	void retrieveVideoSetting(SVideoSetting* videoSetting) const;
	void applyVideoSetting(const SVideoSetting& videosetting);

	void applyAdvancedSetting(const SAdvancedSetting& advancedSetting);

	const SVideoSetting& getVideoSetting() const { return VideoSetting; }
	const SAdvancedSetting& getAdvcanedSetting() const { return AdvancedSetting; }

	void setDriverType(E_DRIVER_TYPE driverType) { AdvancedSetting.driverType = driverType; }
	E_DRIVER_TYPE getDriverType() const { return AdvancedSetting.driverType; }

	//���ʷֱ���
	void setTextureResolution(E_LEVEL level);
	E_LEVEL getTextureResolution() const;

	//���ʹ���
	void setTextureFiltering(E_TEXTURE_FILTER filter);
	E_TEXTURE_FILTER getTextureFiltering() const;

	//����Ͷ��
	void setTextureProjection(bool projection);
	bool getTextureProjection() const;

	//��Ұ����
	void setViewDistance(E_LEVEL level);
	E_LEVEL getViewDistance() const;

	//����ϸ��
	void setEnvironmentDetail(E_LEVEL level);
	E_LEVEL getEnvironmentDetail() const;

	//���澰��
	void setGroundClutter(E_LEVEL level);
	E_LEVEL getGroundClutter() const;

	//��Ӱ����
	void setShadowQuality(E_LEVEL level);
	E_LEVEL getShadowQuality() const;

	//Һ��ϸ��
	void setLiquidDetail(E_LEVEL level);
	E_LEVEL getLiquidDetail() const;

	//����Ч��
	void setSunShafts(E_LEVEL level);
	E_LEVEL getSunShafts() const;

	//�����ܶ�
	void setParticleDensity(E_LEVEL level);
	E_LEVEL getParticleDensity() const;

	//ssao
	void setSSAO(E_LEVEL level);
	E_LEVEL getSSAO() const;

	//��������
	void setTripleBuffering(bool tripleBuffers);
	bool getTripleBuffering() const { return AdvancedSetting.tripleBuffering; }

	//���������ӳ�
	void setReduceInputLag(bool reduce);
	bool getReduceInputLag() const { return AdvancedSetting.reduceInputLag; }

	//ʹ��Ӳ��ָ��
	void setHardwareCursor(bool use);
	bool getHardwareCursor() const { return AdvancedSetting.hardwareCursor; }

	//ǰ̨����̨fps
	void setForegroundFPSLimit(s32 limit) { AdvancedSetting.maxForegroundFPS = limit; }
	s32 getForegroundFPSLimit() const { return AdvancedSetting.maxForegroundFPS; }
	void setBackgroundFPSLimit(s32 limit) { AdvancedSetting.maxBackgroundFPS = limit; }
	s32 getBackgroundFPSLimit() const { return AdvancedSetting.maxBackgroundFPS; }

private:
	bool getValue(IConfigs* config, const c8* key, bool& v);
	bool getValue(IConfigs* config, const c8* key, s32& v);
	bool getValue(IConfigs* config, const c8* key, u32& v);

	bool setValue(IConfigs* config, const c8* key, bool v);
	bool setValue(IConfigs* config, const c8* key, s32 v);
	bool setValue(IConfigs* config, const c8* key, u32 v);

private:
	SVideoSetting		VideoSetting;
	SAdvancedSetting	AdvancedSetting;
};