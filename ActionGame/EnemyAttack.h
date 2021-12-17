#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

#include"Player.h"

//�G�̍U��
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
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

