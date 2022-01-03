#pragma once
#include"GameObject.h"

#include"Player.h"

class GameItem :public MelLib::GameObject
{
protected:
	static Player* pPlayer;

protected:
	bool CheckHitPlayer(const MelLib::GameObject& pObject);

	/// <summary>
	/// �����Ƃ��̏���
	/// </summary>
	void Erase();
public:
	static void SetPPlayer(Player* p) { pPlayer = p; }

	GameItem(const MelLib::Vector3& pos);
	virtual ~GameItem() {}

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
