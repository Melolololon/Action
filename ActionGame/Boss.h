#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"Player.h"
#include"EnemyHPGauge.h"

#include"EnemyAttack.h"

#include"SandEffect.h"


// �����X�y�[�X
// ���݂̃{�X�̍U���p�^�[��
// ��]�@�n�ʒ@�����

// �ǉ���
// �W�����v����̗����U�艺�낵(�J�������l�b�N�H)
// �\���A���ڋ߁A�n���}�[�U��

// �U���̑I�ʊ���ǂ����邩
// ����?

class Boss :public MelLib::GameObject
{
private:

	static Player* pPlayer;
	static Boss* pBoss;

	// �ړ���U���𐧌䂷��^�C�}�[�B��@�����ɂ��鎞�A���̃^�C�}�[�̎��Ԃɉ����Ĉړ����邩�������U�����邩�����߂�
	MelLib::FrameTimer actionTimer;
	
	enum class ThisState 
	{
		BATTLE,
		OTHER
	};
	ThisState thisState = ThisState::OTHER;

	MelLib::Vector3 jumpAttackDir; 
	int jumpAttackCount = 0;
	static const int JUMP_ATTACK_MAX_COUNT = 12;
	MelLib::FrameTimer jumpAttackTimer;

	enum class CurrentState : unsigned int
	{
		NONE,
		NORMAL_1,
		ROLL_ATTACK,

		// �������牓����
		JUMP_ATTACK = 10,
		DASH_ATTACK,

	};
	// �����ŃW�����v�U�����w�肷��Ƃ��ɉ��Z����l
	const int JUMP_ATTACK_ADD_NUM = static_cast<std::underlying_type_t<Boss::CurrentState>>(Boss::CurrentState::JUMP_ATTACK);
	CurrentState currentState = CurrentState::NONE;

	// �U������p�^�C�}�[
	MelLib::FrameTimer attackControlTimer;

	MelLib::Vector3 dashAttackEndPos;
	bool dashAttackGoalSet = false;
	// �_�b�V���U���̃_�b�V�������ǂ���
	bool dashAttackDash = false;

	MelLib::GuiInt hp;
	MelLib::FrameTimer mutekiTimer;
	bool isMuteki = false;
	std::unique_ptr<EnemyHPGauge> hpGauge;

	// �p�[�e�B�N��
	SandEffect sandEffect;
	MelLib::Emitter3D sandParEmitter;

private:

	void Move();

	void SelectAction();
	void AttackUpdate();

	void AttackEnd();

	// �U������ǉ�
	void AddCupsuleAttack(const unsigned int deleteFrame,
		EnemyAttack::AttackType attackType);
	
	// �U���ꗗ
	void NormalAttackUpdate();
	void RollAttackUpdate();
	void JumpAttackUpdate();
	void DashAttackUpdate();

	void Rotate();

	void MoveToPlayer(const float speed);
public:


	static void LoadResources();
	static void SetPPlayer(Player* p) { pPlayer = p; }

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

	static bool CheckStateBattle() { return pBoss->thisState == ThisState::BATTLE; }



};

