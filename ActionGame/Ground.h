#pragma once
#include <GameObject.h>

#include<Material.h>
#include<ModelObject.h>

//�n�ʂ̃��f���̓{�[�h
//�T�C�Y���󂯎��


//�e�N�X�`�����󂯎��
//�e�N�X�`���󂯎��Ɩ���}�e���A���������邱�ƂɂȂ邩��A�}�e���A���n��

//�n�ʃN���X
class Ground :
    public MelLib::GameObject
{
private:
	MelLib::Material* pMaterial = nullptr;
	MelLib::ModelObject model;

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="pMaterial">�}�e���A��</param>
	Ground(const MelLib::Vector3& pos, const MelLib::Vector3& size,  MelLib::Material* pMaterial);

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

