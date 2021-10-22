#pragma once
#include"Scene.h"

//�A�N�V�����p�[�g�̃V�[��
class ActionPart :public MelLib::Scene
{
private:

private:
	// �|�[�Y���̍���
	bool isPause = false;
	int currentPauseSelect = 0;
	struct PauseMenu 
	{
		static const int PAUSE_END = 0;
		static const int RESTART = 1;
		static const int OPTION = 2;
		static const int RETURN_TITLE = 3;
		static const int NUM_MAX = 3;
	};

private:
	void LoadResources();

	void Pause();
public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��
};

