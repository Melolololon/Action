#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//�e�X�g�p
#include<ModelObject.h>

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
private:

private:
	//�e�X�g�p
	MelLib::ModelObject obj;

#pragma region �|�[�Y
	// ��C�Ƀ��j���[��\�������ɁA�����ÂA���t�@�l���グ�ĕ\�����邱��

	static bool isPause;
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
#pragma endregion

	bool isOption;

#pragma region �G�f�B�b�g�֌W
	bool editOn = true;
	static bool isEdit;

#pragma endregion

private:
	void LoadResources();

	void PauseUpdate();
	void PauseDraw();

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��

	static bool GetStopFlag() { return isPause || isEdit; }
};

