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
		static const int NOT_STARTED = 0;
		static const int MOVE = 1;
		static const int CAMERA = 2;
		static const int ATTACK = 3;
		static const int DASH = 4;
		static const int JUMP = 5;
		static const int END = 6;
	};
	int currentTutorial = TutorialType::NOT_STARTED;

	// 固定のチュートリアルメッセージ
	static const std::unordered_map<int, std::wstring> TUTORIAL_TEXT;
	static const std::unordered_map<MelLib::PadButton, std::string> PAD_BUTTON_STRING;
	static const std::unordered_map<MelLib::PadButton, std::wstring> PAD_BUTTON_WSTRING;
	


	MelLib::Sprite2D backGroundSpr;
	MelLib::Sprite2D tutorialButtonSpr;

	// 解説のウィンドウを描画するかどうか
	bool drawWindow = false;

	MelLib::Sprite2D closeWindowButtonSpr;

	MelLib::PadButton drawPadButton = MelLib::PadButton::A;
	bool drawConfigButton = false;

	// 説明のテキストの座標
	MelLib::Vector2 tutorialTextPos;

	
private:

public:
	static void LoadResources();

	void NextTutorial();
	bool GetTutorialEndFlag()const { return currentTutorial == TutorialType::END; }
	bool GetDrawWindow()const { return drawWindow; }

	Tutorial();

	void Update();
	void Draw();

};

