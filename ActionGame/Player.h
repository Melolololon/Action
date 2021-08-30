#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"PlayerSlush.h"


//攻撃時間終わったかどうか判定から受け取ってプレイヤー制御する?
//攻撃判定の生存時間と攻撃時間は違うからとりあえず今のままでいい?
//とりあえず攻撃時間は技ごとに定義したほうがよさそう

class Player :public MelLib::GameObject
{
private:

	//向いてる向き
	MelLib::Vector3 playerDir = MelLib::Vector3(0, 0, 1);

#pragma region 攻撃

	//現在の攻撃
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;
	//次の攻撃の入力受付開始時間
	static const int ATTACK_NEXT_TIME = 60 * 0.2f;
	//攻撃終了時間
	static const int ATTACK_END_TIME = 60 * 0.5f;
	
	//攻撃時間タイマー
	MelLib::FrameTimer attackTimer;

#pragma endregion

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

