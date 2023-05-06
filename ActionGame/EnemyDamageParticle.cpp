#include "EnemyDamageParticle.h"

#include<Random.h>

const std::string EnemyDamageParticle::PARTICLE_NAME = "EnemyDamageParticle";

EnemyDamageParticle::EnemyDamageParticle(const MelLib::Vector3& pos, const MelLib::Vector3& vec)
	:GameObject(PARTICLE_NAME)
	, moveVector(vec)
{
	//spr.Create(MelLib::Texture::Get(PARTICLE_NAME));
	spr.Create(MelLib::Color(255,255,255,255));
	spr.SetPosition(pos);
	spr.SetBillboardFlag(true, true, true);

	moveSpeed = MelLib::Random::GetRandomFloatNumberRangeSelect(0.2f, 1.5f, 1);

	deadTimer.SetMaxTime(60.0f * 1);
}

EnemyDamageParticle::~EnemyDamageParticle()
{
}

void EnemyDamageParticle::Initialize()
{
}

void EnemyDamageParticle::Update()
{
	spr.SetPosition(spr.GetPosition() + moveVector * moveSpeed);
	moveVector.y -= downSpeed;
	downSpeed *= 2;

	if (deadTimer.GetMaxOverFlag())eraseManager = true;
}

void EnemyDamageParticle::Draw()
{
	spr.Draw();
}

void EnemyDamageParticle::LoadResources()
{
	//MelLib::Texture::Load("", PARTICLE_NAME);
}

