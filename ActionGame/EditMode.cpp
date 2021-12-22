#include "EditMode.h"

#include<vector>
#include<unordered_map>
#include<fstream>
#include<filesystem>

#include<GameObjectManager.h>
#include<SceneManager.h>
#include<ImguiManager.h>
#include<Input.h>



#pragma region OBJ_TYPE_ENEMY
#include"NoemalEnemy.h"
#pragma endregion

#pragma region OBJ_TYPE_STAGE
#include"Bamboo.h"
#pragma endregion

#pragma region OBJ_TYPE_FIERD
#include"Ground.h"
#pragma endregion


std::string EditMode::GetFileName()
{
	std::string path;

	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	path = typeid(*currentScene).name();

	// �擪6����("class ")���폜
	path.erase(path.begin(),path.begin() + 6);
	
	path += "_EditData.edit";

	return path;
}

void EditMode::Save()
{
	std::string filePath = GetFileName();

	std::ofstream file;
	file.open(filePath, std::ios_base::binary);

	const std::vector<std::shared_ptr<MelLib::GameObject>>& refGameObjects = 
		MelLib::GameObjectManager::GetInstance()->GetRefGameObject();

	for (size_t i = 0; i < refGameObjects.size(); i++)
	{
		if (i != 0) 
		{
			// ���ʔԍ���������
			file.write(reinterpret_cast<char*>(&objectTypes[i - 1]), sizeof(int));
			file.write(reinterpret_cast<char*>(&objectNums[i - 1]), sizeof(int));
		}

		// ���W�̏�������
		MelLib::Vector3 writeParam = refGameObjects[i]->GetPosition();
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		// ���f���̐���������
		int modelNum = refGameObjects[i]->GetRefModelObjects().size();
		file.write(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

		// ���f���̊p�x�Ƒ傫���̏�������
		const std::unordered_map<std::string, MelLib::ModelObject>& refModels = refGameObjects[i]->GetRefModelObjects();

		for (const auto& model : refModels)
		{
			writeParam = model.second.GetAngle();
			file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

			writeParam = model.second.GetScale();
			file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));
		}
	}

	file.close();
}



