#pragma once
#include "NewEnemy.h"

// �ア�G
class WeakEnemy :
    public NewEnemy
{
private:

	int timer = 0;
	MelLib::Vector3 prePos;

	bool hitGround = false;


private:

	/// <summary>
	/// �U���̓����蔻��̒ǉ�
	/// </summary>
	void AddAttackCollisionDetection();

	void Rotate();


public:

	static void LoadResources();

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

	void SetStartAttackAnimation()override;

	void Attack();


};
