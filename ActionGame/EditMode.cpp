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
		// 識別番号書き込み
		file.write(reinterpret_cast<char*>(&objectTypes[i]), sizeof(int));
		file.write(reinterpret_cast<char*>(&objectNums[i]), sizeof(int));

		// 座標の書き込み
		MelLib::Vector3 writeParam = refGameObjects[i]->GetPosition();
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		// モデルの数書き込み
		int modelNum = refGameObjects[i]->GetRefModelObjects().size();
		file.write(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

		// モデルの角度と大きさの書き込み
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

	// 全部読み込む
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

		// 読み込んだ情報を元に追加
		AddObject();
	}

	file.close();
}



void EditMode::Update()
{



	MelLib::ImguiManager* imguiManager = MelLib::ImguiManager::GetInstance();
	imguiManager->BeginDrawWindow("Edit");

	// どうやって配置するようにする?
	// ビルボードのアルゴリズムでカメラの方に平面の法線を向けるようにして、マウスで置いていく?
	// それとも、床と判定取って置いていく?どうせ地上にした置かないだろうからこれでも問題ないはず
	// となると、床をすべて格納しないといけない

	// カメラを移動できるようにする
	// imguiで座標いじって追加する感じでいい?

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

	// 追加に成功したら、オブジェクト番号を格納
	objectTypes.push_back(objectType);
	objectNums.push_back(objectNum);
}
