#include "Tutorial.h"

void Tutorial::Update()
{
	// 条件を満たしたら(移動の場合、ここまで移動したらなど)進むようにする?
	// それとも、普通にボタンで進める?

	switch (currentTutorial)
	{
	case Tutorial::TutorialType::MOVE:

		break;
	case Tutorial::TutorialType::ATTACK:
		break;
	case Tutorial::TutorialType::JUMP:
		break;
	case Tutorial::TutorialType::CAMERA:
		break;
	default:
		break;
	}
}

void Tutorial::Draw()
{
	switch (currentTutorial)
	{
	case Tutorial::TutorialType::MOVE:

		break;
	case Tutorial::TutorialType::ATTACK:
		break;
	case Tutorial::TutorialType::JUMP:
		break;
	case Tutorial::TutorialType::CAMERA:
		break;
	default:
		break;
	}
}
