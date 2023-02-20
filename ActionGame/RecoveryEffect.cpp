#include "RecoveryEffect.h"

#include<Random.h>

void RecoveryEffect::LoadResources()
{
	MelLib::Texture::Load("Resources/Texture/Effect/H_Effect/HealEffect.png","RecoveryEffect_01");
	MelLib::Texture::Load("Resources/Texture/Effect/H_Effect/HealStick.png","RecoveryEffect_02");
}

RecoveryEffect::RecoveryEffect(const MelLib::Vector3& pos)
	:GameObject("RecoveryEffect")
{
	for (int i = 0; i < SPRITE_NUM; i++) 
	{
		spriteRing[i].Create(MelLib::Texture::Get("RecoveryEffect_01"));
		spriteRing[i].SetPosition(pos+MelLib::Random::GetRandomVector3NumberRangeSelect(-5, 5, 2));
		spriteRing[i].SetScale(4);
		spriteStick[i].Create(MelLib::Texture::Get("RecoveryEffect_02"));
		spriteStick[i].SetPosition(pos + MelLib::Random::GetRandomVector3NumberRangeSelect(-5, 5, 2));
		spriteStick[i].SetScale(4);


		spriteRing[i].SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));
		spriteRing[i].SetBillboardFlag(true, true, true);
		spriteStick[i].SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));
		spriteStick[i].SetBillboardFlag(true, true, true);
	}
}

void RecoveryEffect::Initialize()
{
	deadTimer.SetMaxTime(60 * 1);
	deadTimer.SetStopFlag(false);
}

void RecoveryEffect::Update()
{
	for (int i = 0; i < SPRITE_NUM; i++)
	{
		spriteRing[i].SetPosition(spriteRing[i].GetPosition() + MelLib::Vector3(0, 0.1f, 0));
		spriteStick[i].SetPosition(spriteStick[i].GetPosition() + MelLib::Vector3(0, 0.1f, 0));
	}

	static const float UP_ALPHA_NUM = 5;
	if (deadTimer.GetNowTime() >= deadTimer.GetMaxOverFlag() - 60 * 0.2) 
	{
		subAlpha -= UP_ALPHA_NUM;
	}
	else
	{
		subAlpha += UP_ALPHA_NUM;
	}

	if (subAlpha >= 100.0f)subAlpha = 100.0f;
	else if(subAlpha <= 0.0f)subAlpha = 0.0f;

	if (deadTimer.GetMaxOverFlag()) eraseManager = true;
}

void RecoveryEffect::Draw()
{
	for (int i = 0; i < SPRITE_NUM; i++)
	{
		spriteRing[i].SetSubColor(MelLib::Color(0,0,0,MelLib::Color::ParToUChar(subAlpha)));
		spriteStick[i].SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));
		spriteRing[i].Draw();
		spriteStick[i].Draw();
	}
}
