#include "Title.h"

#include"Input.h"

#include"GameObjectManager.h"

#include"Fade.h"

// Next Scene
#include"ActionPart.h"


void Title::LoadResources()
{
}

void Title::Initialize()
{
	LoadResources();

	pPlayer = std::make_shared<Player>(MelLib::Vector3(0, 0, 0));
	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);


	MelLib::Camera* pCamera = MelLib::Camera::Get();
	pCamera->SetAngle(MelLib::Vector3(20,-5,0));
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(90.0f);
	pCamera->SetRotateCriteriaPosition(MelLib::Vector3(23,16,40));
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
	Fade::GetInstance()->Draw();
}

void Title::Finalize()
{
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* Title::GetNextScene()
{
	return new ActionPart();
}
