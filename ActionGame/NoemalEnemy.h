#pragma once
#include "Enemy.h"
class NoemalEnemy :
	public Enemy
{
private:
	static const int ATTACK_START_TIME = 60 * 0.7;


public:

	static void LoadResource();

	NoemalEnemy(const MelLib::Vector3& pos ,int wallNum);
	~NoemalEnemy() {}

	void Update()override;
	/*/// <summary>
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
	)override;*/

	//void Attack();
};

