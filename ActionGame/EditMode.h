#pragma once
#include<Vector.h>

// シーン内で呼び出すエディットモード
class EditMode
{
private:
	// ファイルの末尾に追加するパス
	const std::string ADD_PATH = "_EditData.edit";

	int objectType = 0;
	int objectNum = 0;
	MelLib::Vector3 addObjectPos;
	MelLib::Vector3 addObjectAngle;
	MelLib::Vector3 addObjectScale;

#pragma region エディットモードで配置するときの識別番号


	std::vector<int>objectTypes;
	std::vector<int>objectNums;

	using ObjID = const int;

	static ObjID OBJ_TYPE_PLAYER = 1000;
	static ObjID OBJ_TYPE_ENEMY = 2000;
	static ObjID OBJ_TYPE_STAGE = 3000;
	static ObjID OBJ_TYPE_FIERD = 4000;

#pragma region OBJ_TYPE_PLAYER
	static ObjID PLAYER = OBJ_TYPE_PLAYER + 0;
#pragma endregion

#pragma region OBJ_TYPE_ENEMY
	static ObjID NORMAL_ENEMY = OBJ_TYPE_ENEMY + 0;

#pragma endregion

#pragma region OBJ_TYPE_STAGE

#pragma endregion

#pragma region OBJ_TYPE_FIERD
	static ObjID GROUND = OBJ_TYPE_FIERD + 0;
#pragma endregion



#pragma endregion



private:
	EditMode(){}
	~EditMode(){}

	void Save();
public:

	EditMode(EditMode& e) = delete;
	EditMode& operator=(EditMode& e) = delete;
	static EditMode* GetInstance() 
	{
		static EditMode e; 
		return &e; 
	}
	

	void Load();

	void Update();
	void Draw();
	void AddObject();
};

