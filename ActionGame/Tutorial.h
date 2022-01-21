#pragma once
#include<Sprite2D.h>
#include<Input.h>

// �`���[�g���A��
class Tutorial
{
private:

	// �`���[�g���A���̃^�C�v
	struct TutorialType
	{
		static const int MOVE = 0;
		static const int CAMERA = 1;
		static const int ATTACK = 2;
		static const int JUMP = 3;
		static const int END = 4;
	};
	int currentTutorial = TutorialType::MOVE;

	// �Œ�̃`���[�g���A�����b�Z�[�W
	static const std::unordered_map<int, std::wstring> TUTORIAL_TEXT;
	static const std::unordered_map<MelLib::PadButton, std::wstring> PAD_BUTTON_TEXT;

	// �R���t�B�O�ɂ���ĕς��`���[�g���A�����b�Z�[�W
	const std::unordered_map<int, std::wstring> operationText;

	MelLib::Sprite2D tutorialTextSpr;
private:


public:
	static void LoadResources();

	void NextTutorial();
	bool TutorialEnd()const { return currentTutorial == TutorialType::END; }

	Tutorial();

	void Update();
	void Draw();

};

