#pragma once
#include"GameObject.h"

#include"ModelData.h"


class Floor :
    public MelLib::GameObject
{

public:

	//���ɂ���đ����ς��邽�߂ɁASoundData���n����悤�ɂ���?
	Floor
	(
		const MelLib::Vector3& pos, 
		const MelLib::Vector3 size, 
		const MelLib::ModelData* pModelData
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
		const GameObject* const  object,
		const MelLib::ShapeType3D collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D hitObjColType,
		const int hitObjArrayNum
	)override;
};

