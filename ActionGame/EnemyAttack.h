#pragma once
#include <GameObject.h>
#include<FrameTimer.h>

#include"Player.h"

//敵の攻撃
class EnemyAttack :
	public MelLib::GameObject
{
public:
	enum class AttackType
	{
		NORMAL,
		BE_BLOWN_AWAY,
	};


private:

	void SetAttackTypeTag(AttackType type);
	void SetInitData(const float radius, AttackType type);

	int timer = 0;
private:
	static const std::string ATTACK_TYPE_STR_NORMAL;
	static const std::string ATTACK_TYPE_STR_BE;
	
	int power = 0;

	// ごちゃごちゃするからクラス分けてもいいかも AttackTypeはヘッダー用意してそこで宣言しよう
	enum class MoveType 
	{
		OBJECT,// オブジェクトに追尾
		ANIMATION// アニメーションに合わせる
	};
	const MoveType MOVE_TYPE = MoveType::OBJECT;

	const MelLib::ModelObject& MODEL;

	const MelLib::Vector3 SPHERE_START_POSITION;
	const MelLib::Vector3 MODEL_START_POS;
	const MelLib::Vector3 MODEL_START_ANGLE;
	const MelLib::Vector3 MODEL_START_SCALE;

	const unsigned int DELETE_FRAME;
	

	static Player* pPlayer;

public:
	
	static AttackType GetAttackType(const std::string& tag);
	static std::string GetAttackTypeStr(const AttackType type);
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
		const MelLib::ModelObject& model,
		const MelLib::Vector3& modelStartPos,
		const MelLib::Vector3& modelStartAngle,
		const MelLib::Vector3& modelStartScale,
		const AttackType attackType,
		const unsigned int deleteFrame
	);

	EnemyAttack
	(
		unsigned int power,
		const MelLib::ModelObject& model,
		float radius,
		const AttackType attackType,
		const unsigned int deleteFrame
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
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

