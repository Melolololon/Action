#pragma once
#include"GameObject.h"

#include"FrameTimer.h"


//刀薄いから、板の判定でもいいかも

//プレイヤーの攻撃判定
class PlayerSlush :public MelLib::GameObject
{
public:
	enum class AttackType
	{
		NONE,
		NORMAL_1,//通常(Xボタン)攻撃1番目
		NORMAL_2,//通常(Xボタン)攻撃2番目
		NORMAL_3,//通常(Xボタン)攻撃3番目
		DASH_1,// ダッシュ中1番目
	};

private:
	


	float playerAngle = 0.0f;
private:
	MelLib::FrameTimer eraseTimer;
	AttackType attackType = AttackType::NORMAL_1;
private:
	void SetAttackParam();

	void Attack();
public:


	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="playerDir">プレイやーの向き</param>
	PlayerSlush(const MelLib::Vector3& pos,const MelLib::Vector3& playerDir,const AttackType type,const int attackTime);

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
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;


};

