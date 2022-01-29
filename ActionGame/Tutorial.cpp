#include "Tutorial.h"
#include"Game.h"
#include"Player.h"

#include"TextWrite.h"

const std::unordered_map<int, std::wstring> Tutorial::TUTORIAL_TEXT=
{
	{TutorialType::MOVE,L"で移動します。"},
	{TutorialType::CAMERA,L"で視点を動かします。"},
	{TutorialType::ATTACK,L"で攻撃します。"},
	{TutorialType::DASH,L"でダッシュします。\n攻撃を回避するときなどに使用します。"},
	{TutorialType::JUMP,L"でジャンプします。"},
};

const std::unordered_map<MelLib::PadButton, std::string> Tutorial::PAD_BUTTON_STRING =
{
	{MelLib::PadButton::A,"A"},
	{MelLib::PadButton::B,"B"},
	{MelLib::PadButton::X,"X"},
	{MelLib::PadButton::Y,"Y"},
	{MelLib::PadButton::LB,"LB"},
	{MelLib::PadButton::RB,"RB"},
};

const std::unordered_map<MelLib::PadButton, std::wstring> Tutorial::PAD_BUTTON_WSTRING = 
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

	const std::string BUTTON_PATH = "Input/Button/";
	const std::string STICK_PATH = "Input/Stick/";

	MelLib::Texture::Load(TEXTURE_PATH + BUTTON_PATH + "ButtonA.png", "ButtonA");
	MelLib::Texture::Load(TEXTURE_PATH + BUTTON_PATH + "ButtonB.png", "ButtonB");
	MelLib::Texture::Load(TEXTURE_PATH + BUTTON_PATH + "ButtonX.png", "ButtonX");
	MelLib::Texture::Load(TEXTURE_PATH + BUTTON_PATH + "ButtonY.png", "ButtonY");
	MelLib::Texture::Load(TEXTURE_PATH + BUTTON_PATH + "ButtonLB.png", "ButtonLB");
	MelLib::Texture::Load(TEXTURE_PATH + BUTTON_PATH + "ButtonRB.png", "ButtonRB");
	MelLib::Texture::Load(TEXTURE_PATH + STICK_PATH + "StickL.png", "StickL");
	MelLib::Texture::Load(TEXTURE_PATH + STICK_PATH + "StickR.png", "StickR");
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

	tutorialButtonSpr.Create();
	tutorialButtonSpr.SetPosition(MelLib::Vector2(30, 30));
	
}

void Tutorial::Update()
{

	// チュートリアル中の変更に対応するために動的に取得

	// キーの設定を取得
	const std::unordered_map<Player::ActionType, MelLib::PadButton> configData = Player::GetKeyConfigData();
	switch (currentTutorial)
	{
	case TutorialType::ATTACK:
		drawPadButton = configData.at(Player::ActionType::ATTACK);

		break;
	case TutorialType::JUMP:
		drawPadButton = configData.at(Player::ActionType::JUMP);
		break;

	case TutorialType::DASH:
		drawPadButton = configData.at(Player::ActionType::DASH);
		break;

	default:
		break;
	}

	std::string textureName;
	switch (currentTutorial)
	{
	case TutorialType::MOVE:
		textureName = "StickL";

		break;
	case TutorialType::CAMERA:
		textureName = "StickR";
		break;

	default:
		textureName = "Button" + PAD_BUTTON_STRING.at(drawPadButton);
		break;
	}
	tutorialButtonSpr.SetTexture(MelLib::Texture::Get(textureName));
}

void Tutorial::Draw()
{
	//tutorialTextSpr.Draw();


	MelLib::TextWrite::Draw
	(
		MelLib::Vector2(300, 300),
		MelLib::Color(230, 255),
		TUTORIAL_TEXT.at(currentTutorial) ,
		"Arial"
	);

	tutorialButtonSpr.Draw();
	tutorialBackGroundSpr.Draw();
}
