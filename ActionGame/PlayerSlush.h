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
		JUMP,
		DEATH_BLOW_1,// �K�E�Z1
		DEATH_BLOW_2,
		DEATH_BLOW_3,
	};

private:
	// T�|�[�Y���̔���̈ʒu
	static const MelLib::Value2<MelLib::Vector3>CAPSULE_START_POS_LEFT;
	static const MelLib::Value2<MelLib::Vector3>CAPSULE_START_POS_RIGTH;


	// �������畽�ʏ��擾���l���Ă݂Ė������������`
	//static const std::unordered_map<AttackType, MelLib::PlaneData> attackPlane;


	float playerAngle = 0.0f;

	// �O�t���[���̐����̍��W
	MelLib::Value2<MelLib::Vector3>preSegmentPosition;

	const MelLib::ModelObject& PLAYER_MODEL;
	const MelLib::Vector3 PLAYER_DIRECTION;

	const MelLib::Vector3 PLAYER_START_POS;
	const MelLib::Vector3 PLAYER_START_ANGLE;
	const MelLib::Vector3 PLAYER_START_SCALE;

	const std::string ATTACK_NAME;
private:
	MelLib::FrameTimer eraseTimer;
	AttackType attackType = AttackType::NORMAL_1;

	bool slushLeft = false;

private:
	void SetAttackParam();

	void Attack();

	void AddEnemyParticle();
public:


	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="playerDir">�v���C��[�̌���</param>
	PlayerSlush
	(
		const MelLib::Vector3& pos,
		const MelLib::Vector3& playerDir,
		const AttackType type,
		const int attackTime,
		const MelLib::ModelObject& model,
		const MelLib::Vector3 playerStartPos,
		const MelLib::Vector3 playerStartAngle,
		const MelLib::Vector3 playerStartScale,
		bool slushLeft,
		const std::string& attackName
	);

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
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	void Erase() { eraseManager = true; }
#pragma region �Q�b�g
	//return capsuleDatas["main"][0].GetSegment3DData().GetPosition(); 
	MelLib::Value2<MelLib::Vector3>GetSegmentPosition()const { return capsuleDatas.at("main")[0].GetSegment3DData().GetPosition(); }
	MelLib::Value2<MelLib::Vector3>GetSegmentPrePosition()const { return preSegmentPosition; }

#pragma endregion

	

};

