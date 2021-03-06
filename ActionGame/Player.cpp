#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Title.h"

#include"ActionPart.h"

#include"PlayerSlush.h"

#include"Stage.h"
#include"Ground.h"
#include"Wall.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"
#include"NormalEnemyAttack.h"
#include"JumpEnemy.h"

#include"EventFlag.h"
#include"TutorialEventFlag.h"


// テスト
#include"SlushEffect.h"

std::unordered_map<Player::ActionType, MelLib::PadButton> Player::keyConfigData =
{
	{ActionType::JUMP, MelLib::PadButton::A},
	{ActionType::ATTACK, MelLib::PadButton::X},
	{ActionType::DASH, MelLib::PadButton::B},
	{ActionType::GUARD, MelLib::PadButton::LB},

};

const float Player::JUMP_POWER = 2.0f;

std::unordered_map<PlayerSlush::AttackType, const Player::AttackData> Player::attackData =
{
	{PlayerSlush::AttackType::NONE,AttackData(0,0,0)},
	{PlayerSlush::AttackType::NORMAL_1,AttackData(1,20,10)},
	{PlayerSlush::AttackType::NORMAL_2,AttackData(1,30,20)},
	{PlayerSlush::AttackType::NORMAL_3,AttackData(1,30,20)},// これのアニメーションバグるのアニメーション終わる前にキャンセルされて立ちに戻るから?
	{PlayerSlush::AttackType::DASH_1,AttackData(1,20,10)},
};



void Player::LoadResources()
{
	// タイトルでも使うから自動削除削除
	MelLib::ModelData::Load("Resources/Model/Player/Player.fbx", false, "Player");
	//MelLib::ModelData::Load("Resources/Model/Enemy/Mokuzin/Mokuzin.fbx", false, "Player");

	// メッシュ2つ以上の時にマテリアル読むとエラーでる
	// そうじゃないときにエラー出たらマテリアル確認すること
}

Player::Player(const MelLib::Vector3& pos)
{

	//物理演算のためにseterとgeter作る?


	collisionFlag.capsule = true;
	capsuleDatas.resize(1);
	capsuleDatas[0].SetRadius(3.0f);
	capsuleDatas[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 15, 0), GetPosition() + MelLib::Vector3(0, -18, 0)));

	segment3DDatas.resize(3);
	segment3DDatas[0] = capsuleDatas[0].GetSegment3DData();

	// 壁との判定用
	segment3DDatas[1].SetPosition
	(
		MelLib::Value2<MelLib::Vector3>
		(GetPosition() + MelLib::Vector3(0, 0, 10), GetPosition() + MelLib::Vector3(0, 0, -10))
	);

	segment3DDatas[2].SetPosition
	(
		MelLib::Value2<MelLib::Vector3>
		(GetPosition() + MelLib::Vector3(10, 0, 0), GetPosition() + MelLib::Vector3(-10, 0, 0))
	);



	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	modelObjects["main"].Create(MelLib::ModelData::Get("Player"), nullptr);

	modelObjects["main"].SetAnimationPlayFlag(true);
#pragma region ダッシュ
	dashEasing.SetAddPar(5.0f);


#pragma endregion

	mutekiTimer.SetMaxTime(60 * 1.5);



	// タイトル用処理
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(Title))
	{
		modelObjects["main"].SetScale(MelLib::Vector3(3));
		modelObjects["main"].SetAngle(0);
		modelObjects["main"].SetAnimation("No_Cont");
	}
	else
	{
		modelObjects["main"].SetScale(MelLib::Vector3(3));
		modelObjects["main"].SetAngle(0);
		modelObjects["main"].SetPosition(MelLib::Vector3(0, -17, -0));


		//浮き防止
		FallStart(0.0f);

		// 落下アニメーションからスタート
		modelObjects["main"].SetAnimation("Jump_Up");
	}

	SetPosition(pos);
}

