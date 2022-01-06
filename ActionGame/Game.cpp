#include "Game.h"
#include"Library.h"

#include"ModelData.h"
#include"ModelObject.h"
#include"ImguiManager.h"
#include<TextWrite.h>

// �I�u�W�F�N�g
#include"GameObject.h"
#include"GameObjectManager.h"
#include"Player.h"
#include"SlushEffect.h"

// �V�[��
#include"SceneManager.h"
#include"Fade.h"
#include"ActionPart.h"
#include"Title.h"

#include"Option.h"

Game* Game::GetInstance()
{
	static Game game;
	return &game;
}

void Game::Run()
{
	Initialize();

	while (true)
	{
		MelLib::Library::LoopStartProcess();	

		Update(); 
		Draw();

		if (MelLib::Library::GetIsEnd())break;
	
		MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

void Game::Initialize()
{



	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(true);
	MelLib::Library::Initialize(1280, 720, MelLib::Color(120, 120, 120, 255), L"Game");
	MelLib::Library::SetFramesPerSecond60(true);

	//�d�͉����x��ݒ�
	MelLib::GameObject::SetGravutationalAcceleration(0.1f);


	// ���ʓǂݍ���
	Player::LoadResources();
	SlushEffect::LoadResources();

	MelLib::SceneManager::GetInstance()->SetStartScene(new ActionPart());
	Fade::GetInstance()->Initializ();

	Option::GetInstance()->Initialize();
	

	MelLib::TextWrite::CreateFontData(/*L"HGP�޼��E"*/L"Arial", "test");
}

void Game::Finalize()
{
	MelLib::Library::Finalize();
}

void Game::Update()
{
	MelLib::SceneManager::GetInstance()->Update();
}

void Game::Draw()
{
	MelLib::SceneManager::GetInstance()->Draw();
}

std::string Game::GetPath(const ResourcePathType type)const
{
	switch (type)
	{
	case ResourcePathType::TEXTURE:
		return texturePath;
		break;
	}
}
