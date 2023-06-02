#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Title.h"

#include"ActionPart.h"

#include"PlayerSlush.h"
#include"JumpAttack.h"

#include"Stage.h"
#include"Ground.h"
#include"Wall.h"

#include"RecoveryEffect.h"
#include"RecoveryItem.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"
#include"NormalEnemyAttack.h"
#include"CapsuleEnemyAttack.h"
#include"JumpEnemy.h"
#include"BossAttack.h"

#include"EventFlag.h"
#include"TutorialEventFlag.h"
#include"Pause.h"


#include<LibMath.h>



// テスト
#include"SlushEffect.h"

#include"HPGauge.h"


std::unordered_map<Player::ActionType, MelLib::PadButton> Player::padConfigData =
{
	{ActionType::JUMP, MelLib::PadButton::A},
	{ActionType::ATTACK, MelLib::PadButton::X},
	{ActionType::DASH, MelLib::PadButton::B},
	{ActionType::GUARD, MelLib::PadButton::LB},
	{ActionType::DEATH_BLOW, MelLib::PadButton::Y}
};

std::unordered_map<Player::ActionType, BYTE> Player::keyboardConfigData =
{
	{ActionType::JUMP, DIK_SPACE},
	{ActionType::DASH, DIK_LSHIFT},
	{ActionType::DEATH_BLOW, DIK_LCONTROL}
};

std::unordered_map<Player::ActionType, MelLib::MouseButton> Player::mouseConfigData =
{
	{ActionType::ATTACK, MelLib::MouseButton::LEFT},
	{ActionType::GUARD, MelLib::MouseButton::RIGHT}
};

const float Player::JUMP_POWER = 3.0f;
const float Player::GROUND_HUND_VELOCITY = -30.0f;
Player* Player::pPlayer = nullptr;

const float Player::JUMP_ATTACK_DROP_SPEED = -3.0f;

std::unordered_map<PlayerSlush::AttackType, const Player::AttackData> Player::attackData =
{
	// AttackData(パワー,攻撃アニメーション終了までの時間,次の攻撃入力時間)
	{PlayerSlush::AttackType::NONE,AttackData(0,0,0 ,AttackEffect::NONE)},
	{PlayerSlush::AttackType::NORMAL_1,AttackData(3,20,10,AttackEffect::NONE)},
	{PlayerSlush::AttackType::NORMAL_2,AttackData(3,30,20,AttackEffect::NONE)},
	{PlayerSlush::AttackType::NORMAL_3,AttackData(5,30,20,AttackEffect::BE_BLOWN_AWAY)},// これのアニメーションバグるのアニメーション終わる前にキャンセルされて立ちに戻るから?
	{PlayerSlush::AttackType::DASH_1,AttackData(3,20,10,AttackEffect::BE_BLOWN_AWAY)},
	{PlayerSlush::AttackType::JUMP,AttackData(5,9999,9999,AttackEffect::BE_BLOWN_AWAY)},
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
	:GameObject("Player")
{

	//物理演算のためにseterとgeter作る?


	collisionFlag.capsule = true;
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(3.0f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 15, 0), GetPosition() + MelLib::Vector3(0, -0.1f, 0)));

	segment3DDatas["main"].resize(3);
	segment3DDatas["main"][0] = capsuleDatas["main"][0].GetSegment3DData();


	const float SEGMENT_LENGTH = 6.0f;
	const float SEGMENT_HEIGTH = 60.0f;
	// 壁との判定
	segment3DDatas["main"][1].SetPosition
	(
		MelLib::Value2<MelLib::Vector3>
		(GetPosition() + MelLib::Vector3(0, SEGMENT_HEIGTH, SEGMENT_LENGTH), GetPosition() + MelLib::Vector3(0, SEGMENT_HEIGTH, -SEGMENT_LENGTH))
	);

	segment3DDatas["main"][2].SetPosition
	(
		MelLib::Value2<MelLib::Vector3>
		(GetPosition() + MelLib::Vector3(SEGMENT_LENGTH, SEGMENT_HEIGTH, 0), GetPosition() + MelLib::Vector3(-SEGMENT_LENGTH, SEGMENT_HEIGTH, 0))
	);



	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	modelObjects["main"].Create(MelLib::ModelData::Get("Player"), "Player", nullptr);

	modelObjects["main"].SetAnimationPlayFlag(true);

