#pragma once

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
		static const int END = 4;
	};
	int currentTutorial = TutorialType::MOVE;
public:
	void NextTutorial() { currentTutorial++; }
	bool TutorialEnd()const { return currentTutorial == TutorialType::END; }

	void Update();
	void Draw();

};

