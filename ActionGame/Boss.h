#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"Player.h"

class Boss:public MelLib::GameObject
{
private:
	static Player* pPlayer;

	// �ړ���U���𐧌䂷��^�C�}�[�B��@�����ɂ��鎞�A���̃^�C�}�[�̎��Ԃɉ����Ĉړ����邩�������U�����邩�����߂�
	MelLib::FrameTimer actionTimer;
	
	enum class ThisState 
	{
		BATTLE,
		OTHER
	};
	ThisState thisState = ThisState::OTHER;

	MelLib::Vector3 direction; 
	int jumpAttackCount = 0;
	static const int JUMP_ATTACK_MAX_COUNT = 8;
	MelLib::FrameTimer jumpAttackTimer;

	enum class CurrentAttack 
	{
		NONE,
		JUMP,
	};
	CurrentAttack currentAttack = CurrentAttack::NONE;

	
	void AttackUpdate();
	// �U���ꗗ
	void JumpAttack();

public:


	static void LoadResources();
	static void SetPlayer();

	Boss();

	std::shared_ptr<MelLib::GameObject> GetNewPtr()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Hit
	(
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;


	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	void Move();


};

