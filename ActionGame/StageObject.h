#pragma once
#include<GameObject.h>

#include"Player.h"

class StageObject :public MelLib::GameObject
{
private:
	static Player* pPlayer;

private:
	// プレイヤーの攻撃情報の変換
	MelLib::PlaneData SegmentToPlane();

protected:
	/// <summary>
	/// プレイヤーの攻撃と当たったら切断処理をします。
	/// </summary>
	/// <param name="object"></param>
	/// <param name="pFront"></param>
	/// <param name="pBack"></param>
	void MeshCat(const MelLib::GameObject* const object, MelLib::ModelData*& pFront, MelLib::ModelData*& pBack);

public:
	StageObject(const MelLib::Vector3& pos, const std::string& modelName);
	StageObject(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData);
	~StageObject();

	void Update()override;
	void Draw()override;
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
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

