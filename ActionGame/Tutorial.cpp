#include "Tutorial.h"
#include"Game.h"
#include"Player.h"

#include"TextWrite.h"

const std::unordered_map<int, std::wstring> Tutorial::TUTORIAL_TEXT=
{
	{TutorialType::MOVE,L"左スティックで移動します。"},
	{TutorialType::CAMERA,L"右スティックで視点を動かします。"},
	{TutorialType::ATTACK,L"ボタンで攻撃します。"},
	{TutorialType::DASH,L"ボタンでダッシュします。\n攻撃を回避するときなどに使用します。"},
	{TutorialType::JUMP,L"ボタンでジャンプします。"},
};

const std::unordered_map<MelLib::PadButton, std::wstring> Tutorial::PAD_BUTTON_TEXT = 
{
	{MelLib::PadButton::A,L"A"},
	{MelLib::PadButton::B,L"B"},
	{MelLib::PadButton::X,L"X"},
	{MelLib::PadButton::Y,L"Y"},
	{MelLib::PadButton::LB,L"LB"},
	{MelLib::PadButton::RB,L"RB"},
};

void Tutorial::LoadResources()
{
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
	
	MelLib::Texture::Load(TEXTURE_PATH + "tutorialFrame.png","backGround");
}

void Tutorial::NextTutorial()
{
	if (currentTutorial != TutorialType::END)
	{
		currentTutorial++;
	}
}

Tutorial::Tutorial()
{
	//tutorialTextSpr.Create(MelLib::Texture::Get("tutorialText0"));
	//tutorialTextSpr.Create(MelLib::Color(255,255));
	//tutorialTextSpr.SetScale(400);

	tutorialBackGroundSpr.Create(MelLib::Texture::Get("backGround"));

	tutorialBackGroundSpr.SetPosition(MelLib::Vector2(30, 30));
}

void Tutorial::Update()
{

	// チュートリアル中の変更に対応するために動的に取得

	// キーの設定を取得
	const std::unordered_map<Player::ActionType, MelLib::PadButton> configData = Player::GetKeyConfigData();
	drawConfigButton = true;
	switch (currentTutorial)
	{
	case TutorialType::ATTACK:
		drawPadButton = configData.at(Player::ActionType::ATTACK);


	case TutorialType::JUMP:
		drawPadButton = configData.at(Player::ActionType::JUMP);
		break;

	case TutorialType::DASH:
		drawPadButton = configData.at(Player::ActionType::DASH);
		break;

	default:
		drawConfigButton = false;
		break;
	}


}

void Tutorial::Draw()
{
	//tutorialTextSpr.Draw();


	//switch (currentTutorial)
	//{
	// case :
	//	break;
	//}

	std::wstring drawText;
	if (drawConfigButton)
	{
		drawText = PAD_BUTTON_TEXT.at(drawPadButton) + TUTORIAL_TEXT.at(currentTutorial);
	}
	else
	{
		drawText = TUTORIAL_TEXT.at(currentTutorial);
	}


	MelLib::TextWrite::Draw
	(
		MelLib::Vector2(300, 300),
		MelLib::Color(230, 255),
		drawText,
		"Arial"
	);

	tutorialBackGroundSpr.Draw();
}
