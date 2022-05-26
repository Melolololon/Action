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
	/// �����������̏���
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
	/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
	/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>
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

