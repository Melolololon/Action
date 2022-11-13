#include "Title.h"

#include"Input.h"

#include"TextWrite.h"
#include"GameObjectManager.h"

#include"Fade.h"

#include"Game.h"

// Next Scene
#include"ActionPart.h"


void Title::LoadResources()
{
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
	
	MelLib::Texture::Load(TEXTURE_PATH + "Title/Title.png","title");


	MelLib::ModelData::Load("Resources/Model/Fierd/TitleFierd/TitleFierd.obj", true, "titleFierd");
}

void Title::Initialize()
{
	LoadResources();

	pPlayer = std::make_shared<Player>(MelLib::Vector3(0, 0, 0));
	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);

	titleSprite = std::make_shared<MelLib::Sprite2D>();
	titleSprite->Create(MelLib::Texture::Get("title"));
	titleSprite->SetPosition(MelLib::Vector2(270,30));

	MelLib::Camera* pCamera = MelLib::Camera::Get();
	pCamera->SetAngle(MelLib::Vector3(20,-5,0));
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(90.0f);
	pCamera->SetRotateCriteriaPosition(MelLib::Vector3(23,16,40));

	fierd.Create(MelLib::ModelData::Get("titleFierd"),"field", nullptr);
	fierd.SetScale(MelLib::Vector3(5,5,5));
	fierd.SetPosition(MelLib::Vector3(0,-13,-35));
	fierd.SetAngle(MelLib::Vector3(0, 180, 0));
}

void Title::Update()
{

	
	MelLib::GameObjectManager::GetInstance()->Update();


#pragma region シーン遷移

	bool pushSceneChangeButton =
		MelLib::Input::PadButtonTrigger(MelLib::PadButton::A)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::B)
		|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::START);

	if(pushSceneChangeButton && !Fade::GetInstance()->GetIsFade())
	{
		pushGameStart = true;
	}

	// プレイヤーのアニメーション
	if (pushGameStart) pPlayer->TitleUpdate();

	if(pPlayer->GetPosition().y <= -20.0f)
	{
		Fade::GetInstance()->Start();
	}

	Fade::GetInstance()->Update();

	if(Fade::GetInstance()->GetChangeSceneFlag())
	{
		isEnd = true;
	}
#pragma endregion

}

void Title::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
	titleSprite->Draw();

	fierd.Draw();

	Fade::GetInstance()->Draw();

	if (!pushGameStart) 
	{
		MelLib::TextWrite::Draw
		(MelLib::Vector2(470, 400), MelLib::Color(255, 255, 255, 255), L"Aボタンを押してください", "Arial");
	}
}

void Title::Finalize()
{
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* Title::GetNextScene()
{
	return new ActionPart();
}
