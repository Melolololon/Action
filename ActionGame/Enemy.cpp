#include "Enemy.h"

#include<LibMath.h>
#include<Collision.h>
#include<GameObjectManager.h>

#include"EnemyAttack.h"

#include"Ground.h"
#include"Stage.h"

#include"EditMode.h"
#include"Pause.h"

Player* Enemy::pPlayer;
std::vector<std::vector<std::vector<MelLib::AStarNode>>> Enemy::nodes;

Enemy::Enemy(const MelLib::Vector3& pos, const unsigned int hp, const float moveSpeed, const std::string& modelName) :
	hp(hp)
	, moveSpeed(moveSpeed)
{
	SetPosition(pos);


	if (modelName == "")
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
		modelObjects["main"].SetScale(5);
	}
	else
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(modelName), nullptr);
	}
	modelObjects["main"].SetPosition(pos);

	routeSearchTimer.SetMaxTime(1);
	routeSearchTimer.SetStopFlag(false);

	/*for (int i = 0; i < _countof(routeObj); i++)
	{
		routeObj[i].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
		routeObj[i].SetScale(MelLib::Vector3(nodes[0][0][0].size.x,1, nodes[0][0][0].size.z));
	}*/

	mutekiTimer.SetMaxTime(60 * 0.2);

	tags.push_back(typeid(Enemy).name());


	attackTimer.SetStopFlag(false);

	tags.push_back("Enemy");

	// �����h�~
	FallStart(0.0f);
}

void Enemy::Update()
{


}

void Enemy::Draw()
{
	modelObjects["main"].Draw();
}

void Enemy::Stun()
{
	// �d���A�j���[�V�����I����false��
	if(modelObjects["main"].GetAnimationEndFlag())
	{
		isStun = false;
		//modelObjects["main"].SetAnimation("No_Move");
	}

	MelLib::Vector3 playerPos = pPlayer->GetPosition();
	MelLib::Vector3 myToPlayer(playerPos - GetPosition());
	myToPlayer = myToPlayer.Normalize();

	// �m�b�N�o�b�N
	AddPosition(-myToPlayer * 0.3f);
}

void Enemy::Dead()
{
	// �|���A�j���[�V�������I�������^�C�}�[������
	if(modelObjects["main"].GetAnimationEndFlag())
	{
		// ���S�A�j���[�V�����I����̉��b��ɏ�����
		static const int ERASE_TIME = 60 * 1;
		eraseTimer.SetMaxTime(ERASE_TIME);
		eraseTimer.SetStopFlag(false);
	}

	if(eraseTimer.GetMaxOverFlag())
	{
		eraseManager = true;

		// �����ɃG�t�F�N�g�Ȃǂ�ǉ����鏈�����L�q

	}
}

void Enemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	if (isDead)return;

	std::string n = typeid(*object).name();
	if (typeid(*object) == typeid(PlayerSlush) && !isMuteki)
	{

		// �v���C���[���猻�݂̍U���̍U���͂��擾���A�̗͂����炷
		hp -= pPlayer->GetCurrentAttackPower();

		// ���G��
		isMuteki = true;
		mutekiTimer.SetStopFlag(false);

		// �d������
		isStun = true;
		modelObjects["main"].SetAnimation("Stun");
		modelObjects["main"].SetAnimationFrameStart();

		// �U�������I��
		isAttack = false;

		// 0�ɂȂ�������ꏈ��
		if (hp <= 0)
		{
			isDead = true;

			modelObjects["main"].SetAnimation("Dead");
			modelObjects["main"].SetAnimationFrameStart();
			modelObjects["main"].SetAnimationEndStopFlag(true);
		}
	}

	if (typeid(*object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//�����グ�����I��
		FallEnd();

		addPos.y += segment3DData[0].GetCalcResult().triangleHitPos.y - segment3DData[0].GetPosition().v2.y;
	
		AddPosition(addPos);
	}
}

void Enemy::SetAStarNodeDatas
(
	const MelLib::Vector3& leftDownFrontPos,
	const MelLib::Vector3& rightUpBackPos,
	const MelLib::Value3<UINT>& nodeNum,
	const std::vector<MelLib::BoxData>& datas,
	const std::vector<UINT>& costs
)
{
	MelLib::RouteSearch::SetNodePosition(leftDownFrontPos, rightUpBackPos, nodeNum, nodes);
	MelLib::RouteSearch::SetHitObjectFlag(datas, nodes);
	MelLib::RouteSearch::SetCost(datas, costs, nodes);
}

void Enemy::AddRouteVector()
{

	if (MelLib::LibMath::CalcDistance3D(routeVectors[currentMoveVector], GetPosition()) <= 0.3f
		&& currentMoveVector + 1 != routeVectors.size())
	{
		currentMoveVector++;
	}

	if (currentMoveVector != routeVectors.size())
	{
		if (slowMove)
		{
			AddPosition(routeVectors[currentMoveVector] * moveSpeed * 0.25f);
		}
		else
		{
			AddPosition(routeVectors[currentMoveVector] * moveSpeed);
		}
	}
}

