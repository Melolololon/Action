#pragma once
#include"GameObject.h"

#include<FrameTimer.h>
#include<Easing.h>

#include"PlayerSlush.h"


//�U�����ԏI��������ǂ������肩��󂯎���ăv���C���[���䂷��?
//�U������̐������ԂƍU�����Ԃ͈Ⴄ����Ƃ肠�������̂܂܂ł���?
//�Ƃ肠�����U�����Ԃ͋Z���Ƃɒ�`�����ق����悳����

class Player :public MelLib::GameObject
{
private:
	
	//�O�t���[���̍��W���i�[����ϐ�
	MelLib::Vector3 prePos;
	//�����Ă����
	MelLib::Vector3 direction = MelLib::Vector3(0, 0, 1);

	static const unsigned int HP_MAX = 200;
	unsigned int hp = HP_MAX;
	bool hitGround = false;

#pragma region �ړ�
	float moveSpeed = 0.5f;

#pragma region �_�b�V��
	//�V����!���Ċ����œ������
	//���[�i���X�e�b�v�A�M���J�O���̂���
	bool isDash = false;

	MelLib::Easing<MelLib::Vector3> dashEasing;
	//�_�b�V���̑�����
	MelLib::Vector3 dashAddValue = 0.0f;
#pragma endregion

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

		AttackData(){}
		AttackData(const int power, const int time, const int nextTime):
			power(power),
			time(time),
			nextTime(nextTime){}

	};

	std::shared_ptr<PlayerSlush>pPSlush = nullptr;
	std::shared_ptr<PlayerSlush>pLeftSlush = nullptr;
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


#pragma region �J����
	float cameraSpeed = 0.0f;
#pragma endregion

	// �f�o�b�O�p
	bool isTPause = false;

private:

#pragma region Update�֌W

	void ChangeAnimationData();

	//����s�\����if�ň͂ނ̑�ς�����֐���


	void Move();
	/// <summary>
	/// �U�����̈ړ�
	/// </summary>
	void AttackMove();

	void Dash();
	void Jump();

	//�U�������A�U������̑���
	void Attack();
	void SetAttackType();

	/// <summary>
	/// �v���C���[�̍U���ɍ��W��n���֐�
	/// </summary>
	void SetAttackData();

	void Camera();
	void SetCameraPosition();
#pragma endregion

public:
	static void LoadResources();

	Player(const MelLib::Vector3& pos);
	~Player(){}

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

#pragma region Get
	unsigned int GetCurrentAttackPower()const { return attackData[currentAttack].power; }

	MelLib::Vector3 GetDirection() { return direction; }
#pragma endregion

};

