#include "Game.h"
#include"Library.h"

#include"ModelData.h"
#include"ModelObject.h"

#include"GameObjectManager.h"

#include"SceneManager.h"
#include"ActionPart.h"

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
	MelLib::Library::Initialize(1280, 720, MelLib::Color(120, 120, 120, 255), L"Game");
	MelLib::Library::SetFramesPerSecond60(true);

	MelLib::GameObjectManager::GetInstance()->Initialize();

	MelLib::SceneManager::GetInstace()->SetStartScene(new ActionPart());
}

void Game::Finalize()
{
	MelLib::Library::Finalize();
}

void Game::Update()
{
	MelLib::SceneManager::GetInstace()->Update();
}

void Game::Draw()
{
	MelLib::SceneManager::GetInstace()->Draw();
}
