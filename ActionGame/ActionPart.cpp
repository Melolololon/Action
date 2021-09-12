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

	Ground::LoadResource();
	
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>(MelLib::Vector3(0,5,0)));

	
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Ground>());

}

void ActionPart::Update()
{
	////ƒ|[ƒY
	if (MelLib::Input::ButtonTrigger(1,MelLib::GamePadButton::START))isPause = isPause == false ? true : false;
	if (isPause)return;

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
