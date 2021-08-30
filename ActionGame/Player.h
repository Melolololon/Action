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

	//�����Ă����
	MelLib::Vector3 playerDir = MelLib::Vector3(0, 0, 1);

#pragma region �U��

	//���݂̍U��
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;
	//���̍U���̓��͎�t�J�n����
	static const int ATTACK_NEXT_TIME = 60 * 0.2f;
	//�U���I������
	static const int ATTACK_END_TIME = 60 * 0.5f;
	
	//�U�����ԃ^�C�}�[
	MelLib::FrameTimer attackTimer;

#pragma endregion

public:
	Player(const MelLib::Vector3& pos);
	~Player(){}

#pragma region Update�֌W
	
	//����s�\����if�ň͂ނ̑�ς�����֐���
	
	void Move();
	void Attack();
	void SetAttackType();
	void Camera();
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
		const MelLib::ShapeType3D collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D hitObjColType,
		const int hitObjArrayNum
	)override;
};

