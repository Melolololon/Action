#pragma once
#include"GameObject.h"

#include"FrameTimer.h"


//����������A�̔���ł���������

//�v���C���[�̍U������
class PlayerSlush :public MelLib::GameObject
{
public:
	enum class AttackType
	{
		NONE,
		NORMAL_1,//�ʏ�(X�{�^��)�U��1�Ԗ�
		NORMAL_2,//�ʏ�(X�{�^��)�U��2�Ԗ�
		NORMAL_3,//�ʏ�(X�{�^��)�U��3�Ԗ�
		DASH_1,// �_�b�V����1�Ԗ�
	};

private:
	


	float playerAngle = 0.0f;
private:
	MelLib::FrameTimer eraseTimer;
	AttackType attackType = AttackType::NORMAL_1;
private:
	void SetAttackParam();

	void Attack();
public:


	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="playerDir">�v���C��[�̌���</param>
	PlayerSlush(const MelLib::Vector3& pos,const MelLib::Vector3& playerDir,const AttackType type,const int attackTime);

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

