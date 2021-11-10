#include "Pause.h"

#include<Input.h>

void Pause::ActionPartPauseUpdate()
{
	// ���ڑI���������Ɉڂ�

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
	// �|�[�Y�I��
	else if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START))
	{
		pauseEnd = true;
	}

	if (isPause)return;


	static const float PAUSE_SUB_ALPHA_SPEED = 10.0f;
	//�X�v���C�g�̃A���t�@�l�̌��Z�l����
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

	//��������50.0f�Ŏ~�߂�
	//pauseBackSpr.SetSubColor(MelLib::Color(0,0,0, pauseBackSubAlpha2.GetValue()));
	pauseBackSpr.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(pauseBackSubAlpha.GetValue())));

	// ���ڕύX
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


	// �I��
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))pushPauseSelect = currentPauseSelect;

	// �I���������̂ɉ����ď������s��
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
