#include "Pause.h"

#include<Input.h>

#include"Game.h"

void Pause::ActionPartPauseUpdate()
{
	// 項目選択をここに移す

}

void Pause::ActionPartPauseDraw()
{
	//pauseBackSpr.Draw();

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

void Pause::CreateSprite()
{
	if (loadResource)return;
	loadResource = true;


	pauseBackSpr.Create(MelLib::Color(0, 0, 0, 255));
	pauseBackSpr.SetScale(MelLib::Vector2(1280, 720));


	std::string texturePath = Game::GetInstance()->GetPath(Game::ResourcePathType::TEXTURE);

	MelLib::Texture::Load(texturePath + "UIText/Stop.png","pauseStr");
	pauseStringSpr.Create(MelLib::Texture::Get("pauseStr"));
	pauseStringSpr.SetPosition(MelLib::Vector2(400,170));

	MelLib::Texture::Load(texturePath + "UIText/PauseEnd.png", "pauseEnd");
	menuStringSprites[PauseMenu::PAUSE_END].Create(MelLib::Texture::Get("pauseEnd"));

	MelLib::Texture::Load(texturePath + "UIText/ReStart.png", "reStart");
	menuStringSprites[PauseMenu::RESTART].Create(MelLib::Texture::Get("reStart"));

	MelLib::Texture::Load(texturePath + "UIText/Option.png", "option");
	menuStringSprites[PauseMenu::OPTION].Create(MelLib::Texture::Get("option"));

	MelLib::Texture::Load(texturePath + "UIText/CheckOperation.png", "checkOperation");
	menuStringSprites[PauseMenu::CHECK_OPERATION].Create(MelLib::Texture::Get("checkOperation"));

	MelLib::Texture::Load(texturePath + "UIText/ReturnTitle.png", "returnTitle");
	menuStringSprites[PauseMenu::RETURN_TITLE].Create(MelLib::Texture::Get("returnTitle"));

	
}

Pause* Pause::GetInstance()
{
	static Pause p;
	return &p;
}

void Pause::Initialize()
{
	CreateSprite();

	pauseSubAlpha.SetStart(0.0f);
	pauseSubAlpha.SetEnd(100.0f);
	pauseSubAlpha.SetPar(100.0f);
	pauseBackSubAlpha.SetStart(70.0f);
	pauseBackSubAlpha.SetEnd(100.0f);
	pauseBackSubAlpha.SetPar(100.0f);

	static const MelLib::Vector2 TOP_MENU_POSITION = MelLib::Vector2(300,300);
	static const MelLib::Vector2 MOVE_POSITION = MelLib::Vector2(0, 60);
	int loopCount = 0;
	// 位置をずらしながらセット
	for(auto& sprite : menuStringSprites)
	{
		sprite.second.SetPosition(TOP_MENU_POSITION + MOVE_POSITION * loopCount);
		loopCount++;
	}

	// 最初の項目の大きさをセット
	//menuStringSprites[0].SetScale(SELECT_SCALE);
}

void Pause::Update()
{
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::START) && !isPause)
	{
		isPause = true;
		pushPauseSelect = -1;
	}
	// ポーズ終了
	else if (isPause &&(MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START)))
	{
		pauseEnd = true;
	}

	if (!isPause)return;

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
		|| MelLib::Input::LeftStickUpTrigger(80.0f))
	{
		menuStringSprites[currentPauseSelect].SetScale(UN_SELECTED_SCALE);

		currentPauseSelect--;
		if (currentPauseSelect < 0)currentPauseSelect = PauseMenu::NUM_MAX;

		menuStringSprites[currentPauseSelect].SetScale(SELECT_SCALE);
	}
	else if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::DOWN)
		|| MelLib::Input::LeftStickDownTrigger(80.0f))
	{
		menuStringSprites[currentPauseSelect].SetScale(UN_SELECTED_SCALE);

		currentPauseSelect++;
		if (currentPauseSelect > PauseMenu::NUM_MAX)currentPauseSelect = 0;

		menuStringSprites[currentPauseSelect].SetScale(SELECT_SCALE);
	}


	// 決定
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
	if (!isPause) return;
	
	pauseBackSpr.Draw();
	pauseStringSpr.Draw();

	for (auto& sprite : menuStringSprites)
	{
		sprite.second.Draw();
	}

}
