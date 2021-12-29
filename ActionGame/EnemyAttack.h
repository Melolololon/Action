#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

#include"Player.h"

//敵の攻撃
class EnemyAttack :
	public MelLib::GameObject
{
private:
	int power;

	MelLib::FrameTimer deadTimer;


	const MelLib::ModelObject& MODEL;

	const MelLib::Vector3 SPHERE_START_POSITION;
	const MelLib::Vector3 MODEL_START_POS;
	const MelLib::Vector3 MODEL_START_ANGLE;
	const MelLib::Vector3 MODEL_START_SCALE;

	static Player* pPlayer;

public:

	// startPosを引数で渡すようにせず、クラス名わたしてこっちに定義した座標を持ってくるなりstatic関数で取得してもいい?
	/// <summary>
	/// 
	/// </summary>
	/// <param name="power">パワー</param>
	/// <param name="pos">これ攻撃側でセットするから消すこと</param>
	/// <param name="radius">半径</param>
	/// <param name="deadTime">消える時間</param>
	/// <param name="model">敵のモデル(アニメーションの状態を取得するのに使用)</param>
	/// <param name="startPos">攻撃の判定をセットしたときの座標(敵ごとに位置が違うので渡す)</param>
	/// <param name="startAngle"></param>
	/// <param name="startScale"></param>
	EnemyAttack
	(
		unsigned int power,
		const MelLib::Vector3& attackStartPos,
		float radius,
		int deadTime,
		const MelLib::ModelObject& model,
		const MelLib::Vector3& modelStartPos,
		const MelLib::Vector3& modelStartAngle,
		const MelLib::Vector3& modelStartScale
	);
	~EnemyAttack() {}

	void Update()override;

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

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