void Player::Update()
{


	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())
	{
		if(EditMode::GetInstance()->GetIsEdit())RotCamera();
		


		MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
		if (typeid(*currentScene) != typeid(Title))
		{
			SetCameraData();
		}
		return;
	}


	hitEventFlag = false;
	hitTutorialEventFlag = false;

	modelObjects["main"].Update();
	
	if(isDead)
	{
		Dead();
		return;
	}


	if (isStun)
	{
		Stun();
		SetCameraData();
		RotCamera();
		LockOn();
		DashEnd();
		return;
	}

	if (!setStartParam)
	{
		if (typeid(*currentScene) != typeid(Title))
		{
			JumpAnimation();
			ChangeAnimationData();

			SetCameraData();
			CalcMovePhysics();
		}
		return;
	}

	if (hitGroundNotMove)
	{
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			hitGroundNotMove = false;
		}
		RotCamera();

		return;
	}

	prePosition = GetPosition();
	if (!GetIsFall() && !isDash)
	{
		// 前フレームの座標の格納場所をずらす
		for (int i = 0; i < _countof(notFallPrePosition) - 1; i++)
		{
			notFallPrePosition[i + 1] = notFallPrePosition[i];
		}
		notFallPrePosition[0] = prePosition;
	}

	Move();
	Jump();

	ReturnStage();

	Guard();
	Attack();

	CalcMovePhysics();

	Muteki();

	RotCamera();
	//LockOn();

	float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	// 回転処理(仮)
	modelObjects["main"].SetAngle(MelLib::Vector3(0, angle, 0));


	// アニメーションに合わせて再生速度やループを設定する
	ChangeAnimationData();

	// 落下中差を追加
	if(GetIsFall())
	{
		dropStartPosY -= prePosition.y - GetPosition().y;
	}

	// この数値以上落下したら落下扱い
	static const float DROP_POS_Y = 10.0f;

	// 一定量落下したら落下扱い
	if (dropStartPosY >= DROP_POS_Y)isDrop = true;


	preHitGround = hitGround;
	hitGround = false;
}

void Player::TitleUpdate()
{
	// 移動
	AddPosition(MelLib::Vector3(0, 0, 0.5f));


	// ジャンプ
	if (!GetIsFall())
	{
		FallStart(JUMP_POWER);
		modelObjects["main"].SetAnimation("Jump_Up");
	}
	CalcMovePhysics();
	ChangeAnimationData();
}

