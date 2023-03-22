#pragma once
#include <GameObject.h>
#include"Player.h"

class CapsuleEnemyAttack :
	public MelLib::GameObject
{
private:
	int power = 0;


	const MelLib::ModelObject& MODEL;

	const MelLib::Value2<MelLib::Vector3> CAPSULE_START_POSITION;
	const MelLib::Vector3 MODEL_START_POS;
	const MelLib::Vector3 MODEL_START_ANGLE;
	const MelLib::Vector3 MODEL_START_SCALE;
	const std::string BONE_NAME;
	const std::string MESH_NAME;

	static Player* pPlayer;

public:

	// startPos�������œn���悤�ɂ����A�N���X���킽���Ă������ɒ�`�������W�������Ă���Ȃ�static�֐��Ŏ擾���Ă�����?
	/// <summary>
	/// 
	/// </summary>
	/// <param name="power">�p���[</param>
	/// <param name="pos">�����蔻��̈ʒu</param>
	/// <param name="radius">���a</param>
	/// <param name="deadTime">�����鎞��</param>
	/// <param name="model">�G�̃��f��(�A�j���[�V�����̏�Ԃ��擾����̂Ɏg�p)</param>
	/// <param name="startPos">�����蔻����Z�b�g�����Ƃ��̃��f���̍��W(�G���ƂɈʒu���Ⴄ�̂œn��)</param>
	/// <param name="startAngle">�����蔻����Z�b�g�����Ƃ��̃��f���̊p�x</param>
	/// <param name="startScale">�����蔻����Z�b�g�����Ƃ��̃��f���̑傫��</param>
	CapsuleEnemyAttack
	(
		unsigned int power,
		const MelLib::Value2<MelLib::Vector3>& attackStartPos,
		float radius,
		const MelLib::ModelObject& model,
		const MelLib::Vector3& modelStartPos,
		const MelLib::Vector3& modelStartAngle,
		const MelLib::Vector3& modelStartScale,
		const std::string& boneName,
		const std::string& meshName
	);
	~CapsuleEnemyAttack() {}

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
		const GameObject& object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

