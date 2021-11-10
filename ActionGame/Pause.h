#pragma once
#include<Easing.h>
#include<Sprite2D.h>

class Pause
{
private:
	bool loadResource = false;

	bool isPause = false;

	// �|�[�Y���I�������(�I���������s����)
	bool pauseEnd = false;

	//���ݑI�΂�Ă��鍀��
	int currentPauseSelect = 0;
	//�{�^����������đI�΂ꂽ����
	int pushPauseSelect = 0;
	// �|�[�Y���̍���
	struct PauseMenu
	{
		static const int PAUSE_END = 0;
		static const int RESTART = 1;
		static const int OPTION = 2;
		static const int RETURN_TITLE = 3;
		static const int NUM_MAX = 3;
	};

	// �|�[�Y�֌W�̃X�v���C�g�̃A���t�@�l
	MelLib::Easing<float>pauseSubAlpha;
	MelLib::Easing<float>pauseBackSubAlpha;

	// ����Object2D�ɂ��Ă���������
	// �|�[�Y���̔w�i
	MelLib::Sprite2D pauseBackSpr;

private:
	Pause(){}
	~Pause(){}

	void ActionPartPauseUpdate();
	void ActionPartPauseDraw();

	void CreateSprite();
public:
	Pause(Pause& p) = delete;
	Pause& operator=(Pause& p) = delete;
	static Pause* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsPause() { return isPause; }
};

