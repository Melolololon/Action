#include "ActionPart.h"

#include"DirectionalLight.h"

#include"GameObjectManager.h"
#include"Input.h"

#include"Player.h"

#include"Ground.h"

void ActionPart::Initialize()
{
	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(20, 0, 0));

	
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>(MelLib::Vector3(0,5,0)));


	Ground::LoadResource();
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Ground>(0, MelLib::Vector3(90, 0, 0), 100));
	MelLib::GameObjectManager::GetInstance()->AddObject
	(std::make_shared<Ground>(MelLib::Vector3(0,0,50), MelLib::Vector3(45, 0, 0), 100));

}

void ActionPart::Update()
{
	// �|�[�Y(��)
	if (MelLib::Input::PadButtonTrigger(1,MelLib::PadButton::START))isPause = isPause == false ? true : false;
	//if (isPause)return;

	MelLib::GameObjectManager::GetInstance()->Update();
}

void ActionPart::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void ActionPart::Finalize()
{
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