void Player::ChangeAnimationData()
{
	std::string animationName = modelObjects["main"].GetCurrentAnimationName();

	modelObjects["main"].SetAnimationSpeedMagnification(1);
	modelObjects["main"].SetAnimationEndStopFlag(false);

	if (animationName == "Dash") {
		modelObjects["main"].SetAnimationSpeedMagnification(2);
	}
	else if (animationName.find("Attack") != std::string::npos) {

		modelObjects["main"].SetAnimationSpeedMagnification(2);
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Dash_02") != std::string::npos) {

		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Jump_Up") != std::string::npos)
	{
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Dead") != std::string::npos)
	{
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}

	//// デバッグ用
	//if (MelLib::Input::KeyTrigger(DIK_1))
	//{
	//	isTPause = !isTPause;
	//}

	// 強制Tポーズ
	if (isTPause)
	{
		modelObjects["main"].SetAnimation("_T");
		modelObjects["main"].SetAnimationSpeedMagnification(1);
		modelObjects["main"].SetAnimationEndStopFlag(false);
	}

}

void Player::Move()
{

	// 仮
	modelObjects["main"].SetAnimationSpeedMagnification(1);

	Dash();

	if (isDash)
	{
		return;
	}
	else if (attackTimer.GetNowTime() > 0)
	{
		AttackMove();
		return;
	}


	static const float FAST_WARK_STICK_PAR = 60.0f;
	static const float WALK_STICK_PAR = 20.0f;
	static const float MAX_FAST_WARK_SPEED = 3.0f;
	static const float MAX_WALK_SPEED = 0.5f;
	static const float FRAME_UP_FAST_WARK_SPEED = MAX_FAST_WARK_SPEED / 10;
	static const float FRAME_UP_WARK_SPEED = MAX_WALK_SPEED / 10;

	if (MelLib::Input::LeftStickUp(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickDown(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickRight(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickLeft(WALK_STICK_PAR))
	{
		direction = MelLib::Input::LeftStickVector3(MelLib::Camera::Get(), false, true);

		MelLib::Vector3 addPos = direction;

		//ダッシュの傾き量を超えていたらダッシュ
		if (MelLib::Input::LeftStickUp(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickDown(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickRight(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickLeft(FAST_WARK_STICK_PAR))
		{
			if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
			else moveSpeed = MAX_FAST_WARK_SPEED;
			addPos *= moveSpeed;

			modelObjects["main"].SetAnimation("Dash");
		}
		else
		{
			if (moveSpeed < MAX_WALK_SPEED)moveSpeed += FRAME_UP_WARK_SPEED;
			else moveSpeed = MAX_WALK_SPEED;
			addPos *= moveSpeed;
			addPos *= MAX_WALK_SPEED;

			modelObjects["main"].SetAnimation("Walk");
		}

		AddPosition(addPos);


	}
	else
	{
		moveSpeed = 0.0;

		modelObjects["main"].SetAnimation("No_Cont");
	}


	//if (position.y <= -100)position.y = 100;

	//落下するために
	if (!GetIsFall())FallStart(0.0f);



}

void Player::ReturnStage()
{

	// 開始位置ぴったりに戻すと、ジャンプせずに落ちた時に、
	// 落下開始時はすでに足元に足場がなく無限に落ち続けるため、対処が必要
	if(GetPosition().y <= -1000)
	{
		SetPosition(notFallPrePosition[_countof(notFallPrePosition) - 1]);

		DownHP(30);
	}
}

void Player::AttackMove()
{

	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		//AddPosition(direction * 0.1f);
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::DASH_1:
		break;
	default:
		break;
	}



}

void Player::Dash()
{


	static const float MAX_DASH_SPEED = 1.5f;
	static const float START_DASH_SPEED = 1.0f;

	static const float DASH_DISTANCE = 100.0f;

	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::DASH])
		&& !isDash
		&& currentAttack == PlayerSlush::AttackType::NONE)
	{
		isDash = true;

		dashEasing.SetStart(GetPosition());
		dashEasing.SetEnd(MelLib::LibMath::FloatDistanceMoveVector3(GetPosition(), direction, DASH_DISTANCE));
	}

	if (isDash)
	{
		MelLib::Vector3 easingPrePos = GetPosition();
		MelLib::Vector3 pos = dashEasing.EaseInOut();
		MelLib::Vector3 addPos = pos - easingPrePos;

		// Yを0にしないとダッシュ中にYが変わったらおかしくなっちゃう
		AddPosition(MelLib::Vector3(addPos.x, 0, addPos.z));


		// ダッシュ攻撃中じゃなかったらダッシュアニメーション
		if (currentAttack != PlayerSlush::AttackType::DASH_1)
		{
			modelObjects["main"].SetAnimation("Dash_02");
		}

	}

	if (dashEasing.GetPar() >= 100.0f)
	{
		DashEnd();
	}
}

void Player::DashEnd()
{
	dashEasing.SetPar(0.0f);
	isDash = false;
}

void Player::Jump()
{
	// ジャンプの着地アニメーションは、ジャンプの逆再生でOK

	if (attackTimer.GetNowTime() != 0 
		|| isDash)return;

	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::JUMP]))
	{
		FallStart(JUMP_POWER);
	}


	JumpAnimation();
}

void Player::JumpAnimation()
{

	// 条件満たしたらジャンプアニメーション
	if (!hitGround && !preHitGround && !isDash && currentAttack == PlayerSlush::AttackType::NONE && isDrop)
	{
		if (!jumpResetAnimation)
		{
			jumpResetAnimation = true;
			modelObjects["main"].SetAnimationFrameStart();
		}
		modelObjects["main"].SetAnimation("Jump_Up");
	}
}



void Player::Attack()
{
	// 空中攻撃とダッシュ攻撃はいったんなし
	if (isDrop && isDash)return;

	if (attackTimer.GetMaxOverFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		attackTimer.ResetTimeZero();

		attackTimer.SetStopFlag(true);
	}

	//ifの2行目のタイマー確認は、コンボ終了後にNONEにするため、その攻撃中に入らないようにするために書いてる
	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::ATTACK])
		&& (attackTimer.GetNowTime() == 0 && currentAttack == PlayerSlush::AttackType::NONE
			|| attackTimer.GetNowTime() >= attackData[currentAttack].nextTime && currentAttack != PlayerSlush::AttackType::NONE))
	{


		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(false);


		// 向き切り替え
		Move();

		//CurrentAttack更新
		SetAttackType();
		//攻撃時間セット
		attackTimer.SetMaxTime(attackData[currentAttack].time);

		//コンボの最後の時にボタン押したらNONEをセットするため、コンボを終わらせるためのif
		if (currentAttack != PlayerSlush::AttackType::NONE)
		{
			if (pPSlush)pPSlush.reset();
			if (pRigthSlush)pRigthSlush.reset();


			CreateAttackSlush();

			//アニメーションをリセット
			modelObjects["main"].SetAnimationFrameStart();
		}
	}

	//攻撃判定も動かす
	//if (pPSlush)pPSlush->AddPosition(GetPosition() - prePos);
}

