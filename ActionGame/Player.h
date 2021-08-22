#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"PlayerSlush.h"

class Player :public MelLib::GameObject
{
private:
	
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;
	static const int ATTACK_NEXT_TIME = 60 * 0.2f;
	static const int ATTACK_END_TIME = 60 * 0.5f;
	MelLib::FrameTimer attackTimer;


public:
	Player(const MelLib::Vector3& pos);
	~Player(){}

#pragma region Update関係
	
	//操作不可能時にifで囲むの大変だから関数化
	
	void Move();
	void Attack();
	void SetAttackType();
	void Camera();
#pragma endregion

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
	/// <param name="hitObjColType">相手のどの判定に当たったか</param>
	/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>
	void Hit
	(
		const GameObject* const  object,
		const MelLib::ShapeType3D collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D hitObjColType,
		const int hitObjArrayNum
	)override;
};

