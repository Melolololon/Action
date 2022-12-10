#pragma once
#include "NewEnemy.h"

// Žã‚¢“G
class WeakEnemy :
    public NewEnemy
{
private:
	static const float MOVE_SPEED;

private:

	void Attack();
public:


	WeakEnemy(const MelLib::Vector3& pos);
	void Update()override;
	void Draw()override;

	static float GetMoveSpeed() { return MOVE_SPEED; }
};

