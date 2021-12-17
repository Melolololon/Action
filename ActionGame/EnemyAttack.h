#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

#include"Player.h"

//敵の攻撃
class EnemyAttack :
    public MelLib::GameObject
{
private:
	int power;

	MelLib::FrameTimer deadTimer;


	const MelLib::ModelObject& MODEL;

	const MelLib::Vector3 START_POS;
	const MelLib::Vector3 START_ANGLE;
	const MelLib::Vector3 START_SCALE;

	static Player* pPlayer;

public:
	EnemyAttack
	(
		unsigned int power,
		const MelLib::Vector3& pos,
		float radius,
		int deadTime,
		const MelLib::ModelObject& model,
		const MelLib::Vector3& startPos,
		const MelLib::Vector3& startAngle, 
		const MelLib::Vector3& startScale
	);
	~EnemyAttack(){}

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
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

