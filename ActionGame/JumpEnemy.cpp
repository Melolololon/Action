#include "JumpEnemy.h"

#include<GameObjectManager.h>
#include"EditMode.h"
#include"Pause.h"
#include"EnemyAttack.h"

void JumpEnemy::AttackRot()
{
	// 攻撃時の回転を開始するフレーム
	static const unsigned int ROT_START_FRAME = 1;

	// 攻撃中に回転フレームになったら回転開始
	if (isAttack&& modelObjects["main"].GetAnimationFrame() == ROT_START_FRAME)
	{
		modelObjects["main"].SetAnimationPlayFlag(true);

		static const float ROT_ANGLE_Y = 25;
		float setAngle = modelObjects["main"].GetAngle().y + ROT_ANGLE_Y;

		if(setAngle >= 360)setAngle = 0;
		modelObjects["main"].SetAngle(MelLib::Vector3(0, setAngle, 0));

		if(setAngle == ROT_ANGLE_Y)
		{
			// 攻撃判定の出現位置
			static const MelLib::Vector3 ATTACK_POS[3] =
			{
				MelLib::Vector3(),
				MelLib::Vector3(),
				MelLib::Vector3()
			};

			// 足の本数だけ攻撃判定の追加
			for (int i = 0; i < 3; i++) 
			{
				MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
					(
						3,
						ATTACK_POS[i],
						3.0f,
						60 * 0.2,
						modelObjects["main"],
						MelLib::Vector3(0, 0, 0),
						MelLib::Vector3(0, 0, 0),
						3
						)
				);
			}
		}

		// 360回転したらフレーム進める
		if(setAngle == 0.0f)
		{

			// アニメーションのフレームを進め、回転のif分に入らないようにする
			modelObjects["main"].SetAnimationFrame(ROT_START_FRAME + 1);
		}

	}


	

}

void JumpEnemy::CheckJumpStart()
{
	

	// ジャンプ開始フレーム
	static const unsigned int JUMP_START_FRAME = 0;

	bool jumpStartTiming =
		modelObjects["main"].GetCurrentAnimationName() == "Jump"
		&& modelObjects["main"].GetAnimationFrame() == JUMP_START_FRAME
		&& !modelObjects["main"].GetAnimationReversePlayBack();
	
	if(jumpStartTiming)
	{
		FallStart(20.0f);
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
}

void JumpEnemy::HitGroundUpdate()
{

	// 着地した瞬間逆再生開始
	if(modelObjects["main"].GetAnimationEndFlag()
		&& !modelObjects["main"].GetAnimationReversePlayBack())
	{
		modelObjects["main"].SetAnimationReversePlayBack(true);
	}

	// 逆再生が終了したら通常通り再生
	if(modelObjects["main"].GetAnimationEndFlag()
		&& modelObjects["main"].GetAnimationReversePlayBack())
	{
		modelObjects["main"].SetAnimationReversePlayBack(false);
	}

}

void JumpEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/JumpEnemy/JumpEnemy.fbx", true, "jumpEnemy");
}

JumpEnemy::JumpEnemy(const MelLib::Vector3 pos)
	:Enemy(pos, 2, 2.0f, "jumpEnemy")
{
	//modelObjects["main"]

	//sphereData.resize(1);
	//sphereData[0].SetPosition(pos); 
	//sphereData[0].SetRadius(5.0f);

	segment3DData.resize(1);
	MelLib::Value2<MelLib::Vector3>segmentPos(GetPosition());
	segmentPos.v1 += MelLib::Vector3(0, 10, 0);
	segmentPos.v2 += MelLib::Vector3(0, -4, 0);
	segment3DData[0].SetPosition(segmentPos);

	FallStart(0.0f);

	modelObjects["main"].SetAnimation("Jump");
	modelObjects["main"].SetAnimationFrameEnd();
}


void JumpEnemy::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	modelObjects["main"].Update();

	// ジャンプ開始するか確認
	if (!GetIsFall())CheckJumpStart();

	// ジャンプしてないときの処理
	if (!GetIsFall())HitGroundUpdate();

	// 死んでたらreturn
	if (isDead)
	{
		Dead();
		return;
	}

	// 無敵の時間進める
	CheckMutekiEnd();

	// 硬直中だったらreturn
	if (isStun)
	{
		// 硬直関係の処理
		Stun();
		return;
	}

	// ここに攻撃条件を記述
	if (CheckPlayerDistance(25.0f) && !isAttack)
	{
		AttackStart();
	}
	else if (!isAttack && GetIsFall())
	{
		CalcPlayerRoute();

	}
	if (isAttack)
	{
		//modelObjects["main"].SetAnimation("Attack");

		
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			isAttack = false;
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

}
