#pragma once
#include<Sprite2D.h>

// �`���[�g���A��
class TutorialMessege
{
public:
	enum class TutorialType
	{
		BASIC,// ��{����
		DASH,
		JUMP,
	};

private:
	const TutorialType TUTORIAL_TYPE;

	// �g�̃X�v���C�g
	MelLib::Sprite windowSprite;
	// �{�^���̊G�̃X�v���C�g
	MelLib::Sprite buttonSprite;

	bool isEnd = false;
public:
	static void LoadResource();

	TutorialMessege(const TutorialType type);
	void Update();
	void Draw();
	bool GetIsEnd()const { return isEnd; }
};

