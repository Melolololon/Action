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
		MelLib::Input::PadButtonTrigger(MelLib::PadButton::A)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START);

	if(pushSceneChangeButton && !Fade::GetInstance()->GetIsFade())
	{
		Fade::GetInstance()->Start();
	}

	Fade::GetInstance()->Update();

	if(Fade::GetInstance()->GetChangeSceneFlag())
	{
		isEnd = true;
	}
#pragma endregion

}

void Title::Draw()
{
	Fade::GetInstance()->Draw();
}

void Title::Finalize()
{
}

MelLib::Scene* Title::GetNextScene()
{
	return new ActionPart();
}
