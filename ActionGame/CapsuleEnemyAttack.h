#pragma once
#include <GameObject.h>
#include"Player.h"

class CapsuleEnemyAttack :
	public MelLib::GameObject
{
private:
	int power = 0;


	const MelLib::ModelObject& MODEL;

	const MelLib::Value2<MelLib::Vector3> CAPSULE_START_POSITION;
	const MelLib::Vector3 MODEL_START_POS;
	const MelLib::Vector3 MODEL_START_ANGLE;
	const MelLib::Vector3 MODEL_START_SCALE;
	const std::string BONE_NAME;
	const std::string MESH_NAME;

	static Player* pPlayer;

public:

	// startPosを引数で渡すようにせず、クラス名わたしてこっちに定義した座標を持ってくるなりstatic関数で取得してもいい?
	/// <summary>
	/// 
	/// </summary>
	/// <param name="power">パワー</param>
	/// <param name="pos">当たり判定の位置</param>
	/// <param name="radius">半径</param>
	/// <param name="deadTime">消える時間</param>
	/// <param name="model">敵のモデル(アニメーションの状態を取得するのに使用)</param>
	/// <param name="startPos">当たり判定をセットしたときのモデルの座標(敵ごとに位置が違うので渡す)</param>
	/// <param name="startAngle">当たり判定をセットしたときのモデルの角度</param>
	/// <param name="startScale">当たり判定をセットしたときのモデルの大きさ</param>
	CapsuleEnemyAttack
	(
		unsigned int power,
		const MelLib::Value2<MelLib::Vector3>& attackStartPos,
		float radius,
		const MelLib::ModelObject& model,
		const MelLib::Vector3& modelStartPos,
		const MelLib::Vector3& modelStartAngle,
		const MelLib::Vector3& modelStartScale,
		const std::string& boneName,
		const std::string& meshName
	);
	~CapsuleEnemyAttack() {}

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
		const GameObject& object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

