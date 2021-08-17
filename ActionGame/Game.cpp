#include "Game.h"
#include"Library.h"

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
		if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();

		Update(); 
		Draw();
		MelLib::Library::LoopEndProcess();

		if (MelLib::Library::GetIsEnd())break;
	}

	Finitialize();
}

void Game::Initialize()
{
	MelLib::Library::Initialize(1280, 720, MelLib::Color(255, 0, 0, 255), L"Game");
	MelLib::Library::SetFramesPerSecond60(true);
}

void Game::Finitialize()
{
}

void Game::Update()
{
}

void Game::Draw()
{
}
