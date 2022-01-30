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


	std::string texturePath = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);

	MelLib::Texture::Load(texturePath + "UIText/Stop.png","pauseStr");
	pauseStringSpr.Create(MelLib::Texture::Get("pauseStr"));

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

	// 操作説明スプライトの準備
	//MelLib::Texture::Load();
	operationSpr.Create();
}

Pause* Pause::GetInstance()
{
	static Pause p;
	return &p;
}

void Pause::Initialize()
{
	returnTitleFlag = false;
	isPause = false;
	isReStart = false;
	drawOperation = false;
	currentPauseSelect = 0;

	CreateSprite();

	pauseSubAlpha.SetStart(0.0f);
	pauseSubAlpha.SetEnd(100.0f);
	pauseSubAlpha.SetPar(100.0f);
	pauseBackSubAlpha.SetStart(70.0f);
	pauseBackSubAlpha.SetEnd(100.0f);
	pauseBackSubAlpha.SetPar(100.0f);


	pauseStringSpr.SetPosition(MelLib::Vector2(400, 100));
	
	// メニューの初期化
	static const MelLib::Vector2 TOP_MENU_POSITION = MelLib::Vector2(400,300);
	static const MelLib::Vector2 MOVE_POSITION = MelLib::Vector2(0, 80);
	int loopCount = 0;
	for(auto& sprite : menuStringSprites)
	{
		// 拡縮基準を設定
		sprite.second.SetScalingPoint(sprite.second.GetTexture()->GetTextureSize() / 2);
		
		// 項目の文字の大きさをセット
		if(sprite.first == 0)sprite.second.SetScale(SELECT_SCALE);
		else sprite.second.SetScale(UN_SELECTED_SCALE);

		// 位置をずらしながらセット
		sprite.second.SetPosition(TOP_MENU_POSITION + MOVE_POSITION * loopCount);
		loopCount++;
	}

	
}

void Pause::Update()
{
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::START) && !isPause)
	{
		isPause = true;
	}
	// ポーズ終了
	else if (isPause &&(MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START)))
	{
		pauseEnd = true;

	}

	if (!isPause)return;

	if(drawOperation)
	{
		if (isPause && (MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
			|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START)))drawOperation = false;

		return;
	}

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

			// 項目の大きさリセット
			menuStringSprites[currentPauseSelect].SetScale(UN_SELECTED_SCALE);
			menuStringSprites[0].SetScale(SELECT_SCALE);

			currentPauseSelect = 0;
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

	// アルファのセット
	pauseBackSpr.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(pauseBackSubAlpha.GetValue())));
	pauseStringSpr.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(pauseSubAlpha.GetValue())));
	for (auto& sprite : menuStringSprites)
	{
		sprite.second.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(pauseSubAlpha.GetValue())));
	}


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
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))
	{

		// 選択したものに応じて処理を行う
		switch (currentPauseSelect)
		{
		case PauseMenu::PAUSE_END:
			pauseEnd = true;
			break;

		case PauseMenu::RESTART:
			isReStart = true;
			break;

		case PauseMenu::OPTION:
			break;
		case PauseMenu::CHECK_OPERATION:
			drawOperation = true;
			break;

		case PauseMenu::RETURN_TITLE:
			returnTitleFlag = true;
			break;
		}
	}



}

void Pause::Draw()
{
	if (!isPause) return;
	
	pauseBackSpr.Draw();

	
	if(drawOperation)
	{
		operationSpr.Draw();
		return;
	}

	pauseStringSpr.Draw();

	for (auto& sprite : menuStringSprites)
	{
		sprite.second.Draw();
	}

}
