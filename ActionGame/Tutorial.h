#pragma once

// �`���[�g���A��
class Tutorial
{
private:

	// �`���[�g���A���̃^�C�v
	enum class TutorialType
	{
		MOVE,
		ATTACK,
		JUMP,
		CAMERA,
	};
	TutorialType currentTutorial = TutorialType::MOVE;
public:
	void Update();
	void Draw();

};

