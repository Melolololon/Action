#pragma once
#include <GameObject.h>
class Bamboo :
    public MelLib::GameObject
{
private:

public:
	Bamboo(const MelLib::Vector3& pos);
	
	/// <summary>
	/// �J�b�g�����|�p�̃R���X�g���N�^
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="pCatModelData"></param>
	Bamboo(const MelLib::Vector3& pos,MelLib::ModelData* pCatModelData);


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

	static void LoadResources();
};

