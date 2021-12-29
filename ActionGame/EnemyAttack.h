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

	const MelLib::Vector3 SPHERE_START_POSITION;
	const MelLib::Vector3 MODEL_START_POS;
	const MelLib::Vector3 MODEL_START_ANGLE;
	const MelLib::Vector3 MODEL_START_SCALE;

	static Player* pPlayer;

public:

	// startPos�������œn���悤�ɂ����A�N���X���킽���Ă������ɒ�`�������W�������Ă���Ȃ�static�֐��Ŏ擾���Ă�����?
	/// <summary>
	/// 
	/// </summary>
	/// <param name="power">�p���[</param>
	/// <param name="pos">����U�����ŃZ�b�g���邩���������</param>
	/// <param name="radius">���a</param>
	/// <param name="deadTime">�����鎞��</param>
	/// <param name="model">�G�̃��f��(�A�j���[�V�����̏�Ԃ��擾����̂Ɏg�p)</param>
	/// <param name="startPos">�U���̔�����Z�b�g�����Ƃ��̍��W(�G���ƂɈʒu���Ⴄ�̂œn��)</param>
	/// <param name="startAngle"></param>
	/// <param name="startScale"></param>
	EnemyAttack
	(
		unsigned int power,
		const MelLib::Vector3& attackStartPos,
		float radius,
		int deadTime,
		const MelLib::ModelObject& model,
		const MelLib::Vector3& modelStartPos,
		const MelLib::Vector3& modelStartAngle,
		const MelLib::Vector3& modelStartScale
	);
	~EnemyAttack() {}

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