#pragma region ダッシュ
	dashEasing.SetAddPar(5.0f);


#pragma endregion

	mutekiTimer.SetMaxTime(60 * 1.0f);



	// タイトル用処理
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (currentScene) 
	{
		if (typeid(*currentScene) == typeid(Title))
		{
			modelObjects["main"].SetScale(MelLib::Vector3(3));
			modelObjects["main"].SetAngle(0);
			modelObjects["main"].SetAnimation("No_Cont");
		}
		else
		{
			/*modelObjects["main"].SetScale(MelLib::Vector3(3));
			modelObjects["main"].SetAngle(0);
			modelObjects["main"].SetPosition(MelLib::Vector3(0, -17, -0));*/


			//浮き防止
			FallStart(0.0f);
			jumpStartPosition = GetPosition();
			// 落下アニメーションからスタート
			modelObjects["main"].SetAnimation("Jump_Up");
		}
	}



	pPlayer = this;
	HPGauge::SetPPlayer(this);

	jumpAttackEndTimer.SetMaxTime(60 * 0.5);


	SetCameraData();


	collisionCheckDistance = 500.0f;

	

	/*MelLib::DrawOption op;
	op.cullMode = MelLib::CullMode::NONE;
	downMaterial.Create(op, 1);
	downMaterial.SetTexture(modelObjects["main"].GetPMaterial("Zubon")->GetPTexture());
	modelObjects["main"].SetMaterial(&downMaterial);*/
}

void Player::Initialize()
{
	ShowCursor(FALSE);

	MelLib::Camera* pCamera = MelLib::Camera::Get();
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(35.0f);

}

// ジャンプ攻撃中に攻撃食らうとバグる
// Tポーズで動かなくなる
void Player::Update()
{
	preAttack = currentAttack;

	// マウスカーソル確認
	//ChangeMouseCursorShow();


	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())
	{
		if (EditMode::GetInstance()->GetIsEdit())UpdateCamera();
		
		MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
		if (typeid(*currentScene) != typeid(Title))
		{
			SetCameraData();
		}
		return;
	}

	if (isHit)isMuteki = true;
	isHit = false;

	hitEventFlag = false;
	hitTutorialEventFlag = false;

	modelObjects["main"].Update();
	damageEffect.Update();
	
	
	if(isDead)
	{
		Dead();
		return;
	}


	if (isStun)
	{
		Stun();
		UpdateCamera();
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
			UpdateCamera();
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
		UpdateCamera();

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


	// アニメーションの終了確認フラグがアニメーションのフレームを0にしても切り替わらない(次のフレームにならないとfalseにならない)
	// からいったんここに書く

	// 落下攻撃処理
//if (jumpAttackStartTimer.GetMaxOverFlag()) 
	if (modelObjects["main"].GetCurrentAnimationName() == "Jump_Attack"
		&& modelObjects["main"].GetAnimationEndFlag()
		&& currentAttack == PlayerSlush::AttackType::JUMP)
	{
		FallStart(JUMP_ATTACK_DROP_SPEED);
		jumpStartPosition = GetPosition();
		jumpAttackStartTimer.ResetTimeZero();
		jumpAttackStartTimer.SetStopFlag(true);
	}

	// ジャンプ攻撃終了
	if (modelObjects["main"].GetCurrentAnimationName() == "Jump_Attack_End"
		&& modelObjects["main"].GetAnimationEndFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		// 強制終了
		attackTimer.SetNowTime(attackData[PlayerSlush::AttackType::JUMP].time);

		modelObjects["main"].SetAnimationFrame(0);
		modelObjects["main"].SetAnimation("No_Cont");

	}

	// ジャンプ攻撃終了アニメーション
	if (jumpAttackEndTimer.GetMaxOverFlag())
	{
		modelObjects["main"].SetAnimation("Jump_Attack_End");
		modelObjects["main"].SetAnimationFrame(0);
		jumpAttackEndTimer.SetStopFlag(true);
		jumpAttackEndTimer.ResetTimeZero();

	}

	CreateAttackSlush();

	if (currentAttack == PlayerSlush::AttackType::NONE) 
	{
		Move();
		Jump();
	}
	else 
	{
		AttackMove();
	}

	preHitGround = hitGround;
	hitGround = false;

	CalcMovePhysics();


	ReturnStage();


	DeathBlow();
	if (isDeathBlow)return;


	//Guard();
	Attack();

	

	Muteki();

	UpdateCamera();
	//LockOn();

	float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	// 回転処理(仮)
	//modelObjects["main"].SetAngle(MelLib::Vector3(0, angle, 0));
	SetAngle(MelLib::Vector3(0, angle, 0));


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



}

