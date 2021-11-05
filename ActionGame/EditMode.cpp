#include "EditMode.h"

#include<GameObjectManager.h>
#include<ImguiManager.h>
#include<Input.h>


#pragma region OBJ_TYPE_PLAYER
#include"Player.h"
#pragma endregion

#pragma region OBJ_TYPE_ENEMY
#include"NoemalEnemy.h"
#pragma endregion

#pragma region OBJ_TYPE_STAGE

#pragma endregion

#pragma region OBJ_TYPE_FIERD

#pragma endregion


void EditMode::Update()
{



	MelLib::ImguiManager* imguiManager = MelLib::ImguiManager::GetInstance();
	imguiManager->BeginDrawWindow("Edit");

	// �ǂ�����Ĕz�u����悤�ɂ���?
	// �r���{�[�h�̃A���S���Y���ŃJ�����̕��ɕ��ʂ̖@����������悤�ɂ��āA�}�E�X�Œu���Ă���?
	// ����Ƃ��A���Ɣ������Ēu���Ă���?�ǂ����n��ɂ����u���Ȃ����낤���炱��ł����Ȃ��͂�
	// �ƂȂ�ƁA�������ׂĊi�[���Ȃ��Ƃ����Ȃ�

	// �J�������ړ��ł���悤�ɂ���
	// imgui�ō��W�������Ēǉ����銴���ł���?

	imguiManager->DrawRadioButton("OBJ_Player", objectType, OBJ_TYPE_PLAYER);
	imguiManager->DrawRadioButton("OBJ_Enemy", objectType, OBJ_TYPE_ENEMY);
	imguiManager->DrawRadioButton("OBJ_Stage", objectType, OBJ_TYPE_STAGE);
	imguiManager->DrawRadioButton("OBJ_Fierd", objectType, OBJ_TYPE_FIERD);

	imguiManager->DrawSliderInt("ObjectNum", objectNum, 0, 10);

	imguiManager->DrawSliderVector3("Position", addObjectPos, -1000.0f, 1000.0f);
	imguiManager->DrawSliderVector3("Angle", addObjectAngle, 0.0f, 359.9999f);

	imguiManager->EndDrawWindow();

	if (MelLib::Input::KeyTrigger(DIK_SPACE))AddObject();


}

void EditMode::Draw()
{
}

void EditMode::AddObject()
{
	
	auto add = [](std::shared_ptr<MelLib::GameObject> pObj)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(pObj);
	};

	switch (objectType + objectNum)
	{
	case PLAYER:
		add(std::make_shared<Player>(addObjectPos));
		break;

	case NORMAL_ENEMY:
		add(std::make_shared<NoemalEnemy>(addObjectPos));
		break;
	}
}
