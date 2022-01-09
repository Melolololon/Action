#pragma once
#include<GameObject.h>

class Wall : public MelLib::GameObject
{
	private:
		MelLib::ADSAMaterial testMtl;

		static const MelLib::Vector2 GROUND_SIZE;

	public:
		Wall(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector2& scale);

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


	};

