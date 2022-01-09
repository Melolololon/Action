#pragma once

#include<string>

#include<Vector.h>

#include"Player.h"
#include"Enemy.h"

// シーン内で呼び出すエディットモード
class EditMode final
{
private:
	Player* pPlayer = nullptr;
	
	bool editPossible = true;
	bool isEdit = false;

	
	std::shared_ptr<MelLib::GameObject> selectObject;
	int objectType = EditMode::OBJ_TYPE_ENEMY;
	int objectNum = 0;
	MelLib::Vector3 addObjectPos;
	MelLib::Vector3 addObjectAngle;
	MelLib::Vector3 addObjectScale;

#pragma region エディットモードで配置するときの識別番号


	std::vector<int>objectTypes;
	std::vector<int>objectNums;
	std::vector<MelLib::GameObject*>pGameObjects;

	int currentAddObject = 0;

	using ObjID = const int;

	static ObjID OBJ_TYPE_ENEMY = 0;
	static ObjID OBJ_TYPE_STAGE = 1000;
	static ObjID OBJ_TYPE_FIERD = 2000;

#pragma region OBJ_TYPE_ENEMY
	static ObjID NORMAL_ENEMY = OBJ_TYPE_ENEMY + 0;

#pragma endregion

#pragma region OBJ_TYPE_STAGE
	static ObjID BAMBOO = OBJ_TYPE_STAGE + 0;
#pragma endregion

#pragma region OBJ_TYPE_FIERD
	static ObjID GROUND = OBJ_TYPE_FIERD + 0;
	static ObjID WALL = OBJ_TYPE_FIERD + 1;
#pragma endregion



#pragma endregion


	std::vector<std::shared_ptr<Enemy>>* pEnemys = nullptr;
private:
	EditMode(){}
	~EditMode(){}

	std::string GetFileName();

	/// <summary>
	///GameObjectの読み込み
	/// </summary>
	void Save();

	void SetSelectObject();
	void AddObject();

	void AddEnemy(std::shared_ptr<Enemy>p);
public:

	EditMode(EditMode& e) = delete;
	EditMode& operator=(EditMode& e) = delete;
	static EditMode* GetInstance();
	
	/// <summary>
	/// GameObjectの読み込み
	/// </summary>
	/// <param name="p">プレイヤーのポインタ</param>
	/// <returns>読み込みが成功したかどうか</returns>
	bool Load(std::shared_ptr<Player>& p, std::vector<std::shared_ptr<Enemy>>* pEnemys);

	void Update();
	void Draw();

	bool GetIsEdit() { return isEdit; }

	void SetPPlayer(Player* p) { pPlayer = p; }

};

