#include "Tutorial.h"
#include"Game.h"
#include"Player.h"

#include"TextWrite.h"

const std::unordered_map<int, std::wstring> Tutorial::TUTORIAL_TEXT=
{
	{TutorialType::NOT_STARTED,L""},
	{TutorialType::MOVE_AND_CAMERA,L"で移動します。\nで視点を動かします。"},
	{TutorialType::ATTACK,L"で攻撃します。"},
	{TutorialType::DASH,L"でダッシュします。"},
	{TutorialType::JUMP,L"でジャンプします。"},
};

const std::unordered_map<int, int> Tutorial::DRAW_TEXTURE_NUM =
{
	{TutorialType::NOT_STARTED,0},
	{TutorialType::MOVE_AND_CAMERA,2},
	{TutorialType::ATTACK,1},
	{TutorialType::DASH,1},
	{TutorialType::JUMP,1},
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
		drawWindow = true;

		for (int i = 0; i < _countof(tutorialButtonSpr); i++)
		{
			tutorialButtonSpr[i].SetPosition(MelLib::Vector2(150, 270 + 70 * i));
		}

		tutorialTextPos = MelLib::Vector2(350, 300);
	}
}

Tutorial::Tutorial()
{
	//tutorialTextSpr.Create(MelLib::Texture::Get("tutorialText0"));
	//tutorialTextSpr.Create(MelLib::Color(255,255));
	//tutorialTextSpr.SetScale(400);

	backGroundSpr.Create(MelLib::Texture::Get("backGround"));
	backGroundSpr.SetPosition(MelLib::Vector2(70, 40));

	tutorialButtonSpr[0].Create();
	tutorialButtonSpr[1].Create();
	
	
	closeWindowButtonSpr.Create(MelLib::Texture::Get("ButtonA"));
	closeWindowButtonSpr.SetPosition(MelLib::Vector2(790,520));
	closeWindowButtonSpr.SetScale(0.8f);
}

void Tutorial::Update()
{
	// ウィンドウ閉じる処理
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::A) && drawWindow)
	{
		drawWindow = false;

		for (int i = 0; i < _countof(tutorialButtonSpr); i++) 
		{
			tutorialButtonSpr[i].SetPosition(MelLib::Vector2(20, 570 + -70 * i));
		}

		tutorialTextPos = MelLib::Vector2(200, 600);
	}

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

	std::string textureName[2];
	switch (currentTutorial)
	{
	case TutorialType::MOVE_AND_CAMERA:
		textureName[1] = "StickL";
		textureName[0] = "StickR";
		break;

	default:
		textureName[0] = "Button" + PAD_BUTTON_STRING.at(drawPadButton);
		break;
	}
	for(int i = 0; i < DRAW_TEXTURE_NUM.at(currentTutorial); i++)
	{
		tutorialButtonSpr[i].SetTexture(MelLib::Texture::Get(textureName[i]));
	}
}

void Tutorial::Draw()
{
	if (currentTutorial == TutorialType::END
		|| currentTutorial == TutorialType::NOT_STARTED)return;


	//tutorialTextSpr.Draw();


	MelLib::TextWrite::Draw
	(
		tutorialTextPos,
		MelLib::Color(230, 255),
		TUTORIAL_TEXT.at(currentTutorial),
		"Arial"
	);

	for (int i = 0; i < DRAW_TEXTURE_NUM.at(currentTutorial); i++)
	{
		tutorialButtonSpr[i].Draw();
	}

	if (drawWindow) 
	{
		backGroundSpr.Draw();
		closeWindowButtonSpr.Draw();

		MelLib::TextWrite::Draw
		(
			MelLib::Vector2(940, 530),
			MelLib::Color(230, 255),
			L"で閉じる",
			"Arial"
		);
	}
}
