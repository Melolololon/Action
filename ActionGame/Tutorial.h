#pragma once

// チュートリアル
class Tutorial
{
private:

	// チュートリアルのタイプ
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

