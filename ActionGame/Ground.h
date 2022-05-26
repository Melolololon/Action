#pragma once
#include <GameObject.h>

#include<Material.h>
#include<ModelObject.h>

//�n�ʂ̃��f���̓{�[�h
//�T�C�Y���󂯎��


//�e�N�X�`�����󂯎��
//�e�N�X�`���󂯎��Ɩ���}�e���A���������邱�ƂɂȂ邩��A�}�e���A���n��

// �n�ʂ̓��f��������āA
// ����͔���݂̂ɂ��Ă���������

//�n�ʃN���X
class Ground :
    public MelLib::GameObject
{
private:
	MelLib::ADSAMaterial testMtl;
	
	static const MelLib::Vector2 GROUND_SIZE;

public:
	Ground(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector2& scale);

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

	static void LoadResource();

};

