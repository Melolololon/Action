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
#include"JumpEnemy.h"
#pragma endregion

#pragma region OBJ_TYPE_STAGE
#include"Bamboo.h"
#include"Rock.h"
#include"EventFlag.h"
#include"TutorialEventFlag.h"
#pragma endregion

#pragma region OBJ_TYPE_FIERD
#include"Ground.h"
#include"Wall.h"
#pragma endregion


std::string EditMode::GetFileName()
{
	std::string path = "Resources/Edit/";

	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	std::string sceneName = typeid(*currentScene).name();

	// 先頭6文字("class ")を削除
	sceneName.erase(sceneName.begin(), sceneName.begin() + 6);
	path += sceneName;
	path += "_EditData.edit";

	return path;
}

void EditMode::Save()
{
	std::string filePath = GetFileName();

	std::ofstream file;
	file.open(filePath, std::ios_base::binary);

	// プレイヤー書き込み
	// 座標の書き込み
	MelLib::Vector3 writeParam = pPlayer->GetPosition();
	file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

	// モデルの数書き込み
	int modelNum = pPlayer->GetRefModelObjects().size();
	file.write(reinterpret_cast<char*>(&modelNum), sizeof(modelNum));

	// モデルの角度と大きさの書き込み
	const std::unordered_map<std::string, MelLib::ModelObject>& refModels = pPlayer->GetRefModelObjects();

	for (const auto& model : refModels)
	{
		writeParam = model.second.GetAngle();
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		writeParam = model.second.GetScale();
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));
	}



	const std::vector<std::shared_ptr<MelLib::GameObject>>& refGameObjects =
		MelLib::GameObjectManager::GetInstance()->GetRefGameObject();

	for (int objectNumber = 0; objectNumber < pGameObjects.size(); objectNumber++)
	{

		// 識別番号書き込み
		file.write(reinterpret_cast<char*>(&objectTypes[objectNumber]), sizeof(int));
		file.write(reinterpret_cast<char*>(&objectNums[objectNumber]), sizeof(int));

		// 座標の書き込み
		MelLib::Vector3 writeParam = addObjectPositions[objectNumber];
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		// 角度の書き込み
		writeParam = addObjectAngles[objectNumber];
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		// スケールの書き込み
		writeParam = addObjectScales[objectNumber];
		file.write(reinterpret_cast<char*>(&writeParam), sizeof(writeParam));

		// 敵の場合壁との紐付け番号書き込み
		if(objectTypes[objectNumber] == OBJ_TYPE_ENEMY)
		{
			for (const auto& enemy : *pEnemys)
			{
				if(pGameObjects[objectNumber] == enemy)
				{
					int num = enemy->GetWallNum();
					file.write(reinterpret_cast<char*>(&num), sizeof(num));
				}
			}
		}

	}

	file.close();
}



bool EditMode::Load
(
	std::shared_ptr<Player>& p,
	std::vector<std::shared_ptr<Enemy>>* pEnemys,
	std::vector<std::shared_ptr<MelLib::GameObject>>* pWalls
)
{
	this->pEnemys = pEnemys;
	this->pWalls = pWalls;

	std::string filePath = GetFileName();

	std::ifstream file;
	file.open(filePath, std::ios_base::binary);



	if (!file)
	{
		// 床の情報入れる
		/*objectTypes.push_back(OBJ_TYPE_FIERD);
		objectNums.push_back(GROUND - OBJ_TYPE_FIERD);*/



		p = std::make_shared<Player>(MelLib::Vector3(0, 5, 0));
		MelLib::GameObjectManager::GetInstance()->AddObject(p);
		//pGameObjects.push_back(p.get());
		// 強制的にエディットから開始
		isEdit = true;


		return false;
	}

	// プレイヤー情報読み込み
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
	//pGameObjects.push_back(p.get());

	// 残り全部読み込む
	size_t fileSize = std::filesystem::file_size(filePath);
	
	while (fileSize != file.tellg())
	{
		file.read(reinterpret_cast<char*>(&objectType), sizeof(objectType));
		file.read(reinterpret_cast<char*>(&objectNum), sizeof(objectNum));

		file.read(reinterpret_cast<char*>(&addObjectPos), sizeof(addObjectPos));

		file.read(reinterpret_cast<char*>(&addObjectAngle), sizeof(addObjectAngle));
		file.read(reinterpret_cast<char*>(&addObjectScale), sizeof(addObjectScale));


		if(objectType == OBJ_TYPE_ENEMY)
		{
			file.read(reinterpret_cast<char*>(&wallNum), sizeof(wallNum));
		}
	

		// セット
	/*	selectObject = GetPObject
		(
			objectType,
			objectNum,
			addObjectPos,
			addObjectAngle,
			addObjectScale,
			false
		);*/

		// 読み込んだ情報を元に追加
		AddObject();
	}

	file.close();

	// 値を戻す
	objectType = OBJ_TYPE_ENEMY;
	objectNum = 0;
	addObjectPos = 0;
	addObjectAngle = 0;
	addObjectScale = 0;

	return true;
}



