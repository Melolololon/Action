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
	
	bool editPossible = false;
	bool isEdit = false;

	
	std::shared_ptr<MelLib::GameObject> selectObject;
	int objectType = EditMode::OBJ_TYPE_ENEMY;
	int objectNum = 0;
	MelLib::Vector3 addObjectPos;
	MelLib::Vector3 addObjectAngle;
	MelLib::Vector3 addObjectScale;


	std::vector<int>objectTypes;
	std::vector<int>objectNums;
	std::vector<std::shared_ptr<MelLib::GameObject>>pGameObjects;
	std::vector<MelLib::Vector3>addObjectPositions;
	std::vector<MelLib::Vector3>addObjectAngles;
	std::vector<MelLib::Vector3>addObjectScales;

	// 敵と壁を紐付けるための番号(例えば1が割り当てられた敵が全滅すると、1に壁が消える)
	int wallNum = 0;

	// 現在選択されているシーンに配置されたオブジェクトの番号
	int selectAddObjectNum = 1;
	int preSelectAddObjectNum = 1;

#pragma region エディットモードで配置するときの識別番号


	using ObjID = const int;

	static ObjID OBJ_TYPE_ENEMY = 0;
	static ObjID OBJ_TYPE_STAGE = 1000;
	static ObjID OBJ_TYPE_FIERD = 2000;

#pragma region OBJ_TYPE_ENEMY
	static ObjID NORMAL_ENEMY = OBJ_TYPE_ENEMY + 0;
	static ObjID JUMP_ENEMY = OBJ_TYPE_ENEMY + 1;

#pragma endregion

#pragma region OBJ_TYPE_STAGE
	static ObjID BAMBOO = OBJ_TYPE_STAGE + 0;
	static ObjID ROCK = OBJ_TYPE_STAGE + 1;
	static ObjID ROCK_2 = OBJ_TYPE_STAGE + 2;
	static ObjID ROCK_3 = OBJ_TYPE_STAGE + 3;

	static ObjID EVENT_FLAG = OBJ_TYPE_STAGE + 4;
	static ObjID TUTORIAL_EVENT_FLAG = OBJ_TYPE_STAGE + 5;
#pragma endregion

#pragma region OBJ_TYPE_FIERD
	static ObjID GROUND = OBJ_TYPE_FIERD + 92929;
	static ObjID WALL = OBJ_TYPE_FIERD + 1;
#pragma endregion



#pragma endregion


	std::vector<std::shared_ptr<Enemy>>* pEnemys = nullptr;
	std::vector<std::shared_ptr<MelLib::GameObject>>* pWalls = nullptr;
private:
	EditMode(){}
	~EditMode(){}

	std::string GetFileName();

	/// <summary>
	///GameObjectの読み込み
	/// </summary>
	void Save();

	std::shared_ptr<MelLib::GameObject> GetPObject
	(
		int objectType,
		int objectNum,
		const MelLib::Vector3 pos,
		const MelLib::Vector3 angle,
		const MelLib::Vector3 scale,
		bool addObjectArray
	);
	void AddObject();

public:

	EditMode(EditMode& e) = delete;
	EditMode& operator=(EditMode& e) = delete;
	static EditMode* GetInstance();
	
	/// <summary>
	/// GameObjectの読み込み
	/// </summary>
	/// <param name="p">プレイヤーのポインタ</param>
	/// <returns>読み込みが成功したかどうか</returns>
	bool Load
	(
		std::shared_ptr<Player>& p, 
		std::vector<std::shared_ptr<Enemy>>* pEnemys,
		std::vector<std::shared_ptr<MelLib::GameObject>>* pWalls);

	void Update();
	void Draw();

	bool GetIsEdit() { return isEdit; }

	void SetPPlayer(Player* p) { pPlayer = p; }

};

