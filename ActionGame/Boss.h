#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"Player.h"

class Boss:public MelLib::GameObject
{
private:
	static Player* pPlayer;

	// �ړ���U���𐧌䂷��^�C�}�[�B��@�����ɂ��鎞�A���̃^�C�}�[�̎��Ԃɉ����Ĉړ����邩�������U�����邩�����߂�
	MelLib::FrameTimer actionTimer;
public:


	static void LoadResources();
	static void SetPlayer();

	Boss(const MelLib::Vector3 pos);

	void Update()override;
	void Draw()override;
	void Hit
	(
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;


	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	void Move();
};

