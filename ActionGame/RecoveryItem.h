#pragma once
#include "GameItem.h"

class RecoveryItem :
    public GameItem
{
private:
    static const unsigned int RECOVERY_HP = 20;
public:
	RecoveryItem(const MelLib::Vector3& pos);
	~RecoveryItem() {}

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


};

