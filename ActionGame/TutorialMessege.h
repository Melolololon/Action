#pragma once
#include<Sprite2D.h>

// チュートリアル
class TutorialMessege
{
public:
	enum class TutorialType
	{
		BASIC,// 基本操作
		DASH,
		JUMP,
	};

private:
	const TutorialType TUTORIAL_TYPE;

	// 枠のスプライト
	MelLib::Sprite windowSprite;
	// ボタンの絵のスプライト
	MelLib::Sprite buttonSprite;

	bool isEnd = false;
public:
	static void LoadResource();

	TutorialMessege(const TutorialType type);
	void Update();
	void Draw();
	bool GetIsEnd()const { return isEnd; }
};

