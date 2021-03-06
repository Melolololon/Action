#pragma once
#include"GameObject.h"

#include"ModelData.h"


class Floor :
    public MelLib::GameObject
{

public:

	//床によって足音変えるために、SoundDataも渡せるようにする?
	Floor
	(
		const MelLib::Vector3& pos, 
		const MelLib::Vector3 size, 
		const MelLib::ModelData* pModelData
	);

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
};

