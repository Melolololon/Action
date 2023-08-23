#include "TestSandParticle.h"
#include"Random.h"
#include"LibMath.h"

TestSandParticle::TestSandParticle(const MelLib::Vector3 pos):GameObject("Sand")
{
	particleSprite.Create(MelLib::Texture::Get("sand"));
	particleSprite.SetPosition(pos);
}

void TestSandParticle::Initialize()
{
	particleSprite.SetScale(10);
	particleSprite.SetBillboardFlag(true, true, true);

	// �����_���ŐF�̔Z����ς���
	// RGB�S���ꊇ�ŉ����Ē�������
	//int addColor = MelLib::Random::GetRandomNumberRangeSelect(-40, 40);
	//char cAddColor = static_cast<char>(addColor);
	//particleSprite.SetAddColor(MelLib::Color(cAddColor, cAddColor, cAddColor, 0));
}

void TestSandParticle::Update()
{// ��ɏオ���ē����x�グ�ď���������

	// ��ɕ����オ�鏈��
	particleSprite.SetPosition
	(MelLib::LibMath::FloatDistanceMoveVector3(particleSprite.GetPosition(), { 0,1,0 }, FRAME_MOVE_DIS));

	// ������������
//	particleSprite.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));

	if (subAlpha < 100.0f)
	{
		subAlpha += FRAME_SUB_ALPHA;
	}
	else
	{
		subAlpha = 100.0f;
	}
}

void TestSandParticle::Draw()
{
	particleSprite.Draw();
}
