#pragma once
#include "Enemy.h"
class JumpEnemy :
    public Enemy
{
private:
	static const int ATTACK_START_TIME = 60 * 0.7;

	MelLib::Vector3 prePos;
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

