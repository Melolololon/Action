#pragma once
#include"GameObject.h"

#include<FrameTimer.h>
#include<Easing.h>

#include"PlayerSlush.h"
#include"Input.h"
#include"Enemy.h"

//攻撃時間終わったかどうか判定から受け取ってプレイヤー制御する?
//攻撃判定の生存時間と攻撃時間は違うからとりあえず今のままでいい?
//とりあえず攻撃時間は技ごとに定義したほうがよさそう

class Enemy;

class Player :public MelLib::GameObject
{
public:
	enum class ActionType
	{
		JUMP,
		ATTACK,
		DASH,
		GUARD,
	};

private:


	static std::unordered_map<ActionType, MelLib::PadButton> keyConfigData;

	//前フレームの座標を格納する変数
	MelLib::Vector3 prePosition;
	//向いてる向き
	MelLib::Vector3 direction = MelLib::Vector3(0, 0, 1);

	static const unsigned int HP_MAX = 150;
	unsigned int hp = HP_MAX;
	bool hitGround = false;
	bool preHitGround = false;
	bool jumpResetAnimation = false;

#pragma region 移動
	float moveSpeed = 0.5f;

	// 落下してない時の数フレーム前の座標を格納する配列(落下時に戻すために使用)
	MelLib::Vector3 notFallPrePosition[5];

#pragma region ダッシュ
	//シュン!って感じで動くやつ
	//ヤーナムステップ、閃乱カグラのあれ
	bool isDash = false;

	MelLib::Easing<MelLib::Vector3> dashEasing;
	//ダッシュの増加量
	MelLib::Vector3 dashAddValue = 0.0f;
#pragma endregion

#pragma endregion

#pragma region ジャンプ
	static const float JUMP_POWER;

	bool isDrop = false;
	float dropStartPosY = 0.0f;

	// ジャンプ開始位置
	MelLib::Vector3 fallStartPosition;
#pragma endregion

#pragma region 攻撃

	struct AttackData
	{
		int power = 0;
		int time = 0;


		//キャンセル可能になるまでの時間
		//現在は攻撃判定のある時間にもなっているが、変える可能性あり
		//その場合、判定のある時間を判定自身に持たせ、引数をなくす
		// 次の攻撃に移れるようになるまでの時間
		int nextTime = 0;

		AttackData() {}
		AttackData(const int power, const int time, const int nextTime) :
			power(power),
			time(time),
			nextTime(nextTime) {}

	};

	std::shared_ptr<PlayerSlush>pPSlush = nullptr;
	std::shared_ptr<PlayerSlush>pRigthSlush = nullptr;
	//現在の攻撃
	PlayerSlush::AttackType currentAttack = PlayerSlush::AttackType::NONE;

	//攻撃時間タイマー
	MelLib::FrameTimer attackTimer;


	// static constだと要素にアクセスできないので、const AttackDataにしてる
	// 攻撃の情報をまとめたもの
	static std::unordered_map<PlayerSlush::AttackType, const AttackData>attackData;

	bool setStartParam = false;
	MelLib::Vector3 startPos;
	MelLib::Vector3 startScale;
	MelLib::Vector3 startAngle;
#pragma endregion

#pragma region 防御
	bool isGuard = false;

#pragma endregion

#pragma region 無敵

	bool isMuteki = false;
	MelLib::FrameTimer mutekiTimer;
#pragma endregion

	// 攻撃を受けて硬直しているじゃどうか
	bool isStun = false;

	//イベントフラグに衝突したかどうか
	bool hitEventFlag = false;


	// 低いところは、着地アニメーションをジャンプの逆再生にし、硬直はなし
	// 地面に着地した時の動けない時間
	MelLib::FrameTimer hitGroundNotMoveTimer;
	bool hitGroundNotMove = false;

#pragma region カメラ
	float cameraSpeed = 0.0f;

	bool lockOn = false;
	const MelLib::GameObject* lockOnEnemy = nullptr;
	float lockOnEnemyDistance = FLT_MAX;
#pragma endregion

	// デバッグ用(trueにすると強制Tポーズ)
	bool isTPause = false;



private:

#pragma region Update関係


	void ChangeAnimationData();

	//操作不可能時にifで囲むの大変だから関数化


	void Move();

	/// <summary>
	/// 落下時にステージに戻す処理
	/// </summary>
	void ReturnStage();

	/// <summary>
	/// 攻撃時の移動
	/// </summary>
	void AttackMove();

	void Dash();
	void DashEnd();

	void Jump();
	void JumpAnimation();

	//攻撃処理、攻撃判定の操作
	void Attack();
	void SetAttackType();
	void CreateAttackSlush();

	void Guard();

	void Muteki();

	// 攻撃硬直
	void Stun();

	// カメラ回転処理
	void RotCamera();
	// カメラに値セット
	void SetCameraData();

	// ロックオン
	void LockOn();

#pragma endregion

public:
	static void LoadResources();

	Player(const MelLib::Vector3& pos);
	~Player() {}

	PlayerSlush* GetPPlayerSlush() { return pPSlush.get(); }

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


	void DownHP(const unsigned int power) { hp -= power; }
	void LifeUp(const unsigned int upNum);
#pragma region Get
	static const std::unordered_map<Player::ActionType, MelLib::PadButton> GetKeyConfigData() { return keyConfigData; }

	unsigned int GetCurrentAttackPower()const { return attackData[currentAttack].power; }

	MelLib::Vector3 GetDirection() { return direction; }

	const MelLib::GameObject* GetPLockOnEnemy()const { return lockOnEnemy; }
	
	unsigned int GetHP()const { return hp; }
	unsigned int GetHPMax()const { return HP_MAX; }

	bool GetIsMuteki()const { return isMuteki; }

	bool GetHitEventFlag()const { return hitEventFlag; }
#pragma endregion

#pragma region Set

	static void SetKeyConfigData(const ActionType type, const MelLib::PadButton button) { keyConfigData[type] = button; }

	/// <summary>
	/// ロックオン強制終了
	/// </summary>
	void LockOnEnd() 
	{ 
		lockOnEnemy = nullptr;
		lockOn = false;
	}
#pragma endregion

	void TitleUpdate();

};