void Player::SetAttackType()
{


	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:

		// ダッシュ攻撃
		if (isDash)
		{
			currentAttack = PlayerSlush::AttackType::DASH_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");

			isDash = false;
			DashEnd();
		}
		else if (hitGround)// 通常攻撃
		{
			currentAttack = PlayerSlush::AttackType::NORMAL_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");

		}
		else// 空中攻撃
		{
		}

		break;
	case PlayerSlush::AttackType::NORMAL_1:
		currentAttack = PlayerSlush::AttackType::NORMAL_2;

		modelObjects["main"].SetAnimation("Attack_Normal_2");
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		currentAttack = PlayerSlush::AttackType::NORMAL_3;

		modelObjects["main"].SetAnimation("Attack_Normal_3");
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;

	case PlayerSlush::AttackType::DASH_1:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;

	default:
		break;
	}



}

void Player::CreateAttackSlush()
{
	// 消える時間を次の攻撃へ移行できるようになる時間と同じにすること


	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		pRigthSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);

		break;
	case PlayerSlush::AttackType::NORMAL_2:


		pPSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, true);

		break;
	case PlayerSlush::AttackType::NORMAL_3:
		pPSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, true);

		pRigthSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);


		break;
	case PlayerSlush::AttackType::DASH_1:
		pRigthSlush = std::make_shared<PlayerSlush>
		(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);

		break;
	default:
		break;
	}

	if (pPSlush)MelLib::GameObjectManager::GetInstance()->AddObject(pPSlush);
	if (pRigthSlush)MelLib::GameObjectManager::GetInstance()->AddObject(pRigthSlush);
}

void Player::Guard()
{
	if (MelLib::Input::PadButtonState(keyConfigData[ActionType::GUARD]))
	{
		isGuard = true;
	}
	else
	{
		isGuard = false;
	}
}

void Player::Muteki()
{
	if (mutekiTimer.GetMaxOverFlag())
	{
		isMuteki = false;
		mutekiTimer.ResetTimeZero();
		mutekiTimer.SetStopFlag(true);
	}
}

void Player::Stun()
{
	if(modelObjects["main"].GetAnimationEndFlag())
	{
		isStun = false;
	}
}

void Player::Dead()
{
	modelObjects["main"].SetAnimation("Dead");
	ChangeAnimationData();
}

