#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

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
	MelLib::Vector3 playerDir = MelLib::Vector3(0, 0, 1);

	static const unsigned int HP_MAX = 200;
	unsigned int hp = HP_MAX;
	bool hitGround = false;

#pragma region �ړ�
	float speed = 0.5f;

#pragma region �_�b�V��
	//�V����!���Ċ����œ������
	//���[�i���X�e�b�v�A�M���J�O���̂���
	bool isDash = false;
	MelLib::FrameTimer dashTimer;

#pragma endregion

#pragma endregion


#pragma region �U��

	std::shared_ptr<PlayerSlush>pPSlush = nullptr;
	//���݂̍U��
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;
	
	//�U�����ԃ^�C�}�[
	MelLib::FrameTimer attackTimer;

	//�U���̎���
	static std::unordered_map<PlayerSlush::AttackType, const int>attackTime;

	//�L�����Z���\�ɂȂ�܂ł̎���
	//���݂͍U������̂��鎞�Ԃɂ��Ȃ��Ă��邪�A�ς���\������
	//���̏ꍇ�A����̂��鎞�Ԃ𔻒莩�g�Ɏ������A�������Ȃ���
	static  std::unordered_map<PlayerSlush::AttackType, const int>nextAttackTime;
#pragma endregion

private:


public:
	Player(const MelLib::Vector3& pos);
	~Player(){}

#pragma region Update�֌W
	
	//����s�\����if�ň͂ނ̑�ς�����֐���
	
	
	void Move();
	void Dash();
	void Jump();

	//�U�������A�U������̑���
	void Attack();
	void SetAttackType();
	
	void Camera();
	void SetCameraPosition();
#pragma endregion

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


};

