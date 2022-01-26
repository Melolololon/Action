#pragma once
#include "Enemy.h"
#include"NormalEnemyAttack.h"


class JumpEnemy :
    public Enemy
{
private:
	static const int ATTACK_START_TIME = 60 * 0.7;

	MelLib::Vector3 prePos;
	bool attackAddFrame = false;

	// ジャンプ開始フレーム
	static const unsigned int JUMP_START_FRAME = 48;
	bool preIsFall = false;

	std::shared_ptr<NormalEnemyAttack>enemyAttack;

private:
	void Attack();


	void AttackRot();

	void CheckJumpStart();
	void HitGroundUpdate();
public:

	static void LoadResources();

	JumpEnemy(const MelLib::Vector3 pos);

	void Update()override;
	


};

