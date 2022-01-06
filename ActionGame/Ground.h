#pragma once
#include <GameObject.h>

#include<Material.h>
#include<ModelObject.h>

//地面のモデルはボード
//サイズを受け取る


//テクスチャを受け取る
//テクスチャ受け取ると毎回マテリアル生成することになるから、マテリアル渡す

// 地面はモデルを作って、
// これは判定のみにしてもいいかも

//地面クラス
class Ground :
    public MelLib::GameObject
{
private:
	MelLib::ADSAMaterial testMtl;
	
	static const MelLib::Vector2 GROUND_SIZE;

public:
	Ground(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector2& scale);

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

	static void LoadResource();

};

