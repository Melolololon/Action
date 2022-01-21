#include "Tutorial.h"
#include"Game.h"
#include"Player.h"

#include"TextWrite.h"

const std::unordered_map<int, std::wstring> Tutorial::TUTORIAL_TEXT=
{
	{TutorialType::MOVE,L"�ňړ����܂�"},
	{TutorialType::CAMERA,L"�Ŏ��_�𓮂����܂�"},
	{TutorialType::ATTACK,L"�{�^���ōU�����܂�"},
	{TutorialType::JUMP,L"�{�^���ŃW�����v���܂�"},
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

		// �摜�����̃`���[�g���A���̃e�L�X�g��
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

	// �`���[�g���A�����̕ύX�ɑΉ����邽�߂ɓ��I�Ɏ擾

	// �L�[�̐ݒ���擾
	const std::unordered_map<Player::ActionType, MelLib::PadButton> configData = Player::GetKeyConfigData();

	// ActionType�ƃ`���[�g���A���̃^�C�v�̕ϊ����s����悤�ɂ���΁A�L�[�̕�����擾���₷���Ȃ�?
}

void Tutorial::Draw()
{
	//tutorialTextSpr.Draw();


	//switch (currentTutorial)
	//{
	// case :
	//	break;
	//}

	// �e�L�X�g��g�ݍ��킹�ĕ`��
	/*MelLib::TextWrite::Draw
	(
		MelLib::Vector2(300,400),
		MelLib::Color(0,255),
		operationText.at(currentTutorial) + TUTORIAL_TEXT.at(currentTutorial),
		"Arial"
	);*/
}
