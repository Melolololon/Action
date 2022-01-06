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
	// Tポーズ時の判定の位置
	static const MelLib::Value2<MelLib::Vector3>CAPSULE_START_POS_LEFT;
	static const MelLib::Value2<MelLib::Vector3>CAPSULE_START_POS_RIGTH;


	// 線分から平面情報取得を考えてみて無理だったら定義
	//static const std::unordered_map<AttackType, MelLib::PlaneData> attackPlane;


	float playerAngle = 0.0f;

	// 前フレームの線分の座標
	MelLib::Value2<MelLib::Vector3>preSegmentPosition;

	const MelLib::ModelObject& PLAYER_MODEL;
	const MelLib::Vector3 PLAYER_DIRECTION;

	const MelLib::Vector3 PLAYER_START_POS;
	const MelLib::Vector3 PLAYER_START_ANGLE;
	const MelLib::Vector3 PLAYER_START_SCALE;
private:
	MelLib::FrameTimer eraseTimer;
	AttackType attackType = AttackType::NORMAL_1;

	bool slushLeft = false;
private:
	void SetAttackParam();

	void Attack();
public:


	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="playerDir">プレイやーの向き</param>
	PlayerSlush
	(
		const MelLib::Vector3& pos,
		const MelLib::Vector3& playerDir,
		const AttackType type,
		const int attackTime,
		const MelLib::ModelObject& model,
		const MelLib::Vector3 playerStartPos,
		const MelLib::Vector3 playerStartAngle,
		const MelLib::Vector3 playerStartScale,
		bool slushLeft
	);

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

#pragma region ゲット

	MelLib::Value2<MelLib::Vector3>GetSegmentPosition()const { return capsuleData[0].GetSegment3DData().GetPosition(); }
	MelLib::Value2<MelLib::Vector3>GetSegmentPrePosition()const { return preSegmentPosition; }

#pragma endregion

};

