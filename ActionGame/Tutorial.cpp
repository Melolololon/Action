#include "Tutorial.h"
#include"Game.h"
#include"Player.h"

#include"TextWrite.h"

const std::unordered_map<int, std::wstring> Tutorial::TUTORIAL_TEXT=
{
	{TutorialType::MOVE,L"で移動します"},
	{TutorialType::CAMERA,L"で視点を動かします"},
	{TutorialType::ATTACK,L"ボタンで攻撃します"},
	{TutorialType::JUMP,L"ボタンでジャンプします"},
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
	//const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePathType::TEXTURE);
	
	/*MelLib::Texture::Load(TEXTURE_PATH + "Tutorial/","tutorialText0");
	MelLib::Texture::Load(TEXTURE_PATH + "Tutorial/","tutorialText1");
	MelLib::Texture::Load(TEXTURE_PATH + "Tutorial/","tutorialText2");*/

}

void Tutorial::NextTutorial()
{
	if (currentTutorial != TutorialType::END)
	{
		currentTutorial++;

		// 画像を次のチュートリアルのテキストに
		//tutorialTextSpr.SetTexture(MelLib::Texture::Get("tutorialText" + std::to_string(currentTutorial)));
	}
}

Tutorial::Tutorial()
{
	//tutorialTextSpr.Create(MelLib::Texture::Get("tutorialText0"));
	//tutorialTextSpr.Create(MelLib::Color(255,255));
	//tutorialTextSpr.SetScale(400);

	
}

void Tutorial::Update()
{

	// チュートリアル中の変更に対応するために動的に取得

	// キーの設定を取得
	const std::unordered_map<Player::ActionType, MelLib::PadButton> configData = Player::GetKeyConfigData();

	// ActionTypeとチュートリアルのタイプの変換を行えるようにすれば、キーの文字列取得しやすくなる?
}

void Tutorial::Draw()
{
	//tutorialTextSpr.Draw();


	//switch (currentTutorial)
	//{
	// case :
	//	break;
	//}

	// テキストを組み合わせて描画
	/*MelLib::TextWrite::Draw
	(
		MelLib::Vector2(300,400),
		MelLib::Color(0,255),
		operationText.at(currentTutorial) + TUTORIAL_TEXT.at(currentTutorial),
		"Arial"
	);*/
}
