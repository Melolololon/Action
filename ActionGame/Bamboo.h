#pragma once
#include"StageObject.h"
class Bamboo :
    public StageObject
{
private:

public:
	Bamboo(const MelLib::Vector3& pos);
	
	/// <summary>
	/// カットした竹用のコンストラクタ
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="pCatModelData"></param>
	Bamboo(const MelLib::Vector3& pos,MelLib::ModelData* pCatModelData);


	void Update()override;
	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
	/// <param name="hitObjColType">相手のどの判定に当たったか</param>
	/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>
	void Hit
	(
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static void LoadResources();
};

