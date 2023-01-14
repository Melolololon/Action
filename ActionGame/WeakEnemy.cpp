#include "WeakEnemy.h"
#include"Pause.h"
#include"Stage.h"

#include"CapsuleEnemyAttack.h"

#include"PlayerSlush.h"
#include"JumpAttack.h"


#include<GameObjectManager.h>

// 35Fで攻撃発生

void WeakEnemy::SetStartAttackAnimation()
{
	modelObjects["main"].SetAnimation("Attack");
}

void WeakEnemy::Attack()
{
	/*timer++;

	if (timer >= 60 * 1.5f)
	{
		isAttack = false;
		timer = 0;
	}*/

	if (modelObjects["main"].GetAnimationEndFlag()) 
	{
		isAttack = false;
	}
}

void WeakEnemy::AddAttackCollisionDetection()
{
	std::shared_ptr<CapsuleEnemyAttack>attack;

	// 判定のセット方法
	// 1.Tポーズにする
	// 2.四角いモデルを用意し、当たり判定の位置にセットし、その座標をメモする
	// 3.2つ目の引数(今回の0の部分)をメモした座標にする
	// 4.2を行ったときのモデルの座標などを引数の5〜7にセットする
	// 
	// 0を判定の初期座標に書き換える
	// 5の部分に、当たり判定の座標ををセットした時の座標、角度、大きさを入力

	// 判定の座標
	MelLib::Value2<MelLib::Vector3>p = 
		MelLib::Value2<MelLib::Vector3>(MelLib::Vector3(-4.9, 9.7, 6.5), MelLib::Vector3(-4.9, 9.7, 8));

	attack = std::make_shared<CapsuleEnemyAttack>
		(
			1,
			p,
			2.0f,
			modelObjects["main"],
			0,
			0,
			1,
			"Bone_R.003",
			"Body"
		);

	MelLib::GameObjectManager::GetInstance()->AddObject(attack);
}

void WeakEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/WeakEnemy.fbx", true, "WeakEnemy");
	int a = 0;
}

WeakEnemy::WeakEnemy(const MelLib::Vector3& pos):NewEnemy("WeakEnemy")
{
	SetPosition(pos);

	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"WeakEnemy");
	modelObjects["main"].Create(MelLib::ModelData::Get("WeakEnemy"),"WeakEnemy");
	modelObjects["main"].SetPosition(pos);

	// 当たり判定の作成
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());

	// プレイヤーみたいな十字の線判定作って、壁でジャンプするようにする


	//modelObjects["main"].SetScale(20);

	modelObjects["Hit01"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "H01");
	modelObjects["Hit01"].SetPosition(MelLib::Vector3(-4.9, 9.7, 6.5));

	modelObjects["Hit02"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "H02");
	modelObjects["Hit02"].SetPosition(MelLib::Vector3(-4.9, 9.7, 8));
}

void WeakEnemy::Initialize()
{
	FallStart(0.0f);
}

void WeakEnemy::Update()
{
	//CalcMovePhysics();

	if (isAttack)Attack();

	// 吹っ飛び中だったら吹っ飛ぶ動作をする
	if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)BeBlownAwayMove();

	// 攻撃判定追加
	if (modelObjects["main"].GetAnimationFrame() == 35) 
	{
		AddAttackCollisionDetection();
	}
}

void WeakEnemy::Draw()
{
	if (isAttack)modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));
	else modelObjects["main"].SetMulColor(MelLib::Color(255,255, 255, 255));
	AllDraw();
}

void WeakEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	//if (Pause::GetInstance()->GetIsPause())return;

	//if (isDead)return;

	std::string n = typeid(object).name();
	if (typeid(object) == typeid(PlayerSlush) || typeid(object) == typeid(JumpAttack) /* && !isMuteki*/)
	{

		// プレイヤーから現在の攻撃の攻撃力を取得し、体力を減らす
		//hp -= pPlayer->GetCurrentAttackPower();

		// 無敵に
		//isMuteki = true;
		//mutekiTimer.SetStopFlag(false);

		// 硬直処理
		//isStun = true;
		//modelObjects["main"].SetAnimation("Stun");

		//modelObjects["main"].SetAnimationFrameStart();
		//modelObjects["main"].SetAnimationEndStopFlag(true);
		//modelObjects["main"].SetAnimationReversePlayBack(false);

		// 攻撃強制終了
		isAttack = false;
		//attackTimer.ResetTimeZero();
		//attackTimer.SetStopFlag(true);


		// 0になったらやられ処理
		/*if (hp <= 0)
		{
			isDead = true;

			modelObjects["main"].SetAnimation("Dead");
		}*/

		// 吹っ飛ばし処理
		// 後で1フレーム置いてNONEに切り替えるようにする
		if (pPlayer->GetPlayerAttackEffect() == AttackEffect::BE_BLOWN_AWAY || typeid(object) == typeid(JumpAttack))
		{
			FallStart(0.6f);
			currentThisAttackEffect = AttackEffect::BE_BLOWN_AWAY;
		}
	}

	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//投げ上げ処理終了
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);

		if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)currentThisAttackEffect = AttackEffect::NONE;
	}
}
