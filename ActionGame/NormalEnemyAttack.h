#pragma once
#include<GameObject.h>
#include<FrameTimer.h>

#include"Player.h"

// アニメーションに依存しない攻撃
class NormalEnemyAttack :public MelLib::GameObject
{
private:
	static Player* pPlayer;

	int power;
	const MelLib::ModelObject& MODEL;
	MelLib::FrameTimer deadTimer;
public:
	NormalEnemyAttack
	(
		unsigned int power,
		const MelLib::Vector3& attackStartPos,
		float radius,
		int deadTime,
		const MelLib::ModelObject& model
	);

	void Update()override;

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

