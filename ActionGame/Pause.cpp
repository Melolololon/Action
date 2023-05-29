#include "Pause.h"

#include<Input.h>
#include<Collision.h>

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

	//case PauseMenu::OPTION:
	//	break;

	case PauseMenu::RETURN_TITLE:
		break;
	}
}

void Pause::CreateSprite()
{
	if (loadResource)return;
	loadResource = true;


	pauseBackSpr.Create(MelLib::Color(0, 0, 0, 255));
	pauseBackSpr.SetScale(MelLib::Vector2(1920, 1080));


	std::string texturePath = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);

	bool res = MelLib::Texture::Load(texturePath + "UIText/Stop.png","pauseStr");
	pauseStringSpr.Create(MelLib::Texture::Get("pauseStr"));

	res = MelLib::Texture::Load(texturePath + "UIText/PauseEnd.png", "pauseEnd");
	menuStringSprites[PauseMenu::PAUSE_END].Create(MelLib::Texture::Get("pauseEnd"));

	res = MelLib::Texture::Load(texturePath + "UIText/ReStart.png", "reStart");
	menuStringSprites[PauseMenu::RESTART].Create(MelLib::Texture::Get("reStart"));

	//MelLib::Texture::Load(texturePath + "UIText/Option.png", "option");
	//menuStringSprites[PauseMenu::OPTION].Create(MelLib::Texture::Get("option"));

	//MelLib::Texture::Load(texturePath + "UIText/CheckOperation.png", "checkOperation");
	//menuStringSprites[PauseMenu::CHECK_OPERATION].Create(MelLib::Texture::Get("checkOperation"));

	res = MelLib::Texture::Load(texturePath + "UIText/ReturnTitle.png", "returnTitle");
	menuStringSprites[PauseMenu::RETURN_TITLE].Create(MelLib::Texture::Get("returnTitle"));

	// 操作説明スプライトの準備
	//MelLib::Texture::Load();
	operationSpr.Create();
}

int Pause::HitCheck()
{
	MelLib::RectData cursorRect;
	cursorRect.SetPosition(MelLib::Input::GetMousePosition());
	cursorRect.SetSize(1.0f);

	for (const auto& sprRect : menuStringRects) 
	{
		MelLib::RectData rect = sprRect.second;

		if (MelLib::Collision::RectAndRect(rect, cursorRect))return sprRect.first;
	}
	return -1;
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
	pauseEnd = false;
	prePause = false;
	isReStart = false;
	drawOperation = false;
	currentPauseSelect = 0;

	CreateSprite();

	pauseSubAlpha.SetStart(0.0f);
	pauseSubAlpha.SetEnd(100.0f);
	pauseSubAlpha.SetPar(100.0f);
	pauseBackSubAlpha.SetStart(50.0f);
	pauseBackSubAlpha.SetEnd(100.0f);
	pauseBackSubAlpha.SetPar(100.0f);


	pauseStringSpr.SetPosition(MelLib::Vector2(400, 100));
	
	
	int loopCount = 0;
	for(auto& sprite : menuStringSprites)
	{
		// 拡縮基準を設定
		sprite.second.SetScalingPoint(0.5f);
		
		// 項目の文字の大きさをセット
		if (sprite.first == 0)
		{
			sprite.second.SetScale(SELECT_SCALE);
			sprite.second.SetAddColor(MelLib::Color(255, 255, 255, 0));
		}
		else
		{
			sprite.second.SetScale(UN_SELECTED_SCALE);
			sprite.second.SetAddColor(MelLib::Color(0, 0));
		}
		// 位置をずらしながらセット
		sprite.second.SetPosition(TOP_MENU_POSITION + MOVE_POSITION * loopCount);

		menuStringRects[loopCount].SetPosition(sprite.second.GetPosition());
		menuStringRects[loopCount].SetSize(sprite.second.GetTexture()->GetTextureSize() * UN_SELECTED_SCALE);

		loopCount++;
	}

}

void Pause::Update()
{
	prePause = isPause;

	bool pushPauseButtonOrKey =
		MelLib::Input::PadButtonTrigger(PAUSE_BUTTON)
		|| MelLib::Input::KeyTrigger(PAUSE_KEY);

	bool pushPauseEndButtonOrKey = pushPauseButtonOrKey || (MelLib::Input::PadButtonTrigger(MelLib::PadButton::B));

	if (pushPauseButtonOrKey && !isPause)
	{
		isPause = true;
	}
	// ポーズ終了
	else if (pushPauseEndButtonOrKey && isPause)
	{
		pauseEnd = true;

	}

	if (!isPause)return;

	if (drawOperation)
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
			menuStringSprites[currentPauseSelect].SetAddColor(MelLib::Color(0, 0, 0, 0));
			menuStringSprites[0].SetScale(SELECT_SCALE);
			menuStringSprites[0].SetAddColor(MelLib::Color(255, 255, 255, 0));

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
	// マウス
	int hitNum = HitCheck();
	if (hitNum != -1) 
	{
		menuStringSprites[currentPauseSelect].SetScale(UN_SELECTED_SCALE);
		menuStringSprites[currentPauseSelect].SetAddColor(MelLib::Color(0, 0, 0, 0));

		currentPauseSelect = hitNum;

		menuStringSprites[hitNum].SetScale(SELECT_SCALE);
		menuStringSprites[hitNum].SetAddColor(MelLib::Color(255, 255, 255, 0));
	}

	// パッド
	bool inputPadUp = MelLib::Input::PadButtonTrigger(MelLib::PadButton::UP)
		|| MelLib::Input::LeftStickUpTrigger(80.0f);

	bool inputPadDown = MelLib::Input::PadButtonTrigger(MelLib::PadButton::DOWN)
		|| MelLib::Input::LeftStickDownTrigger(80.0f);


	if (inputPadUp)
	{
		menuStringSprites[currentPauseSelect].SetScale(UN_SELECTED_SCALE);
		menuStringSprites[currentPauseSelect].SetAddColor(MelLib::Color(0, 0, 0, 0));

		currentPauseSelect--;
		if (currentPauseSelect < 0)currentPauseSelect = PauseMenu::NUM_MAX;

		menuStringSprites[currentPauseSelect].SetScale(SELECT_SCALE);
		menuStringSprites[currentPauseSelect].SetAddColor(MelLib::Color(255, 255, 255, 0));
	}
	else if (inputPadDown)
	{
		menuStringSprites[currentPauseSelect].SetScale(UN_SELECTED_SCALE);
		menuStringSprites[currentPauseSelect].SetAddColor(MelLib::Color(0, 0, 0, 0));

		currentPauseSelect++;
		if (currentPauseSelect > PauseMenu::NUM_MAX)currentPauseSelect = 0;

		menuStringSprites[currentPauseSelect].SetScale(SELECT_SCALE);
		menuStringSprites[currentPauseSelect].SetAddColor(MelLib::Color(255, 255, 255, 0));
	}


	bool select =
		MelLib::Input::PadButtonTrigger(MelLib::PadButton::A)
		|| MelLib::Input::MouseButtonTrigger(MelLib::MouseButton::LEFT);
	
	// 決定
	if (select)
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

			/*	case PauseMenu::OPTION:
					break;
				case PauseMenu::CHECK_OPERATION:
					drawOperation = true;
					break;*/

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
