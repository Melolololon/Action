#include "Game.h"
#include"Library.h"

#include"ModelData.h"
#include"ModelObject.h"
#include"ImguiManager.h"
#include<TextWrite.h>

// オブジェクト
#include"GameObject.h"
#include"GameObjectManager.h"
#include"Player.h"
#include"SlushEffect.h"
#include"HPGauge.h"
#include"Wall.h"

#include"EnemySpaunPoint.h"
#include"WeakEnemy.h"

// シーン
#include"SceneManager.h"
#include"SceneEditer.h"
#include"Fade.h"
#include"ActionPart.h"
#include"Title.h"
#include"TestScene.h"

#include"Option.h"
#include"Tutorial.h"

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
	MelLib::Library::Initialize(1920, 1080, MelLib::Color(0, 170, 255, 255), L"Game");
	MelLib::Library::SetFramesPerSecond60(true);

	//重力加速度を設定
	MelLib::GameObject::SetGravutationalAcceleration(0.2f);


	// 共通読み込み
	Player::LoadResources();

	Stage::LoadResources(0);
	SlushEffect::LoadResources();
	HPGauge::LoadResources();
	Wall::LoadResources();
	Tutorial::LoadResources();
	WeakEnemy::LoadResources();

	for (int i = 0; i < 4; i++) 
	{
		std::string numStr = std::to_string(i);
		bool res = MelLib::Texture::Load("Resources/Texture/SlushEffect/e_" + numStr + ".png", "slushEffect_" + numStr);
		int a = 0;
	}

#pragma region エディター登録
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Player>(), "Player");

	// ステージ登録
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Stage>(0), "Stage");
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Stage>(1), "Stage");

	// 敵追加
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<EnemySpaunPoint>(typeid(WeakEnemy).name()), "Enemy");
#pragma endregion


	MelLib::SceneManager::GetInstance()->SetStartScene(new TestScene());
	//MelLib::SceneManager::GetInstance()->SetStartScene(new Title());
	Fade::GetInstance()->Initializ();

	Option::GetInstance()->Initialize();
	

	MelLib::TextWrite::CreateFontData(/*L"HGPｺﾞｼｯｸE"*/L"Arial",64.0f, "Arial");

	MelLib::Camera::Get()->SetFar(10000.0f);
	
	//MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(-8, 15, -10));
	//MelLib::Camera::Get()->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_CAMERA_POSITION);
	//MelLib::Camera::Get()->SetAngle(MelLib::Vector3(35,-20,0));


	MelLib::SceneEditer::GetInstance()->SetEditerFlag(true);
	MelLib::SceneEditer::GetInstance()->Initialize();




}

void Game::Finalize()
{
	MelLib::Library::Finalize();
}

void Game::Update()
{
	MelLib::SceneManager::GetInstance()->Update();

	MelLib::SceneEditer::GetInstance()->Update();
}

void Game::Draw()
{
	MelLib::SceneManager::GetInstance()->Draw();
	MelLib::SceneEditer::GetInstance()->Draw();

}

std::string Game::GetPath(const ResourcePath type)const
{
	switch (type)
	{
	case ResourcePath::TEXTURE:
		return texturePath;
		break;
	}
}
