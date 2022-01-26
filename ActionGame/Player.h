#pragma once
#include"GameObject.h"

#include<FrameTimer.h>
#include<Easing.h>

#include"PlayerSlush.h"
#include"Input.h"
#include"Enemy.h"

//�U�����ԏI��������ǂ������肩��󂯎���ăv���C���[���䂷��?
//�U������̐������ԂƍU�����Ԃ͈Ⴄ����Ƃ肠�������̂܂܂ł���?
//�Ƃ肠�����U�����Ԃ͋Z���Ƃɒ�`�����ق����悳����

class Enemy;

class Player :public MelLib::GameObject
{
public:
	enum class ActionType
	{
		JUMP,
		ATTACK,
		DASH,
		GUARD,
	};

private:


	static std::unordered_map<ActionType, MelLib::PadButton> keyConfigData;

	//�O�t���[���̍��W���i�[����ϐ�
	MelLib::Vector3 prePosition;
	//�����Ă����
	MelLib::Vector3 direction = MelLib::Vector3(0, 0, 1);

	static const unsigned int HP_MAX = 150;
	unsigned int hp = HP_MAX;
	bool hitGround = false;
	bool preHitGround = false;
	bool jumpResetAnimation = false;

#pragma region �ړ�
	float moveSpeed = 0.5f;

	// �������ĂȂ����̐��t���[���O�̍��W���i�[����z��(�������ɖ߂����߂Ɏg�p)
	MelLib::Vector3 notFallPrePosition[5];

#pragma region �_�b�V��
	//�V����!���Ċ����œ������
	//���[�i���X�e�b�v�A�M���J�O���̂���
	bool isDash = false;

	MelLib::Easing<MelLib::Vector3> dashEasing;
	//�_�b�V���̑�����
	MelLib::Vector3 dashAddValue = 0.0f;
#pragma endregion

#pragma endregion

#pragma region �W�����v
	static const float JUMP_POWER;

	bool isDrop = false;
	float dropStartPosY = 0.0f;

	// �W�����v�J�n�ʒu
	MelLib::Vector3 fallStartPosition;
#pragma endregion

#pragma region �U��

	struct AttackData
	{
		int power = 0;
		int time = 0;


		//�L�����Z���\�ɂȂ�܂ł̎���
		//���݂͍U������̂��鎞�Ԃɂ��Ȃ��Ă��邪�A�ς���\������
		//���̏ꍇ�A����̂��鎞�Ԃ𔻒莩�g�Ɏ������A�������Ȃ���
		// ���̍U���Ɉڂ��悤�ɂȂ�܂ł̎���
		int nextTime = 0;

		AttackData() {}
		AttackData(const int power, const int time, const int nextTime) :
			power(power),
			time(time),
			nextTime(nextTime) {}

	};

	std::shared_ptr<PlayerSlush>pPSlush = nullptr;
	std::shared_ptr<PlayerSlush>pRigthSlush = nullptr;
	//���݂̍U��
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;

	//�U�����ԃ^�C�}�[
	MelLib::FrameTimer attackTimer;


	// static const���Ɨv�f�ɃA�N�Z�X�ł��Ȃ��̂ŁAconst AttackData�ɂ��Ă�
	// �U���̏����܂Ƃ߂�����
	static std::unordered_map<PlayerSlush::AttackType, const AttackData>attackData;

	bool setStartParam = false;
	MelLib::Vector3 startPos;
	MelLib::Vector3 startScale;
	MelLib::Vector3 startAngle;
#pragma endregion

#pragma region �h��
	bool isGuard = false;

#pragma endregion

#pragma region ���G

	bool isMuteki = false;
	MelLib::FrameTimer mutekiTimer;
#pragma endregion

	// �U�����󂯂čd�����Ă��邶��ǂ���
	bool isStun = false;

	//�C�x���g�t���O�ɏՓ˂������ǂ���
	bool hitEventFlag = false;


	// �Ⴂ�Ƃ���́A���n�A�j���[�V�������W�����v�̋t�Đ��ɂ��A�d���͂Ȃ�
	// �n�ʂɒ��n�������̓����Ȃ�����
	MelLib::FrameTimer hitGroundNotMoveTimer;
	bool hitGroundNotMove = false;

#pragma region �J����
	float cameraSpeed = 0.0f;

	bool lockOn = false;
	const MelLib::GameObject* lockOnEnemy = nullptr;
	float lockOnEnemyDistance = FLT_MAX;
#pragma endregion

	// �f�o�b�O�p(true�ɂ���Ƌ���T�|�[�Y)
	bool isTPause = false;



private:

#pragma region Update�֌W


	void ChangeAnimationData();

	//����s�\����if�ň͂ނ̑�ς�����֐���


	void Move();

	/// <summary>
	/// �������ɃX�e�[�W�ɖ߂�����
	/// </summary>
	void ReturnStage();

	/// <summary>
	/// �U�����̈ړ�
	/// </summary>
	void AttackMove();

	void Dash();
	void DashEnd();

	void Jump();
	void JumpAnimation();

	//�U�������A�U������̑���
	void Attack();
	void SetAttackType();
	void CreateAttackSlush();

	void Guard();

	void Muteki();

	// �U���d��
	void Stun();

	// �J������]����
	void RotCamera();
	// �J�����ɒl�Z�b�g
	void SetCameraData();

	// ���b�N�I��
	void LockOn();

#pragma endregion

public:
	static void LoadResources();

	Player(const MelLib::Vector3& pos);
	~Player() {}

	PlayerSlush* GetPPlayerSlush() { return pPSlush.get(); }

	void Update()override;
	void Draw()override;
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


	void DownHP(const unsigned int power) { hp -= power; }
	void LifeUp(const unsigned int upNum);
#pragma region Get
	static const std::unordered_map<Player::ActionType, MelLib::PadButton> GetKeyConfigData() { return keyConfigData; }

	unsigned int GetCurrentAttackPower()const { return attackData[currentAttack].power; }

	MelLib::Vector3 GetDirection() { return direction; }

	const MelLib::GameObject* GetPLockOnEnemy()const { return lockOnEnemy; }
	
	unsigned int GetHP()const { return hp; }
	unsigned int GetHPMax()const { return HP_MAX; }

	bool GetIsMuteki()const { return isMuteki; }

	bool GetHitEventFlag()const { return hitEventFlag; }
#pragma endregion

#pragma region Set

	static void SetKeyConfigData(const ActionType type, const MelLib::PadButton button) { keyConfigData[type] = button; }

	/// <summary>
	/// ���b�N�I�������I��
	/// </summary>
	void LockOnEnd() 
	{ 
		lockOnEnemy = nullptr;
		lockOn = false;
	}
#pragma endregion

	void TitleUpdate();

};

