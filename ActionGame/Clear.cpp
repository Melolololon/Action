#include "Clear.h"

#include"Title.h"
#include"Fade.h"

void Clear::Initialize()
{
}

void Clear::Update()
{
	Fade::GetInstance()->Update();

	if(MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))
	{
		Fade::GetInstance()->Start();
	}

	if (Fade::GetInstance()->GetChangeSceneFlag())isEnd = true;
}

void Clear::Draw()
{
	Fade::GetInstance()->Draw();
}

void Clear::Finalize()
{
}

MelLib::Scene* Clear::GetNextScene()
{
	return new Title();
}