bool Player::GetAttackChangeFrame() const
{
	return preAttack != currentAttack;
}

void Player::TitleUpdate()
{
	// 移動
	AddPosition(MelLib::Vector3(0, 0, 0.5f));


	// ジャンプ
	if (!GetIsFall())
	{
		FallStart(JUMP_POWER);
		jumpStartPosition = GetPosition();
		modelObjects["main"].SetAnimation("Jump_Up");
	}
	CalcMovePhysics();
	ChangeAnimationData();
}

MelLib::Vector3 Player::CalcPlayerVector(const MelLib::Vector3& pos)const
{
	MelLib::Vector3 p = GetPosition();
	return MelLib::Vector3::Normalize(p - pos);
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

	// ダッシュ中またはジャンプ攻撃中は移動不可
	if (isDash || currentAttack == PlayerSlush::AttackType::JUMP || isGuard)
	{
		return;
	}
	else if (currentAttack != PlayerSlush::AttackType::NONE)
	{
		return;
	}


	static const float FAST_WARK_STICK_PAR = 60.0f;
	static const float WALK_STICK_PAR = 20.0f;
	static const float MAX_FAST_WARK_SPEED = 3.0f;
	static const float MAX_WALK_SPEED = 0.5f;
	static const float FRAME_UP_FAST_WARK_SPEED = MAX_FAST_WARK_SPEED / 10;
	static const float FRAME_UP_WARK_SPEED = MAX_WALK_SPEED / 10;



	// 傾けたら移動
	if (MelLib::Input::LeftStickUp(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickDown(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickRight(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickLeft(WALK_STICK_PAR))
	{
		direction = MelLib::Input::LeftStickVector3(20.0f, MelLib::Camera::Get(), false, true);
		direction.y = 0.0f;
		MelLib::Vector3 addPos = direction;

		//ダッシュの傾き量を超えていたらダッシュ
		if (MelLib::Input::LeftStickUp(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickDown(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickRight(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickLeft(FAST_WARK_STICK_PAR))
		{
			// 上限までだんだん加算して移動速度を上げていく
			if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
			else moveSpeed = MAX_FAST_WARK_SPEED;
			addPos *= moveSpeed;

			modelObjects["main"].SetAnimation("Dash");
		}
		else
		{
			// 上限までだんだん加算して移動速度を上げていく
			if (moveSpeed < MAX_WALK_SPEED)moveSpeed += FRAME_UP_WARK_SPEED;
			else moveSpeed = MAX_WALK_SPEED;
			addPos *= moveSpeed;
			addPos *= MAX_WALK_SPEED;

			modelObjects["main"].SetAnimation("Walk");
		}


		AddPosition(addPos);
	}
	else if (MelLib::Input::KeyState(DIK_W)
		|| MelLib::Input::KeyState(DIK_A)
		|| MelLib::Input::KeyState(DIK_S)
		|| MelLib::Input::KeyState(DIK_D)) 
	{
		MelLib::Vector3 moveVector;
		// キーに応じて移動方向を設定
		if (MelLib::Input::KeyState(DIK_W)) moveVector.z += 1;
		else if (MelLib::Input::KeyState(DIK_S))moveVector.z += -1;
		if (MelLib::Input::KeyState(DIK_A)) moveVector.x += -1;
		else if (MelLib::Input::KeyState(DIK_D)) moveVector.x += 1;
		
		// カメラに合わせて回転
		moveVector = MelLib::LibMath::RotateZXYVector3(moveVector,MelLib::Camera::Get()->GetAngle());
		moveVector.y = 0.0f;

		direction = moveVector;
		
		// 加算値に向きを代入
		MelLib::Vector3 addPos = direction;

		if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
		else moveSpeed = MAX_FAST_WARK_SPEED;

		// 速度を加算
		addPos *= moveSpeed;
		AddPosition(addPos);

		modelObjects["main"].SetAnimation("Dash");
	}
	else
	{
		moveSpeed = 0.0;

		modelObjects["main"].SetAnimation("No_Cont");

		// アニメーション確認用
		//modelObjects["main"].SetAnimation("Jump_Attack");
	}


	//if (position.y <= -100)position.y = 100;

	//落下するために
	if (!GetIsFall())
	{
		jumpStartPosition = GetPosition();
		FallStart(0.0f);
	}


}

void Player::ReturnStage()
{

	// 開始位置ぴったりに戻すと、ジャンプせずに落ちた時に、
	// 落下開始時はすでに足元に足場がなく無限に落ち続けるため、対処が必要
	if(GetPosition().y <= -300)
	{
		//SetPosition(notFallPrePosition[_countof(notFallPrePosition) - 1]);
		

		//DownHP(30);


		AddPosition(MelLib::Vector3(0, 500, 0));
	}
}

void Player::AttackMove()
{

	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		AddPosition(direction * 0.3f);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		AddPosition(direction * 0.3f);
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

	if ((MelLib::Input::PadButtonTrigger(padConfigData[ActionType::DASH]) 
		|| MelLib::Input::KeyState(keyboardConfigData[ActionType::DASH]))
		&& !isDash
		&& currentAttack == PlayerSlush::AttackType::NONE
		&& !isGuard)
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
		|| isDash
		|| isGuard
		|| isStun)return;

	if (MelLib::Input::PadButtonTrigger(padConfigData[ActionType::JUMP])
		|| MelLib::Input::KeyState(keyboardConfigData[ActionType::JUMP]))
	{
		FallStart(JUMP_POWER);
		jumpStartPosition = GetPosition();
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
	//if (isDrop)return;

	if (attackTimer.GetMaxOverFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		attackTimer.ResetTimeZero();

		attackTimer.SetStopFlag(true);
	}

	//ifの2行目のタイマー確認は、コンボ終了後にNONEにするため、その攻撃中に入らないようにするために書いてる
	if ((MelLib::Input::PadButtonTrigger(padConfigData[ActionType::ATTACK])
		|| MelLib::Input::MouseButtonTrigger(mouseConfigData[ActionType::ATTACK]))
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

			modelObjects["main"].SetAnimation("DashAttack");

			isDash = false;
			DashEnd();
		}
		else if (preHitGround)// 通常攻撃
		{
			currentAttack = PlayerSlush::AttackType::NORMAL_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");

		}
		else if (GetIsFall() && abs(GetPosition().y - jumpStartPosition.y) >= 10 )// 空中攻撃
		{
			currentAttack = PlayerSlush::AttackType::JUMP;
			FallEnd();
			modelObjects["main"].SetAnimation("Jump_Attack");
			modelObjects["main"].SetAnimationFrame(0);
			jumpAttackStartTimer.SetStopFlag(false);
			jumpAttackStartTimer.SetMaxTime(60 * 0.5);
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
	const int CURRENT_FRAME = modelObjects["main"].GetAnimationFrame();
	const int MAX_FRAME = modelObjects["main"].GetAnimationFrameCount();
	bool addFrame = false;

	// 攻撃アニメーションは全部2倍速だからCURRENT_FRAMEのifは偶数にする
	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:

		if (CURRENT_FRAME == 14)
		{
			// MAX_FRAME / 2の/2はアニメーション速度　ここあとで関数で取得するように変更する
			pRigthSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"],
					startPos, startAngle, startScale, false,"N1");
			addFrame = true;
		}
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		if (CURRENT_FRAME == 24)
		{

			pPSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"], startPos, startAngle, startScale, true, "N2");

			addFrame = true;
		}
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		if (CURRENT_FRAME == 18)
		{
			pPSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"], startPos, startAngle, startScale, true, "N3_L");

			pRigthSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"], startPos, startAngle, startScale, false, "N3_R");

			addFrame = true;
		}
		break;
	case PlayerSlush::AttackType::DASH_1:
		if (CURRENT_FRAME == 14)
		{
			pRigthSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME, modelObjects["main"], startPos, startAngle, startScale, false, "D1");

			addFrame = true;
		}
		break;
	default:
		break;
	}

	if (addFrame)MelLib::GameObjectManager::GetInstance()->AddObject(pPSlush);
	if (addFrame)MelLib::GameObjectManager::GetInstance()->AddObject(pRigthSlush);
}

void Player::CheckEraseSlush()
{
	if (!GetAttackChangeFrame())return;

	// 切り替わりで削除
	if (pPSlush)
	{
		pPSlush->Erase();
		pPSlush.reset();
	}
	if (pRigthSlush)
	{
		pRigthSlush->Erase();
		pRigthSlush.reset();
	}
}

void Player::DeathBlow()
{


	/*if (!isDeathBlow)return;

	if (modelObjects["main"].GetAnimationEndFlag()) 
	{
		std::string aniName = "DeathBlowAttack_";
		switch (currentAttack)
		{

			case PlayerSlush::AttackType::DEATH_BLOW_1:
				aniName += "2";
				break;

			case PlayerSlush::AttackType::DEATH_BLOW_2:
				aniName += "3";
				break;
			case PlayerSlush::AttackType::DEATH_BLOW_3:
				aniName = "No_Cont";
				break;

			default:
				aniName += "1";
				break;
		}

		modelObjects["main"].SetAnimation(aniName);
		modelObjects["main"].SetAnimationFrame(0);
	}*/


}


void Player::Guard()
{
	// 攻撃してない時にボタン押してたらガード
	if (MelLib::Input::PadButtonState(padConfigData[ActionType::GUARD])
		&& currentAttack == PlayerSlush::AttackType::NONE
		&& !isDash
		&& !isStun)
	{
		isGuard = true;
		modelObjects["main"].SetAnimation("Guard");
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
	if (isBeBlownAway) 
	{
		static const float BE_BLOWN_AWAY_SPEED = 0.75f;
		AddPosition(beBlownAwayVector * BE_BLOWN_AWAY_SPEED);

		if (!GetIsFall())isBeBlownAway = false;
	}

	if(modelObjects["main"].GetAnimationEndFlag())
	{
		isStun = false;
	}
}

void Player::Dead()
{
	modelObjects["main"].SetAnimation("Dead");
	ChangeAnimationData();

	//if (!showMouse)
	//{
	//	ShowCursor(TRUE);
	//}
}

void Player::UpdateCamera()
{

	// マウスカーソルが見えている(ポーズ中など)だったらreturn
	if (showMouse)return;

	// カメラ回転処理
	RotCamera();
	// カメラに値セット
	SetCameraData();

}

void Player::ChangeMouseCursorShow()
{
	if (!setStartParam)return;

	if(Pause::GetInstance()->PauseTiming())
	{
		showMouse = !showMouse;
		ShowCursor(showMouse);
	}
}

void Player::DrawDamageEffect()
{
	damageEffect.Draw();
}

void Player::RotCamera()
{

	if (lockOn)return;

	//スティックの倒し具合でスピード変える
	//緩急あったほうがいい?だんだん加速してく。早めにスピードマックスになる

	//メインカメラのポインタ取得
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0.0f;
	const float MAX_CAMERA_ANGLE_X = 30.0f;
	const float MIX_CAMERA_ANGLE_X = -5.0f;

	//最大値は一旦const(あとで設定で最大値を変えられるようにする)
	const float MAX_CAMERA_SPEED = 3.0f;
	const float FRAME_UP_CAMERA_SPEED = MAX_CAMERA_SPEED / 10;

	if (!MelLib::Input::RightStickLeft(30.0f)
		&& !MelLib::Input::RightStickRight(30.0f)
		&& !MelLib::Input::RightStickUp(30.0f)
		&& !MelLib::Input::RightStickDown(30.0f))cameraSpeed = 0.0f;
	else cameraSpeed += FRAME_UP_CAMERA_SPEED;

	// パッド用
	if (MelLib::Input::RightStickLeft(30.0f))addCameraAngle.y = -cameraSpeed;
	else if (MelLib::Input::RightStickRight(30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(30.0f))addCameraAngle.x = -cameraSpeed;
	else if (MelLib::Input::RightStickDown(30.0f))addCameraAngle.x = cameraSpeed;
	
	bool padInput = addCameraAngle != 0.0f;

#pragma region キーボード用カメラ操作
	
	if (!padInput) 
	{

		// ウィンドウサイズを取得
		MelLib::Vector2 winSize(MelLib::Library::GetWindowWidth(), MelLib::Library::GetWindowHeight());
		
		MelLib::Vector2 winPos = MelLib::Library::GetWindowPosition();


		MelLib::Vector2 winHarf = winSize / 2;

		// ウィンドウが動いていることを考慮して座標分加算
		winHarf += winPos;

		// マウスのクライアント座標を取得
		MelLib::Vector2 mousePos = MelLib::Input::GetMousePosition() + winPos;

		// ウィンドウ動かすとバグるからウィンドウ座標取得して補正して対策するかフルスクボーダーレスにするのがいいかも
		// ウィンドウのバー分座標がずれるため,加算して調整
		mousePos.x += 8.0f;
		mousePos.y += 22.0f;

		// カメラが動いたらセット
		if (!MelLib::LibMath::Difference(mousePos.x, winHarf.x, 4)
			|| !MelLib::LibMath::Difference(mousePos.y, winHarf.y, 4))
		{
			cameraSpeed += FRAME_UP_CAMERA_SPEED;

			// ここでカメラの移動量を計算
			MelLib::Vector2 moveVector = MelLib::Vector2(mousePos - winHarf).Normalize() * cameraSpeed;

			// パッドの速度を使いまわすと遅いため、数値を掛けて速度を上げる
			const float MUL_CAMERA_SPEED = 20.0f;
			moveVector *= MUL_CAMERA_SPEED;

			// 角度をセット
			addCameraAngle = MelLib::Vector3(moveVector.y, moveVector.x, 0);

			frameMousePosition = mousePos;
		}
		else cameraSpeed = 0.0f;

		// 中心にカーソルを移動
		if (!showMouse)MelLib::Input::SetMouseFixedPosition(winHarf);

		
	}
#pragma endregion

	if (cameraSpeed >= MAX_CAMERA_SPEED)cameraSpeed = MAX_CAMERA_SPEED;

	cameraAngle += addCameraAngle;
	if (cameraAngle.x >= MAX_CAMERA_ANGLE_X)cameraAngle.x = MAX_CAMERA_ANGLE_X;
	if (cameraAngle.x <= MIX_CAMERA_ANGLE_X)cameraAngle.x = MIX_CAMERA_ANGLE_X;

	pCamera->SetAngle(cameraAngle);
	
}


void Player::SetCameraData()
{
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	if (lockOn)
	{
	}
	else 
	{
		MelLib::Vector3 targetPos = GetPosition() + MelLib::Vector3(0, 15, 0);

		// 落下中のカメラ制御
		if (isDrop)
		{
			MelLib::Vector3 jumpUpCameraVector = 0;
			targetPos.y -= (GetPosition().y - jumpStartPosition.y) / 7;
		}

		pCamera->SetRotateCriteriaPosition(targetPos);
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

void Player::StartBeBlownAway(const MelLib::Vector3& hitObjPos)
{
	// 吹っ飛びアニメーション
	modelObjects["main"].SetAnimation("BeBlownAway");

	// 吹っ飛び(XZ)
	MelLib::Vector3 attackPos = hitObjPos;
	beBlownAwayVector = -(attackPos - GetPosition());
	beBlownAwayVector.y = 0;
	isBeBlownAway = true;

	// 吹っ飛び(Y)
	FallStart(2.0f);
}


void Player::Draw()
{
	AllDraw();
	//modelObjects["main"].Draw();
}

void Player::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	// 敵と当たった時の処理
	for (const auto& tag : object.GetTags()) 
	{
		if(tag == "Enemy" || tag == "StageObject")
		{
			// 真上から移動せずに乗っかっても押し出すようにする
			// 敵から自分へのベクトルを求め、その方向に押し出す。
			// 真上の場合は、適当に押し出す
			MelLib::Vector3 enemyToPlayer = GetPosition() - object.GetPosition();
			enemyToPlayer = enemyToPlayer.Normalize();

			if(GetIsFall())
			{
				AddPosition(MelLib::Vector3(enemyToPlayer.x, 0, enemyToPlayer.z) * 5.0f);
				
			}
			else if (prePosition - GetPosition() == 0)
			{
				if (typeid(object) == typeid(JumpEnemy))
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
	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{
		// ステージの三角形の法線取得
		MelLib::Vector3 normal = GetHitTriangleData().GetNormal();
		
		// 一定以上だったら床扱い
		if(normal.y >= 0.6f)
		{
			// 0番以外の判定(床用判定)意外と当たったらreturn
			//if (arrayNum != 0)return;

			// 落下距離をリセット
			dropStartPosY = 0.0f;

			// 着地処理
			if (!hitGroundNotMove)
			{
				float velocityY = GetVelocity().y;

				// 高いところから落ちたら手をつくため、動けなくする
				if (velocityY <= GROUND_HUND_VELOCITY)
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


			// カプセルじゃなくて線分で判定取ってるときの処理
			float tHitPosY = GetSegmentCalcResult().triangleHitPos.y;
			addPos.y += tHitPosY - segment3DDatas["main"][0].GetPosition().v2.y;
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


			if (modelObjects["main"].GetCurrentAnimationName() == "Jump_Attack" && jumpAttackEndTimer.GetNowTime() == 0)
			{
				jumpAttackEndTimer.SetStopFlag(false);

				// 攻撃判定追加
				MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<JumpAttack>(GetPosition(),70.0f));
			}

		}
		else // そうじゃなかったら壁扱い
		{
			HitWall();
		}
	
	}

	if(typeid(object) == typeid(Wall)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{
		HitWall();
	}


	// エフェクト追加
	if (typeid(object) == typeid(RecoveryItem)) 
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<RecoveryEffect>(GetPosition()));
	}


	// 攻撃を受けた時(体力減算はEnemyAttack側で行ってる)
	if (!isMuteki)
	{
		// 通常
		if (typeid(object) == typeid(EnemyAttack)
			|| typeid(object) == typeid(NormalEnemyAttack)
			|| typeid(object) == typeid(CapsuleEnemyAttack))
		{
			damageEffect.StartEffect();
			isHit = true;
			mutekiTimer.SetStopFlag(false);

			
			isStun = true;

			for (auto& tag : object.GetTags())
			{
				if (EnemyAttack::GetAttackType(tag) == EnemyAttack::AttackType::BE_BLOWN_AWAY)
				{
					StartBeBlownAway(object.GetPosition());
				}
				else if(!isDrop)
				{
					modelObjects["main"].SetAnimation("Stun");
				}
			}

		}

		// 吹っ飛び
		if (typeid(object) == typeid(BossAttack))
		{
			damageEffect.StartEffect();
			isHit = true;
			mutekiTimer.SetStopFlag(false);

			isStun = true;

			StartBeBlownAway(object.GetPosition());
		}
		
		
	}

	if(typeid(object) == typeid(EventFlag))
	{
		hitEventFlag = true;
	}

	if (typeid(object) == typeid(TutorialEventFlag))
	{
		hitTutorialEventFlag = true;
	}
}

std::shared_ptr<MelLib::GameObject> Player::GetNewPtr() 
{
	return std::make_shared<Player>();
}


void Player::DownHP(const int power)
{
	if (isMuteki)return;

	hp -= power;
	if (hp <= 0)
	{
		hp = 0;
		isDead = true;

		modelObjects["main"].SetAnimationFrameStart();
		modelObjects["main"].SetAnimationEndStopFlag(true);
		modelObjects["main"].SetAnimation("Dead");
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