bool Enemy::CheckPlayerDistance(const float distance)
{
	MelLib::Vector3 playerPos = pPlayer->GetPosition();

	return MelLib::LibMath::CalcDistance3D(playerPos, GetPosition()) < distance;
}

bool Enemy::CalcPlayerRoute()
{

	// ��
	routeVectors.clear();

	static const float MOVE_DISTANCE = 400.0f;
	if (MelLib::LibMath::CalcDistance3D(pPlayer->GetPosition(), GetPosition()) >= MOVE_DISTANCE)
	{
		return false;
	}



	routeVectors.resize(1, (pPlayer->GetPosition() - GetPosition()).Normalize());
	AddRouteVector();
	return true;

	// ���t���[���v�Z����Ƃ�����������(�t���[�����ƂɌ������ς�邩��)���t���[���ł�������
	// �m�[�h�ɒH������Ƃ��x�X�g?
	// �v���C���[���ړ�������ɂ���?

	// �f�o�b�O�p
	std::vector<MelLib::Vector3>routeNodePos;

	MelLib::Vector3 playerPos = pPlayer->GetPosition();
	MelLib::Vector3 myToPlayer;
	bool straightMove = MelLib::RouteSearch::CheckStraightMove(GetPosition(), playerPos, nodes, 4.0f, myToPlayer);

	if (!straightMove && routeSearchTimer.GetMaxOverFlag() || routeVectors.size() == 0)
	{
		bool result = MelLib::RouteSearch::CalcRoute(GetPosition(), playerPos, nodes, &routeVectors, &routeNodePos);
		routeSearchTimer.ResetTimeZero();
	}

	if (straightMove)
	{

		routeVectors.clear();
		routeVectors.resize(1, myToPlayer);
		routeVectors[0] = MelLib::Vector3(myToPlayer.x, 0, myToPlayer.z);
	}

	AddRouteVector();


	for (int i = 0; i < routeNodePos.size(); i++)
	{
		routeObj[i].SetPosition(routeNodePos[i]);
		routeObj[i].Draw();
	}

	// �΂߂ɃW�O�U�O�ɂȂ錴���́A�΂߂Ɛ^�������̃R�X�g�������Ƃ��ɁA�΂߂��I�΂��Ƃ������邩��
	// �S�[���ɋ߂��m�[�h��D��I�ɒʂ�悤�ɂ���?(�R�X�g�����Ŕ��f���Ȃ��悤�ɂ���?)
}

void Enemy::CheckMutekiEnd()
{
	if (mutekiTimer.GetMaxOverFlag())
	{
		mutekiTimer.ResetTimeZero();

		mutekiTimer.SetStopFlag(true);

		isMuteki = false;
	}
}

void Enemy::AttackStart()
{
	isAttack = true;
	attackTimer.SetStopFlag(false);

	// �U���A�j���[�V�����Z�b�g
	modelObjects["main"].SetAnimation("Attack");
}

void Enemy::CheckAttackEnd()
{
	//if (attackTimer.GetMaxOverFlag())

	// �U�����ɃA�j���[�V�������I�������U���I��
	if (isAttack && modelObjects["main"].GetAnimationEndFlag())
	{
		isAttack = false;
		attackTimer.ResetTimeZero();
		//attackTimer.SetStopFlag(true);
	}
}

void Enemy::RotModel()
{
	// �U�����͉�]���Ȃ�
	if (isAttack || routeVectors.size() == 0)return;

	//direction = (pPlayer->GetPosition() - GetPosition()).Normalize();

	//// ��]
	//float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	//modelObjects["main"].SetAngle(MelLib::Vector3(28, angle, 0));

	// �i�s�����Ɍ����悤��
	direction = routeVectors[0].Normalize();
	
	// ��]
	float directionAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	float preAngle = modelObjects["main"].GetAngle().y;
	
	// �v���C���[�̌����̊p�x�ƍ��̊p�x��90�x�ȓ������������������
	if (!MelLib::LibMath::AngleDifference(directionAngle, preAngle, 90))slowMove = true;
	else slowMove = false;

	// �v���C���[�̌����̊p�x�ƍ��̊p�x��5�x�ȓ���������return
	if (MelLib::LibMath::AngleDifference(directionAngle, preAngle, 5))return;

	float setAngle = 0.0f;
	
	// 1�t���[���̉�]��
	static const float ROT_ANGLE = 3.0f;

	// ����180�ȏ�œ���
	if (abs(directionAngle - preAngle) > 180) preAngle += 360;

	if (directionAngle > preAngle)setAngle = preAngle + ROT_ANGLE;
	else setAngle = preAngle - ROT_ANGLE;

	if (setAngle >= 360)setAngle -= 360;

	modelObjects["main"].SetAngle(MelLib::Vector3(0, setAngle, 0));
}
