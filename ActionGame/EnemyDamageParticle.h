#pragma once
#include <GameObject.h>

// �G���a��ꂽ�Ƃ��ɏo�Ă���p�[�e�B�N��
class EnemyDamageParticle :
    public MelLib::GameObject
{
public:
    EnemyDamageParticle();
    ~EnemyDamageParticle();

    void Initialize()override;
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
		const GameObject& object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;
};

