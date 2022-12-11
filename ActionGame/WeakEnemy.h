#pragma once
#include "NewEnemy.h"

// Žã‚¢“G
class WeakEnemy :
    public NewEnemy
{
private:
	static const float MOVE_SPEED;

	int timer = 0;
private:




public:


	WeakEnemy(const MelLib::Vector3& pos);
	void Initialize()override;
	void Update()override;
	void Draw()override; 
    void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static float GetMoveSpeed() { return MOVE_SPEED; }

	void Attack();
};
