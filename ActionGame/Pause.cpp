#include "Pause.h"

#include<Input.h>

void Pause::ActionPartPauseUpdate()
{
	// 横目選択をここに移す

}

void Pause::ActionPartPauseDraw()
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

Pause* Pause::GetInstance()
{
	static Pause p;
	return &p;
}

void Pause::Update()
{
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::START) && !isPause)
	{
		isPause = true;
		pushPauseSelect = -1;
	}
	// ポーズ終了
	else if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START))
	{
		pauseEnd = true;
	}

	if (isPause)return;


	static const float PAUSE_SUB_ALPHA_SPEED = 10.0f;
	//スプライトのアルファ値の減算値増減
	if (pauseEnd)
	{

		pauseSubAlpha.SetAddPar(PAUSE_SUB_ALPHA_SPEED);
		pauseBackSubAlpha.SetAddPar(PAUSE_SUB_ALPHA_SPEED);
		if (pauseSubAlpha.GetPar() < 100.0f)
		{
			pauseSubAlpha.Lerp();
			pauseBackSubAlpha.Lerp();
		}
		else
		{
			pauseSubAlpha.SetPar(100.0f);
			pauseBackSubAlpha.SetPar(100.0f);
			isPause = false;
			pauseEnd = false;
		}
	}
	else
	{

		pauseSubAlpha.SetAddPar(-PAUSE_SUB_ALPHA_SPEED);
		pauseBackSubAlpha.SetAddPar(-PAUSE_SUB_ALPHA_SPEED);
		if (pauseSubAlpha.GetPar() > 0.0f)
		{
			pauseSubAlpha.Lerp();
			pauseBackSubAlpha.Lerp();
		}
		else
		{
			pauseSubAlpha.SetPar(0.0f);
			pauseBackSubAlpha.SetPar(0.0f);
		}

	}

	//こいつだけ50.0fで止める
	//pauseBackSpr.SetSubColor(MelLib::Color(0,0,0, pauseBackSubAlpha2.GetValue()));
	pauseBackSpr.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(pauseBackSubAlpha.GetValue())));

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

void Pause::Draw()
{
}