bool EditMode::Load(std::shared_ptr<Player>& p)
{

	std::string filePath = GetFileName();

	std::ifstream file;
	file.open(filePath, std::ios_base::binary);



	if (!file)
	{
		// ���̏������
		objectTypes.push_back(OBJ_TYPE_FIERD);
		objectNums.push_back(GROUND - OBJ_TYPE_FIERD);

		return false;
	}

	// �v���C���[���ǂݍ���
	file.read(reinterpret_cast<char*>(&addObjectPos), sizeof(addObjectPos));

	int modelNum = 0;
	file.read(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

	for (int i = 0; i < modelNum; i++)
	{
		file.read(reinterpret_cast<char*>(&addObjectAngle), sizeof(addObjectAngle));
		file.read(reinterpret_cast<char*>(&addObjectScale), sizeof(addObjectScale));
	}
	p = std::make_shared<Player>(addObjectPos);
	
	// �c��S���ǂݍ���
	size_t fileSize = std::filesystem::file_size(filePath);
	while (fileSize != file.tellg())
	{
		file.read(reinterpret_cast<char*>(&objectType), sizeof(objectType));
		file.read(reinterpret_cast<char*>(&objectNum), sizeof(objectNum));

		file.read(reinterpret_cast<char*>(&addObjectPos), sizeof(addObjectPos));

		file.read(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

		for (int i = 0; i < modelNum; i++)
		{
			file.read(reinterpret_cast<char*>(&addObjectAngle), sizeof(addObjectAngle));
			file.read(reinterpret_cast<char*>(&addObjectScale), sizeof(addObjectScale));
		}

		// �Z�b�g
		SetSelectObject();
		// �ǂݍ��񂾏������ɒǉ�
		AddObject();
	}

	file.close();

	// �l��߂�
	objectType = OBJ_TYPE_ENEMY;
	objectNum = 0;
	addObjectPos = 0;
	addObjectAngle = 0;
	addObjectScale = 0;

	return true;
}



void EditMode::Update()
{
	if (MelLib::Input::KeyTrigger(DIK_F5))isEdit = !isEdit;
	if (!(editPossible && isEdit)) return;

	if (MelLib::Input::KeyState(DIK_LCONTROL) && MelLib::Input::KeyTrigger(DIK_S))Save();


	MelLib::ImguiManager* imguiManager = MelLib::ImguiManager::GetInstance();
	imguiManager->BeginDrawWindow("Edit");

	// �ǂ�����Ĕz�u����悤�ɂ���?
	// �r���{�[�h�̃A���S���Y���ŃJ�����̕��ɕ��ʂ̖@����������悤�ɂ��āA�}�E�X�Œu���Ă���?
	// ����Ƃ��A���Ɣ������Ēu���Ă���?�ǂ����n��ɂ����u���Ȃ����낤���炱��ł����Ȃ��͂�
	// �ƂȂ�ƁA�������ׂĊi�[���Ȃ��Ƃ����Ȃ�

	// �J�������ړ��ł���悤�ɂ���
	// imgui�ō��W�������Ēǉ����銴���ł���?

	bool changeObject = false;

	MelLib::Vector3 playerPos = pPlayer->GetPosition();
	imguiManager->DrawSliderVector3("PlayerPosition", playerPos, -1000.0f, 1000.0f);
	pPlayer->SetPosition(playerPos);


	changeObject = changeObject || imguiManager->DrawRadioButton("OBJ_Enemy", objectType, OBJ_TYPE_ENEMY);
	changeObject = changeObject || imguiManager->DrawRadioButton("OBJ_Stage", objectType, OBJ_TYPE_STAGE);
	changeObject = changeObject || imguiManager->DrawRadioButton("OBJ_Fierd", objectType, OBJ_TYPE_FIERD);

	changeObject = changeObject || imguiManager->DrawSliderInt("ObjectNum", objectNum, 0, 10);

	imguiManager->DrawSliderVector3("Position", addObjectPos, -1000.0f, 1000.0f);
	imguiManager->DrawSliderVector3("Angle", addObjectAngle, 0.0f, 359.9999f);
	imguiManager->DrawSliderVector3("Scale", addObjectScale, 0.0001f, 400.0f);

	imguiManager->EndDrawWindow();

	// �I�u�W�F�N�g�̕ύX����������selectObject��ύX
	if(changeObject)
	{
		selectObject.reset();
		SetSelectObject();
	}

	if (selectObject)
	{
		selectObject->SetPosition(addObjectPos);
		selectObject->SetAngle(addObjectAngle);

		if (objectType == OBJ_TYPE_FIERD)
		{
			selectObject->SetScale(addObjectScale);
		}
	}
	else
	{
		SetSelectObject();
	}

	if (MelLib::Input::KeyTrigger(DIK_SPACE))AddObject();

}

void EditMode::Draw()
{
	if (!(editPossible && isEdit)) return;

	if(selectObject)
	{
		selectObject->Draw();
	}
}

void EditMode::SetSelectObject()
{
	switch (objectType + objectNum)
	{

	case NORMAL_ENEMY:
		selectObject = std::make_shared<NoemalEnemy>(addObjectPos);
		break;

	case BAMBOO:
		selectObject = std::make_shared<Bamboo>(addObjectPos);
		break;

	case GROUND:
		selectObject = std::make_shared<Ground>(addObjectPos, addObjectAngle, addObjectScale.ToVector2());
		break;

	default:
		return;
		break;
	}
}

void EditMode::AddObject()
{
	MelLib::GameObjectManager::GetInstance()->AddObject(selectObject);

	// �ǉ��ɐ���������A�I�u�W�F�N�g�ԍ����i�[
	objectTypes.push_back(objectType);
	objectNums.push_back(objectNum);

	// �I���I�u�W�F�N�g������
	selectObject.reset();

}

EditMode* EditMode::GetInstance()
{
	static EditMode e;
	return &e;
}