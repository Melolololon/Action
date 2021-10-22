#include "ActionPart.h"

#include"DirectionalLight.h"

#include"GameObjectManager.h"
#include"Input.h"

#include"Player.h"

#include"Ground.h"

void ActionPart::LoadResources()
{
	Ground::LoadResource();

	pauseBackSpr.Create(MelLib::Color(0, 0, 0, 255));
}

void ActionPart::PauseUpdate()
{
	static const float PAUSE_SUB_ALPHA_SPEED = 10.0f;
	//スプライトのアルファ値の減算値増減
	if (pauseEnd)
	{
		if (pauseSubAlpha < 100.0f)
		{
			pauseSubAlpha += PAUSE_SUB_ALPHA_SPEED;
		}
		else
		{
			pauseSubAlpha = 100.0f;
			isPause = false;
			pauseEnd = false;
		}
	}
	else
	{
		if (pauseSubAlpha > 0.0f)
		{
			pauseSubAlpha -= PAUSE_SUB_ALPHA_SPEED;
		}
		else
		{
			pauseSubAlpha = 0.0f;
		}
	}

	//こいつだけ50.0fで止める
	pauseBackSpr.SetSubColor(MelLib::Color(0,0,0, pauseSubAlpha));

	// 項目変更
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::UP)
		|| MelLib::Input::LeftStickUpTrigger(30.0f, 30.0f))
	{
		currentPauseSelect--;
		if (currentPauseSelect < 0)currentPauseSelect = PauseMenu::NUM_MAX;
	}
	else if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::DOWN)
		|| MelLib::Input::LeftStickDownTrigger(30.0f, 30.0f))
	{
		currentPauseSelect--;
		if (currentPauseSelect > PauseMenu::NUM_MAX)currentPauseSelect = 0;
	}


	// 選択
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))pushPauseSelect = currentPauseSelect;

	// 選択したものに応じて処理を行う
	switch (pushPauseSelect)
	{
	case PauseMenu::PAUSE_END:
		pauseEnd = true;
		break;

	case PauseMenu::RESTART:

		break;

	case PauseMenu::OPTION:
		break;

	case PauseMenu::RETURN_TITLE:
		break;
	}

	
}

void ActionPart::PauseDraw()
{
	pauseBackSpr.Draw();

	switch (pushPauseSelect)
	{
	case PauseMenu::PAUSE_END:
		break;

	case PauseMenu::RESTART:

		break;

	case PauseMenu::OPTION:
		break;

	case PauseMenu::RETURN_TITLE:
		break;
	}
}

void ActionPart::Initialize()
{
	LoadResources();

	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(20, 0, 0));



	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>(MelLib::Vector3(0, 5, 0)));

	MelLib::GameObjectManager::GetInstance()->AddObject
	(std::make_shared<Ground>(0, MelLib::Vector3(90, 0, 0), 100));
	MelLib::GameObjectManager::GetInstance()->AddObject
	(std::make_shared<Ground>(MelLib::Vector3(0, 0, 50), MelLib::Vector3(45, 0, 0), 100));

}

void ActionPart::Update()
{
	// ポーズ(仮)
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::START)&& !isPause)
	{
		isPause = true;
		pushPauseSelect = -1;
	}
	// ポーズ終了
	else if(MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START))
	{
		pauseEnd = true;
	}



	if (isPause)
	{
		PauseUpdate();
	}



	MelLib::GameObjectManager::GetInstance()->Update();
}

void ActionPart::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
	if (isPause)
	{
		PauseDraw();
	}
}

void ActionPart::Finalize()
{
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
