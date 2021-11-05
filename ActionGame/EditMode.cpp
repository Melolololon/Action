#include "EditMode.h"

#include<string>
#include<vector>
#include<fstream>
#include<filesystem>

#include<GameObjectManager.h>
#include<SceneManager.h>
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
#include"Ground.h"
#pragma endregion


void EditMode::Save()
{
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();

	std::string filePath = typeid(currentScene).name() + ADD_PATH;

	std::ofstream file;
	file.open(filePath, std::ios_base::binary);

	const std::vector<std::shared_ptr<MelLib::GameObject>>& refGameObjects = 
		MelLib::GameObjectManager::GetInstance()->GetRefGameObject();

	for (size_t i = 0; i < refGameObjects.size(); i++)
	{
		// ���ʔԍ���������
		file.write(reinterpret_cast<char*>(&objectTypes[i]), sizeof(int));
		file.write(reinterpret_cast<char*>(&objectNums[i]), sizeof(int));

		// ���W�̏�������
		MelLib::Vector3 writeParam = refGameObjects[i]->GetPosition();
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		// ���f���̐���������
		int modelNum = refGameObjects[i]->GetRefModelObjects().size();
		file.write(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

		// ���f���̊p�x�Ƒ傫���̏�������
		for (const auto& model : refGameObjects[i]->GetRefModelObjects())
		{
			writeParam = model.second.GetAngle();
			file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

			writeParam = model.second.GetScale();
			file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));
		}
	}

	file.close();
}

void EditMode::Load()
{
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	std::string filePath = typeid(currentScene).name() + ADD_PATH;

	std::ifstream file;
	file.open(filePath, std::ios_base::binary);

	if (!file)return;

	size_t fileSize = std::filesystem::file_size(filePath);

	// �S���ǂݍ���
	while (fileSize != file.tellg())
	{
		file.read(reinterpret_cast<char*>(&objectType), sizeof(objectType));
		file.read(reinterpret_cast<char*>(&objectNum), sizeof(objectNum));

		file.read(reinterpret_cast<char*>(&addObjectPos), sizeof(addObjectPos));

		int modelNum = 0;
		file.read(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

		for (int i = 0; i < modelNum; i++)
		{
			file.read(reinterpret_cast<char*>(&addObjectAngle), sizeof(addObjectAngle));
			file.read(reinterpret_cast<char*>(&addObjectScale), sizeof(addObjectScale));
		}

		// �ǂݍ��񂾏������ɒǉ�
		AddObject();
	}

	file.close();
}



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
	imguiManager->DrawSliderVector3("Scale", addObjectScale, 0.0001f, 150.0f);

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

	case GROUND:
		add(std::make_shared<Ground>(addObjectPos, addObjectAngle, addObjectScale.ToVector2()));
		return;

	default:
		return;
		break;
	}

	// �ǉ��ɐ���������A�I�u�W�F�N�g�ԍ����i�[
	objectTypes.push_back(objectType);
	objectNums.push_back(objectNum);
}