void EditMode::Update()
{
	if (MelLib::Input::KeyTrigger(DIK_F5))
	{
		isEdit = !isEdit;

		// 死んでるやつを再配置
		if(isEdit)
		{
			for (int i = 0; i < pGameObjects.size(); i++)
			{
				if(pGameObjects[i]->GetEraseManager())
				{
				

					// 再追加準備
					pGameObjects[i].reset();
					std::shared_ptr<MelLib::GameObject> getP = GetPObject
					(
						objectTypes[i],
						objectNums[i],
						addObjectPositions[i],
						addObjectAngles[i],
						addObjectScales[i],
						true
					);

					// オブジェクトマネージャーに追加
					MelLib::GameObjectManager::GetInstance()->AddObject(getP);
					// 差し替え
					pGameObjects[i] = getP;
					
				

					// 情報セット
					pGameObjects[i]->SetAngle(addObjectAngles[i]);
					pGameObjects[i]->SetScale(addObjectScales[i]);

				}
			}
		}

	}
	if (!(editPossible && isEdit)) return;

	if (MelLib::Input::KeyState(DIK_LCONTROL) && MelLib::Input::KeyTrigger(DIK_S))Save();


	MelLib::ImguiManager* imguiManager = MelLib::ImguiManager::GetInstance();
	imguiManager->BeginDrawWindow("Edit");

	// どうやって配置するようにする?
	// ビルボードのアルゴリズムでカメラの方に平面の法線を向けるようにして、マウスで置いていく?
	// それとも、床と判定取って置いていく?どうせ地上にした置かないだろうからこれでも問題ないはず
	// となると、床をすべて格納しないといけない

	// カメラを移動できるようにする
	// imguiで座標いじって追加する感じでいい?

	bool changeObject = false;

	MelLib::Vector3 playerPos = pPlayer->GetPosition();
	imguiManager->DrawSliderVector3("PlayerPosition", playerPos, -10000.0f, 10000.0f);
	pPlayer->SetPosition(playerPos);


	changeObject = changeObject || imguiManager->DrawRadioButton("OBJ_Enemy", objectType, OBJ_TYPE_ENEMY);
	changeObject = changeObject || imguiManager->DrawRadioButton("OBJ_Stage", objectType, OBJ_TYPE_STAGE);
	changeObject = changeObject || imguiManager->DrawRadioButton("OBJ_Fierd", objectType, OBJ_TYPE_FIERD);

	changeObject = changeObject || imguiManager->DrawSliderInt("ObjectNum", objectNum, 0, 10);


	if (objectType == OBJ_TYPE_ENEMY)
	{
		imguiManager->DrawSliderInt("WallNum", wallNum, 0, 10);
	}


	imguiManager->DrawSliderFloat("PositionX", addObjectPos.x, playerPos.x - 800.0f, playerPos.x + 800.0f);
	imguiManager->DrawSliderFloat("PositionY", addObjectPos.y, playerPos.y - 800.0f, playerPos.y + 800.0f);
	imguiManager->DrawSliderFloat("PositionZ", addObjectPos.z, playerPos.z - 800.0f, playerPos.z + 800.0f);
	imguiManager->DrawSliderVector3("Angle", addObjectAngle, 0.0f, 359.9999f);
	imguiManager->DrawSliderVector3("Scale", addObjectScale, 0.0001f, 1000.0f);

	// 削除するかどうかのフラグ
	bool deleteCurrentAddObject = false;
	// パラメータを取得するかどうかのフラグ
	bool getParam = false;

	if (pGameObjects.size() >= 1)
	{

		preSelectAddObjectNum = selectAddObjectNum;
		imguiManager->DrawSliderInt("CurrentAddObject", selectAddObjectNum, 0, pGameObjects.size() - 1);

		imguiManager->DrawCheckBox("DeleteCurrentAddObject", deleteCurrentAddObject);


		imguiManager->DrawCheckBox("GetCurrentAddObjectParam", getParam);
	}
	imguiManager->EndDrawWindow();

	// オブジェクトの変更があったらselectObjectを変更
	if (changeObject)
	{
		selectObject.reset();
		selectObject = GetPObject
		(
			objectType,
			objectNum,
			addObjectPos,
			addObjectAngle,
			addObjectScale,
			false
		);
	}

	// 情報のセット
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
		selectObject = GetPObject
		(
			objectType,
			objectNum,
			addObjectPos,
			addObjectAngle,
			addObjectScale,
			false
		);
	}

	// 選択したやつの色変更
	if (preSelectAddObjectNum != selectAddObjectNum)
	{
		pGameObjects[preSelectAddObjectNum]->SetSubColor(MelLib::Color(0, 0));
		pGameObjects[selectAddObjectNum]->SetSubColor(MelLib::Color(150, 0));
	}

	// 追加
	if (MelLib::Input::KeyTrigger(DIK_SPACE))AddObject();

	// 削除
	if (deleteCurrentAddObject)
	{

		pGameObjects[selectAddObjectNum]->TrueEraseManager();

		int count = 0;
		for (auto& enemy : *pEnemys)
		{
			if (enemy == pGameObjects[selectAddObjectNum])
			{
				pEnemys->erase(pEnemys->begin() + count);
				break;
			}
			count++;
		}

		count = 0;
		for(auto& wall:*pWalls)
		{
			if (wall == pGameObjects[selectAddObjectNum])
			{
				pWalls->erase(pWalls->begin() + count);
				break;
			}
			count++;
		}

		pGameObjects.erase(pGameObjects.begin() + selectAddObjectNum);
		objectTypes.erase(objectTypes.begin() + selectAddObjectNum );
		objectNums.erase(objectNums.begin() + selectAddObjectNum);
		addObjectPositions.erase(addObjectPositions.begin() + selectAddObjectNum );
		addObjectAngles.erase(addObjectAngles.begin() + selectAddObjectNum );
		addObjectScales.erase(addObjectScales.begin() + selectAddObjectNum );
		
		selectAddObjectNum--;

	}

	// 値コピー
	if(getParam)
	{
		objectType = objectTypes[selectAddObjectNum - 1];
		objectNum = objectNums[selectAddObjectNum - 1];

		MelLib::GameObject* pObj = pGameObjects[selectAddObjectNum].get();
		addObjectPos = pObj->GetPosition();
		addObjectAngle = pObj->GetRefModelObjects().at("main").GetAngle();
		addObjectScale = pObj->GetRefModelObjects().at("main").GetScale();

		selectObject.reset();
		selectObject = GetPObject
		(
			objectType,
			objectNum,
			addObjectPos,
			addObjectAngle,
			addObjectScale,
			false
		);
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

std::shared_ptr<MelLib::GameObject> EditMode::GetPObject
(
	int objectType,
	int objectNum,
	const MelLib::Vector3 pos,
	const MelLib::Vector3 angle,
	const MelLib::Vector3 scale,
	bool addObjectArray)
{
	int num = objectType + objectNum;

#pragma region 敵
	if (num == NORMAL_ENEMY)
	{
		std::shared_ptr<NoemalEnemy>p = std::make_shared<NoemalEnemy>(pos,wallNum);
		if (addObjectArray)pEnemys->push_back(p);
		return p;
	}
	else if (num == JUMP_ENEMY)
	{
		std::shared_ptr<JumpEnemy>p = std::make_shared<JumpEnemy>(pos, wallNum);
		if (addObjectArray)pEnemys->push_back(p);
		return p;
	}
#pragma endregion

	switch (num)
	{

#pragma region ステージオブジェクト


	case BAMBOO:
		return std::make_shared<Bamboo>(pos);
		break;

	case ROCK:
		return std::make_shared<Rock>(pos, angle, scale,1);
		break;

	case ROCK_2:
		return std::make_shared<Rock>(pos, angle, scale, 2);
		break;

	case ROCK_3:
		return std::make_shared<Rock>(pos, angle, scale, 3);
		break;

	case EVENT_FLAG:
		return std::make_shared<EventFlag>(pos, scale);
		break;

	case TUTORIAL_EVENT_FLAG:
		return std::make_shared<TutorialEventFlag>(pos, scale);
		break;
#pragma endregion

#pragma region 壁床


	case GROUND:
		return std::make_shared<Ground>(pos, angle, scale.ToVector2());
		break;

	case WALL:
		return std::make_shared<Wall>(pos, angle, scale.ToVector2());
		break;

#pragma endregion

	default:
		return nullptr;
		break;
	}
}

void EditMode::AddObject()
{
	std::shared_ptr<MelLib::GameObject> getP = GetPObject
	(
		objectType,
		objectNum,
		addObjectPos,
		addObjectAngle,
		addObjectScale,
		true
	);
	if (!getP)
	{
		return;
	}
	getP->SetPosition(addObjectPos);
	getP->SetAngle(addObjectAngle);

	if (objectType == OBJ_TYPE_FIERD || objectType == OBJ_TYPE_STAGE)
	{
		getP->SetScale(addObjectScale);
	}

	MelLib::GameObjectManager::GetInstance()->AddObject(getP);

	// 追加に成功したら、オブジェクト番号などを格納
	objectTypes.push_back(objectType);
	objectNums.push_back(objectNum);
	pGameObjects.push_back(getP);
	addObjectPositions.push_back(addObjectPos);
	addObjectAngles.push_back(addObjectAngle);
	addObjectScales.push_back(addObjectScale);

	
	// 壁だったら追加
	if(objectType + objectNum == WALL)
	{
		pWalls->push_back(getP);
	}
}


EditMode* EditMode::GetInstance()
{
	static EditMode e;
	return &e;
}