#pragma once
#include "Enemy.h"
class NoemalEnemy :
	public Enemy
{
private:
	static const int ATTACK_START_TIME = 60 * 0.7;


public:

	static void LoadResource();

	NoemalEnemy(const MelLib::Vector3& pos);
	~NoemalEnemy() {}

	void Update()override;
	/*/// <summary>
	/// �����������̏���
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
	/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
	/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>
	void Hit
	(
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;*/

	//void Attack();
};