void Player::RotCamera()
{

	if (lockOn)return;

	//スティックの倒し具合でスピード変える
	//緩急あったほうがいい?だんだん加速してく。早めにスピードマックスになる

	//メインカメラのポインタ取得
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0;
	static const float MAX_CAMERA_ANGLE_X = 30.0f;
	static const float MIX_CAMERA_ANGLE_X = -5.0f;

	//最大値は一旦const(あとで設定で最大値を変えられるようにする)
	static const float MAX_CAMERA_SPEED = 3.0f;
	static const float FRAME_UP_CAMERA_SPEED = MAX_CAMERA_SPEED / 10;

	if (!MelLib::Input::RightStickLeft(30.0f)
		&& !MelLib::Input::RightStickRight(30.0f)
		&& !MelLib::Input::RightStickUp(30.0f)
		&& !MelLib::Input::RightStickDown(30.0f))cameraSpeed = 0.0f;
	else cameraSpeed += FRAME_UP_CAMERA_SPEED;

	if (MelLib::Input::RightStickLeft(30.0f))addCameraAngle.y = -cameraSpeed;
	else if (MelLib::Input::RightStickRight(30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(30.0f))addCameraAngle.x = -cameraSpeed;
	else if (MelLib::Input::RightStickDown(30.0f))addCameraAngle.x = cameraSpeed;

	if (cameraSpeed >= MAX_CAMERA_SPEED)cameraSpeed = MAX_CAMERA_SPEED;

	cameraAngle += addCameraAngle;
	if (cameraAngle.x >= MAX_CAMERA_ANGLE_X)cameraAngle.x = MAX_CAMERA_ANGLE_X;
	if (cameraAngle.x <= MIX_CAMERA_ANGLE_X)cameraAngle.x = MIX_CAMERA_ANGLE_X;

	pCamera->SetAngle(cameraAngle);
	

	//pCamera->SetRotateCriteriaPosition(position);
	SetCameraData();
}

void Player::SetCameraData()
{
	MelLib::Camera* pCamera = MelLib::Camera::Get();
	/*pCamera->SetRotateCriteriaPosition
	(MelLib::LibMath::FloatDistanceMoveVector3
	(GetPosition(), MelLib::LibMath::OtherVector3(pCamera->GetCameraPosition(), pCamera->GetTargetPosition()), 30.0f));*/

	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(80.0f);

	if (!lockOn)
	{
		pCamera->SetRotateCriteriaPosition(GetPosition() + MelLib::Vector3(0, 10, 0));
	}
}

void Player::LockOn()
{
	// 全敵を取得
	// 距離以内かつ最短のやつにカメラ向ける
	// ロックオン中はカメラは動かせない
	// プレイヤーが映るかつ、注視点を敵にセットしていい感じに映るようにする。
	// 既存のゲームを参考にしたほうがいいかも
	// プレイヤーと敵の方向ベクトルに応じてカメラ回転させれば、いい感じになるかも?

	// スティックでロックオン対象を切り替えれるようにすること

	// ロックオンの最高距離
	static const float LOCK_ON_DISTANCE = 200.0f;

	// ロックオン有効、無効
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::R_STICK))
	{
		lockOn = !lockOn;
		if(lockOn)
		{
			lockOnEnemyDistance = FLT_MAX;
			// 近い敵を取得
			for (const auto& enemy : ActionPart::GetEnemys())
			{
				if (enemy->GetEraseManager())continue;

				float distance = MelLib::LibMath::CalcDistance3D(GetPosition(), enemy->GetPosition());
				if (distance <= lockOnEnemyDistance)
				{
					lockOnEnemyDistance = distance;
					lockOnEnemy = enemy.get();
				}
			}

			// 敵いなかったら、または、近くにいなかったら終了
			
			if (lockOnEnemyDistance == FLT_MAX || lockOnEnemyDistance > LOCK_ON_DISTANCE)
			{
				LockOnEnd();
				return;
			}

		}
	}

	// 切り替え
	if(MelLib::Input::RightStickRight(40.0f))
	{
	}
	else if(MelLib::Input::RightStickLeft(40.0f))
	{
	}

	// ロックオンを解除した、または、ロックオンした敵がやられてnullptrになったらreturn
	if (!lockOn || !lockOnEnemy)return;

	// カメラ操作
	// カメラの動きを補完すること
	// まずは視点切り替えを優先して実装すること
	
	lockOnEnemyDistance = MelLib::LibMath::CalcDistance3D(GetPosition(), lockOnEnemy->GetPosition());

	// 離れたら解除
	if(lockOnEnemyDistance >= LOCK_ON_DISTANCE)
	{
		lockOn = false;
		return;
	}

	// 注視点を敵の座標に
	MelLib::Camera::Get()->SetRotateCriteriaPosition(lockOnEnemy->GetPosition());

	// 近づける限界地
	static const float MIN_DISTANCE = 50.0f;
	if (lockOnEnemyDistance < MIN_DISTANCE)lockOnEnemyDistance = MIN_DISTANCE;

	// 注視点との距離を変える
	MelLib::Camera::Get()->SetCameraToTargetDistance(lockOnEnemyDistance * 2.0f);


	MelLib::Vector3 playerToEnemy = lockOnEnemy->GetPosition() - GetPosition();
	float xzAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerToEnemy.x, playerToEnemy.z), true);

	static const float CONST_ANGLE_X = 35;
	/*float disAngle = lockOnEnemyDistance * 0.3f;
	if (disAngle - CONST_ANGLE_X > 90 - CONST_ANGLE_X)disAngle = 90 - CONST_ANGLE_X;*/
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(CONST_ANGLE_X , -xzAngle + 90,0));
}

