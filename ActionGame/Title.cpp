#include "Title.h"

#include"Input.h"

#include"Fade.h"

// Next Scene
#include"ActionPart.h"


void Title::LoadResources()
{
}

void Title::Initialize()
{
	LoadResources();

}

void Title::Update()
{
#pragma region ƒV[ƒ“‘JˆÚ

	bool pushSceneChangeButton =
		MelLib::Input::PadButtonTrigger(1, MelLib::PadButton::A)
		|| MelLib::Input::PadButtonTrigger(1, MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(1, MelLib::PadButton::START);

	if(pushSceneChangeButton && !Fade::GetInstance()->GetIsFade())
	{
		Fade::GetInstance()->Start();
	}

	if(Fade::GetInstance()->GetChangeSceneFlag())
	{
		isEnd = true;
	}
#pragma endregion

}

void Title::Draw()
{
}

void Title::Finalize()
{
}

MelLib::Scene* Title::GetNextScene()
{
	return new ActionPart();
}
