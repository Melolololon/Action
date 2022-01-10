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
#include"Rock.h"
#pragma endregion

#pragma region OBJ_TYPE_FIERD
#include"Ground.h"
#include"Wall.h"
#pragma endregion


std::string EditMode::GetFileName()
{
	std::string path;

	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	path = typeid(*currentScene).name();

	// �擪6����("class ")���폜
	path.erase(path.begin(), path.begin() + 6);

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

	int objectNumber = 0;
	for (size_t i = 0; i < refGameObjects.size(); i++)
	{
		if (i != 0)
		{
			bool addObjectFlag = false;
			// �ۑ����Ȃ��Ă�����͖���
			for (auto& p : pGameObjects)
			{
				if (refGameObjects[i].get() == p)
				{
					addObjectFlag = true;
					break;
				}
			}

			if (!addObjectFlag)
			{
				continue;
			}

			// ���ʔԍ���������
			file.write(reinterpret_cast<char*>(&objectTypes[objectNumber]), sizeof(int));
			file.write(reinterpret_cast<char*>(&objectNums[objectNumber]), sizeof(int));
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

		if (i != 0)
		{
			objectNumber++;
		}
	}

	file.close();
}



bool EditMode::Load(std::shared_ptr<Player>& p, std::vector<std::shared_ptr<MelLib::GameObject>>* pEnemys)
{
	this->pEnemys = pEnemys;

	std::string filePath = GetFileName();

	std::ifstream file;
	file.open(filePath, std::ios_base::binary);



	if (!file)
	{
		// ���̏������
		/*objectTypes.push_back(OBJ_TYPE_FIERD);
		objectNums.push_back(GROUND - OBJ_TYPE_FIERD);*/


		p = std::make_shared<Player>(MelLib::Vector3(0, 5, 0));
		MelLib::GameObjectManager::GetInstance()->AddObject(p);
		pGameObjects.push_back(p.get());
		// �����I�ɃG�f�B�b�g����J�n
		isEdit = true;


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
	MelLib::GameObjectManager::GetInstance()->AddObject(p);
	pGameObjects.push_back(p.get());

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
		selectObject = GetPObject();
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

	imguiManager->DrawSliderFloat("PositionX", addObjectPos.x, playerPos.x - 300.0f, playerPos.x + 300.0f);
	imguiManager->DrawSliderFloat("PositionY", addObjectPos.y, playerPos.y - 300.0f, playerPos.y + 300.0f);
	imguiManager->DrawSliderFloat("PositionZ", addObjectPos.z, playerPos.z - 300.0f, playerPos.z + 300.0f);
	imguiManager->DrawSliderVector3("Angle", addObjectAngle, 0.0f, 359.9999f);
	imguiManager->DrawSliderVector3("Scale", addObjectScale, 0.0001f, 1000.0f);

	// �폜���邩�ǂ����̃t���O
	bool deleteCurrentAddObject = false;

	bool getParam = false;

	if (pGameObjects.size() >= 2)
	{
		imguiManager->DrawSliderInt("CurrentAddObject", currentAddObject, 1, pGameObjects.size() - 1);

		imguiManager->DrawCheckBox("DeleteCurrentAddObject", deleteCurrentAddObject);


		imguiManager->DrawCheckBox("GetCurrentAddObjectParam", getParam);
	}
	imguiManager->EndDrawWindow();

	// �I�u�W�F�N�g�̕ύX����������selectObject��ύX
	if (changeObject)
	{
		selectObject.reset();
		selectObject = GetPObject();
	}

	if (selectObject)
	{
		selectObject->SetPosition(addObjectPos);
		selectObject->SetAngle(addObjectAngle);

		if (objectType == OBJ_TYPE_FIERD || objectType == OBJ_TYPE_STAGE)
		{
			selectObject->SetScale(addObjectScale);
		}
	}
	else
	{
		selectObject = GetPObject();
	}

	// �ǉ�
	if (MelLib::Input::KeyTrigger(DIK_SPACE))AddObject();

	// �폜
	if (deleteCurrentAddObject)
	{

		pGameObjects[currentAddObject]->TrueEraseManager();

		pGameObjects.erase(pGameObjects.begin() + currentAddObject);
		objectTypes.erase(objectTypes.begin() + currentAddObject - 1);
		objectNums.erase(objectNums.begin() + currentAddObject - 1);

		if (currentAddObject > 1) currentAddObject--;
	}

	if(getParam)
	{
		objectType = objectTypes[currentAddObject - 1];
		objectNum = objectNums[currentAddObject - 1];

		MelLib::GameObject* pObj = pGameObjects[currentAddObject];
		addObjectPos = pObj->GetPosition();
		addObjectAngle = pObj->GetRefModelObjects().at("main").GetAngle();
		addObjectScale = pObj->GetRefModelObjects().at("main").GetScale();

		selectObject.reset();
		selectObject = GetPObject();
	}

}

void EditMode::Draw()
{
	if (!(editPossible && isEdit)) return;

	if (selectObject)
	{
		selectObject->Draw();
	}
}

std::shared_ptr<MelLib::GameObject> EditMode::GetPObject()
{
	switch (objectType + objectNum)
	{

	case NORMAL_ENEMY:
		return std::make_shared<NoemalEnemy>(addObjectPos);
		break;


	case BAMBOO:
		return std::make_shared<Bamboo>(addObjectPos);
		break;

	case ROCK:
		return std::make_shared<Rock>(addObjectPos, addObjectAngle, addObjectScale,1);
		break;

	case ROCK_2:
		return std::make_shared<Rock>(addObjectPos, addObjectAngle, addObjectScale, 2);
		break;

	case ROCK_3:
		return std::make_shared<Rock>(addObjectPos, addObjectAngle, addObjectScale, 3);
		break;


	case GROUND:
		return std::make_shared<Ground>(addObjectPos, addObjectAngle, addObjectScale.ToVector2());
		break;

	case WALL:
		return std::make_shared<Wall>(addObjectPos, addObjectAngle, addObjectScale.ToVector2());
		break;


	default:
		return nullptr;
		break;
	}
}

void EditMode::AddObject()
{
	std::shared_ptr<MelLib::GameObject> getP = GetPObject();

	getP->SetPosition(addObjectPos);
	getP->SetAngle(addObjectAngle);

	if (objectType == OBJ_TYPE_FIERD || objectType == OBJ_TYPE_STAGE)
	{
		getP->SetScale(addObjectScale);
	}

	MelLib::GameObjectManager::GetInstance()->AddObject(getP);

	// �ǉ��ɐ���������A�I�u�W�F�N�g�ԍ��Ȃǂ��i�[
	objectTypes.push_back(objectType);
	objectNums.push_back(objectNum);
	pGameObjects.push_back(getP.get());

	// �G��������G���X�g�ɒǉ�
	if(objectType == OBJ_TYPE_ENEMY)pEnemys->push_back(getP);

}


EditMode* EditMode::GetInstance()
{
	static EditMode e;
	return &e;
}