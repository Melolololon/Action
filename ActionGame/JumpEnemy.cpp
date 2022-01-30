#include "JumpEnemy.h"

#include<GameObjectManager.h>
#include"EditMode.h"
#include"Pause.h"
#include"EnemyAttack.h"

void JumpEnemy::Attack()
{
	// 判定移動
	MelLib::Vector3 attackPos = GetPosition();
	attackPos.y -= 10;
	enemyAttack->SetPosition(attackPos);

	float moveVectorY = GetPosition().y - prePos.y;
	static const float ATTACK_ADD_VECTOR_Y = -1;

	// 移動量が1以上になったら攻撃判定をマネージャーに追加
	if(moveVectorY < ATTACK_ADD_VECTOR_Y
		&& GetIsFall()
		&& !attackAddFrame)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(enemyAttack);
		attackAddFrame = true;
	}
}

void JumpEnemy::AttackRot()
{
	// 攻撃時の回転を開始するフレーム
	static const unsigned int ROT_START_FRAME = 55 - 1;

	// 攻撃中に回転フレームになったら回転開始
	if (isAttack&& modelObjects["main"].GetAnimationFrame() == ROT_START_FRAME)
	{
		modelObjects["main"].SetAnimationPlayFlag(false);

		static const float ROT_ANGLE_Y = 25;
		float setAngle = modelObjects["main"].GetAngle().y + ROT_ANGLE_Y;

		if(setAngle >= 360)setAngle = 0;
		modelObjects["main"].SetAngle(MelLib::Vector3(0, setAngle, 0));

		if(setAngle == ROT_ANGLE_Y)
		{
			//// 攻撃判定の出現位置
			//static const MelLib::Vector3 ATTACK_POS[3] =
			//{
			//	MelLib::Vector3(),
			//	MelLib::Vector3(),
			//	MelLib::Vector3()
			//};

			//// 足の本数だけ攻撃判定の追加
			//for (int i = 0; i < 3; i++) 
			//{
			//	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
			//		(
			//			3,
			//			ATTACK_POS[i],
			//			3.0f,
			//			60 * 0.2,
			//			modelObjects["main"],
			//			MelLib::Vector3(0, 0, 0),
			//			MelLib::Vector3(0, 0, 0),
			//			3
			//			)
			//	);
			//}
		}

		// 360回転したらフレーム進める
		if(setAngle == 0.0f)
		{

			// アニメーションのフレームを進め、回転のif分に入らないようにする
			modelObjects["main"].SetAnimationFrame(ROT_START_FRAME + 1);

			// 再生再開
			modelObjects["main"].SetAnimationPlayFlag(true);
		}

	}


	

}

void JumpEnemy::CheckJumpStart()
{

	bool jumpStartTiming = 
		modelObjects["main"].GetAnimationFrame() >= JUMP_START_FRAME
		&& !modelObjects["main"].GetAnimationReversePlayBack();
	
	// アニメーションがジャンプかアタックの場合のみジャンプ
	// これ書かないと、やられアニメーション中などにジャンプしちゃう
	jumpStartTiming = jumpStartTiming
		&& (modelObjects["main"].GetCurrentAnimationName() == "Jump"
			|| modelObjects["main"].GetCurrentAnimationName() == "Attack");

	// ジャンプ
	if (jumpStartTiming)
	{
		FallStart(3.0f);
	}
}

void JumpEnemy::HitGroundUpdate()
{
	// やられてたらreturn
	if (isDead)return;
	

	// 着地した瞬間逆再生開始
	if (preIsFall
		&& !GetIsFall())
	{
		modelObjects["main"].SetAnimationReversePlayBack(true);
		modelObjects["main"].SetAnimationPlayFlag(true);

		// ジャンプ防止のため、アニメーションのフレームを戻す 
		modelObjects["main"].SetAnimationFrame(JUMP_START_FRAME - 1);

		attackAddFrame = false;
		enemyAttack->TrueEraseManager();
	}

	// 逆再生が終了したら通常通り再生
	if (modelObjects["main"].GetAnimationReversePlayBack()
		&& modelObjects["main"].GetAnimationEndFlag())
	{
		modelObjects["main"].SetAnimationReversePlayBack(false);
	}

}

void JumpEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/JumpEnemy/JumpEnemy.fbx", true, "jumpEnemy");
}

JumpEnemy::JumpEnemy(const MelLib::Vector3 pos, int wallNum)
	:Enemy(pos, 2, 2.0f, wallNum, "jumpEnemy")
{

	//modelObjects["main"]

	// 判定セット
	sphereData.resize(1);
	sphereData[0].SetPosition(pos + MelLib::Vector3(0,20,0)); 
	sphereData[0].SetRadius(10.0f);

	// 地形用判定セット
	segment3DData.resize(1);
	MelLib::Value2<MelLib::Vector3>segmentPos(GetPosition());
	segmentPos.v1 += MelLib::Vector3(0, 10, 0);
	segmentPos.v2 += MelLib::Vector3(0, 0, 0);
	segment3DData[0].SetPosition(segmentPos);

	// 追加時浮いてても落下するようにするための呼び出し
	FallStart(0.0f);

	// アニメーションセット
	modelObjects["main"].SetAnimation("Jump");
	modelObjects["main"].SetAnimationFrame(JUMP_START_FRAME);
	modelObjects["main"].SetAnimationPlayFlag(true);
	modelObjects["main"].SetAnimationEndStopFlag(true);

	// 攻撃判定作成
	enemyAttack = std::make_shared<NormalEnemyAttack>(2, sphereData[0].GetRadius());




}


void JumpEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;


	prePos = GetPosition();

	// 死んでたらreturn
	if (isDead)
	{
		Dead(); 

		// 球の判定処理を無視するように
		collisionFlag.sphere = false;

		modelObjects["main"].SetAnimationPlayFlag(true);
		modelObjects["main"].Update();
		return;
	}

	modelObjects["main"].Update();

	if (!GetIsFall())
	{
		HitGroundUpdate();
	}


	//// ジャンプしてないときの処理
	if (!GetIsFall())
	{
		// ジャンプ開始するか確認
		CheckJumpStart();

	}
	else
	{
		// 通常ジャンプ中はアニメーション停止
		if(!isAttack) modelObjects["main"].SetAnimationPlayFlag(false);
	}


	// 無敵の時間進める
	CheckMutekiEnd();

	// 硬直中だったらreturn
	if (isStun)
	{
		// 硬直関係の処理
		Stun();

		// 硬直終了で切り替え
		if(!isStun)
		{
			modelObjects["main"].SetAnimation("Jump");
		}

		return;
	}


	// ここに攻撃条件を記述
	if (CheckPlayerDistance(25.0f) && !isAttack && !GetIsFall())
	{
		// 攻撃方法変更のためコメントアウト
		//AttackStart();
	}
	else if (!isAttack && GetIsFall())
	{
		if (!CalcPlayerRoute())modelObjects["main"].SetAnimation("No_Move");
		else modelObjects["main"].SetAnimation("Jump");
	}
	else if(!isAttack && !GetIsFall())
	{
		if(CheckPlayerDistance(MOVE_DISTANCE))
		{
			modelObjects["main"].SetAnimation("Jump");
		}
		else
		{
			modelObjects["main"].SetAnimation("No_Move");
		}
	}


	if (isAttack)
	{
		//modelObjects["main"].SetAnimation("Attack");
		AttackRot();
		
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			isAttack = false;
			modelObjects["main"].SetAnimation("Jump");
		}
	}

	CheckAttackEnd();

	// 回転
	RotModel();

	// 時間になったら攻撃
	if (attackTimer.GetNowTime() == ATTACK_START_TIME)
	{
		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(true);
	}

	CalcMovePhysics();


	// 攻撃処理
	Attack();

	preIsFall = GetIsFall();
}