void Player::HitWall()
{
	AddPosition(MelLib::Vector3(prePosition.x - GetPosition().x, 0, prePosition.z - GetPosition().z));
	DashEnd();
}


void Player::Draw()
{
	modelObjects["main"].Draw();
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	// 敵と当たった時の処理
	for (const auto& tag : object->GetTags()) 
	{
		if(tag == "Enemy" || tag == "StageObject")
		{
			// 真上から移動せずに乗っかっても押し出すようにする
			// 敵から自分へのベクトルを求め、その方向に押し出す。
			// 真上の場合は、適当に押し出す
			MelLib::Vector3 enemyToPlayer = GetPosition() - object->GetPosition();
			enemyToPlayer = enemyToPlayer.Normalize();

			if(GetIsFall())
			{
				AddPosition(MelLib::Vector3(enemyToPlayer.x, 0, enemyToPlayer.z) * 5.0f);
				
			}
			else if (prePosition - GetPosition() == 0)
			{
				if (typeid(*object) == typeid(JumpEnemy))
				{
					AddPosition(MelLib::Vector3(1,0,0) * 1.0f);
				}
				else 
				{
					AddPosition(MelLib::Vector3(enemyToPlayer.x, 0, enemyToPlayer.z) * 4.0f);
				}
			}
			else 
			{
				
				AddPosition(MelLib::Vector3(prePosition.x - GetPosition().x, 0, prePosition.z - GetPosition().z));
				
			}
		}

		if (tag == "StageObject")
		{
			DashEnd();
		}
	}


	// 線分がステージの判定に当たったら入る
	//if (typeid(*object) == typeid(Ground)
	if (typeid(*object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{
		// ステージの三角形の法線取得
		MelLib::Vector3 normal = GetHitTriangleData().GetNormal();
		
		// 一定以上だったら床扱い
		if(normal.y >= 0.6f)
		{
			// 0番以外の判定(床用判定)意外と当たったらreturn
			if (arrayNum != 0)return;

			// 落下距離をリセット
			dropStartPosY = 0.0f;

			// 着地処理
			if (!hitGroundNotMove)
			{
				float velocityY = GetVelocity().y;

				// 高いところから落ちたら手をつくため、動けなくする
				if (velocityY <= -3.0f)
				{
					hitGroundNotMove = true;

					modelObjects["main"].SetAnimation("Jump_End_1");
					modelObjects["main"].SetAnimationSpeedMagnification(1);

					// ダッシュ中に地面についたら強制終了
					DashEnd();
				}
				jumpResetAnimation = false;

				isDrop = false;
			}


			MelLib::Vector3 addPos;

			//velocity分戻すと戻しすぎちゃう
			//カプセルの下の座標-半径(先端座標)から衝突点へのベクトルの大きさ分戻せばOK?

			//重力加速度分上げる
			//addPos.y += MelLib::GameObject::GetGravutationalAcceleration();

			//投げ上げ処理終了
			FallEnd();

			//カプセルの下の先端から衝突点のベクトルを求め、その分押し出す。
			//-0.15fは、押し出しすぎるとHitが呼ばれなくなって、非ジャンプ時の落下の処理で下がってがくがくするのを防止するためにある
			//線分の先端がヒットしたらカプセルの先端が上に来るまで押し返すから、-0.15引かないとがくがくする
			/*addPos.y -= (capsuleData[0].GetSegment3DData().GetPosition().v2.y - capsuleData[0].GetRadius()) -
				capsuleData[0].GetSegment3DData().GetCalcResult().boardHitPos.y;*/


				//カプセルじゃなくて線分で判定取ってるときの処理
			addPos.y += segment3DDatas[0].GetCalcResult().triangleHitPos.y - segment3DDatas[0].GetPosition().v2.y;
			//segment3DData[0] = capsuleData[0].GetSegment3DData();

			AddPosition(addPos);
			SetCameraData();

			hitGround = true;



			if (!setStartParam)
			{
				setStartParam = true;

				// 攻撃判定移動用
				// 攻撃判定をセットしたときのプレイヤーのデータを保存
				startPos = modelObjects["main"].GetPosition();
				startAngle = modelObjects["main"].GetAngle();
				startScale = modelObjects["main"].GetScale();
			}
		}
		else // そうじゃなかったら壁扱い
		{
			HitWall();
		}
	
	}

	if(typeid(*object) == typeid(Wall)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{
		HitWall();
	}


	//if (typeid(*object) == typeid(Wall) 
	//	&& collisionType == MelLib::ShapeType3D::SEGMENT)
	//{
	//	


	////	MelLib::BoardData board = GetHitBoardData();
	////	MelLib::Vector3 boardNormal(board.GetNormal());
	////	MelLib::Vector3 moveVector = GetPosition() - prePos;

	////	// テスト
	/////*	boardNormal = MelLib::Vector3(0.5,0,-0.5).Normalize();
	////	moveVector = MelLib::Vector3(-0.5, 0, 0.5);*/

	////	//// 進行方向と一致で反転
	////	if (std::signbit(boardNormal.x) == std::signbit(moveVector.x))boardNormal.x *= -1;
	////	if (std::signbit(boardNormal.z) == std::signbit(moveVector.z))boardNormal.z *= -1;

	////	float max = abs(boardNormal.x) + abs(boardNormal.z);
	////	float hirituX = abs(boardNormal.x) / max;
	////	float hirituZ = abs(boardNormal.z) / max;

	////	//float length = boardNormal.Length();
	////	float sum = abs(moveVector.x) + abs(moveVector.z);
	////	MelLib::Vector3 addPos(sum * hirituX, 0, sum * hirituZ);

	////	// 法線に応じて向きを変える
	////	if (std::signbit(boardNormal.x))addPos.x *= -1;
	////	if (std::signbit(boardNormal.z))addPos.z *= -1;
	////	

	////	AddPosition(addPos);

	//}

	// 攻撃を受けた時(体力減算はEnemyAttack側で行ってる)
	if (!isMuteki)
	{

		if (typeid(*object) == typeid(EnemyAttack)
			|| typeid(*object) == typeid(NormalEnemyAttack))
		{
			isMuteki = true;
			mutekiTimer.SetStopFlag(false);

			isStun = true;
			modelObjects["main"].SetAnimation("Stun");

			/*MelLib::Vector3 enemyToPlayer = GetPosition() - object->GetPosition();
			enemyToPlayer = enemyToPlayer.Normalize();
			AddPosition(-enemyToPlayer * 0.3f);*/
		}
	}

	if(typeid(*object) == typeid(EventFlag))
	{
		hitEventFlag = true;
	}

	if (typeid(*object) == typeid(TutorialEventFlag))
	{
		hitTutorialEventFlag = true;
	}
}

void Player::DownHP(const int power)
{
	hp -= power;
	if (hp <= 0)
	{
		hp = 0;
		isDead = true;

		modelObjects["main"].SetAnimationFrameStart();

	}
}

void Player::LifeUp(const int upNum)
{
	hp += upNum;
	if (hp > HP_MAX)
	{
		hp = HP_MAX;
	}
}
