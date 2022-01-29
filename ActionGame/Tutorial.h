#pragma once
#include<Sprite2D.h>
#include<Input.h>

// チュートリアル
class Tutorial
{
private:

	// チュートリアルのタイプ
	struct TutorialType
	{
		static const int MOVE = 0;
		static const int CAMERA = 1;
		static const int ATTACK = 2;
		static const int JUMP = 3;
		static const int DASH = 4;
		static const int END = 4;
	};
	int currentTutorial = TutorialType::MOVE;

	// 固定のチュートリアルメッセージ
	static const std::unordered_map<int, std::wstring> TUTORIAL_TEXT;
	static const std::unordered_map<MelLib::PadButton, std::string> PAD_BUTTON_STRING;
	static const std::unordered_map<MelLib::PadButton, std::wstring> PAD_BUTTON_WSTRING;


	MelLib::Sprite2D tutorialBackGroundSpr;
	MelLib::Sprite2D tutorialButtonSpr;

	MelLib::PadButton drawPadButton = MelLib::PadButton::A;
	bool drawConfigButton = false;
private:
	

public:
	static void LoadResources();

	void NextTutorial();
	bool TutorialEnd()const { return currentTutorial == TutorialType::END; }

	Tutorial();

	void Update();
	void Draw();

};

