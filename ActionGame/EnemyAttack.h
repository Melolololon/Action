#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

#include"Player.h"

//�G�̍U��
class EnemyAttack :
	public MelLib::GameObject
{
public:
	enum class AttackType
	{
		NORMAL,
		BE_BLOWN_AWAY,
	};


private:

	void SetAttackTypeTag(AttackType type);
	void SetInitData(const float radius, AttackType type);

	int timer = 0;
private:
	static const std::string ATTACK_TYPE_STR_NORMAL;
	static const std::string ATTACK_TYPE_STR_BE;
	
	int power = 0;

	// �����Ⴒ���Ⴗ�邩��N���X�����Ă��������� AttackType�̓w�b�_�[�p�ӂ��Ă����Ő錾���悤
	enum class MoveType 
	{
		OBJECT,// �I�u�W�F�N�g�ɒǔ�
		ANIMATION// �A�j���[�V�����ɍ��킹��
	};
	const MoveType MOVE_TYPE = MoveType::OBJECT;

	const MelLib::ModelObject& MODEL;

	const MelLib::Vector3 SPHERE_START_POSITION;
	const MelLib::Vector3 MODEL_START_POS;
	const MelLib::Vector3 MODEL_START_ANGLE;
	const MelLib::Vector3 MODEL_START_SCALE;

	const unsigned int DELETE_FRAME;
	

	static Player* pPlayer;

public:
	
	static AttackType GetAttackType(const std::string& tag);
	static std::string GetAttackTypeStr(const AttackType type);
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
		const MelLib::ModelObject& model,
		const MelLib::Vector3& modelStartPos,
		const MelLib::Vector3& modelStartAngle,
		const MelLib::Vector3& modelStartScale,
		const AttackType attackType,
		const unsigned int deleteFrame
	);

	EnemyAttack
	(
		unsigned int power,
		const MelLib::ModelObject& model,
		float radius,
		const AttackType attackType,
		const unsigned int deleteFrame
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
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

