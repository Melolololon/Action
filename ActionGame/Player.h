#pragma once
#include"GameObject.h"
class Player :public MelLib::GameObject
{
private:

	//�J����
	float cameraSpeed = 3.0f;

public:
	Player(const MelLib::Vector3& pos);
	~Player(){}

#pragma region Update�֌W
	void Update()override;

	
	//����s�\����if�ň͂ނ̑�ς�����֐���
	
	void Move();
	void Camera();
#pragma endregion

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

