#pragma once
#include <GameObject.h>
#include<RouteSearch.h>
#include<FrameTimer.h>
#include"Player.h"

class Player;

//�G�N���X
class Enemy :
	public MelLib::GameObject
{
private:


	static Player* pPlayer;
	static std::vector<std::vector<std::vector<MelLib::AStarNode>>> nodes;

	// ���ݑI�΂�Ă���i�s����
	int currentMoveVector = 0;
	MelLib::FrameTimer routeSearchTimer;
	std::vector<MelLib::Vector3>routeVectors;
	// �f�o�b�O�p
	MelLib::ModelObject routeObj[100];

	MelLib::Vector3 direction;


	bool isDead = false;

	bool isMuteki = false;
	MelLib::FrameTimer mutekiTimer;
private:
	/// <summary>
	/// �o�H�T���̌��ʂ̉��Z�֐�
	/// </summary>
	void AddRouteVector();

protected:

	unsigned int hp = 0;
	float moveSpeed = 0.0f;

	bool isAttack = false;

	MelLib::FrameTimer attackTimer;
protected:
	/// <summary>
	/// �v���C���[�Ƃ̋������w�肵�����l�������m�F
	/// </summary>
	/// <param name="distance"></param>
	/// <returns></returns>
	bool CheckPlayerDistance(float distance);

	/// <summary>
	/// �v���C���[�֌���������
	/// </summary>
	void CalcPlayerRoute();

	void CheckMutekiEnd();

	void AttackStart();
	void CheckAttackEnd();

	void RotModel();

public:

	Enemy(const MelLib::Vector3& pos, unsigned int hp, float moveSpeed, const std::string& modelName = "");
	virtual ~Enemy() {}
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


	static void SetPPlayer(Player* p) { pPlayer = p; }

	/// <summary>
	/// �o�H�T���ɕK�v�ȏ���n���ăm�[�h�ɏ����Z�b�g����֐�
	/// </summary>
	/// <param name="leftDownFrontPos">�o�H�T���͈̔͂̍�����O���W</param>
	/// <param name="rightUpBackPos">�o�H�T���͈̔͂̉E�㉜���W</param>
	/// <param name="nodeNum">xyz�̃m�[�h��</param>
	/// <param name="datas">�G���ʂ�Ȃ��I�u�W�F�N�g�̔���f�[�^</param>
	/// <param name="costs">�I�u�W�F�N�g�̃R�X�g</param>
	static void SetAStarNodeDatas
	(
		const MelLib::Vector3& leftDownFrontPos,
		const MelLib::Vector3& rightUpBackPos,
		const MelLib::Value3<UINT>& nodeNum,
		const std::vector<MelLib::BoxData>& datas,
		const std::vector<UINT>& costs
	);
};

