#include "SandEffect.h"

#include<Random.h>

void SandEffect::LoadResources()
{
	MelLib::Texture::Load("Resources/Texture/Boss/SandEffect.png","sand");
}

void SandEffect::Initialize()
{
	particleSprite.Create(MelLib::Texture::Get("sand"));

	// �����_���ŐF�̔Z����ς���
	// RGB�S���ꊇ�ŉ����Ē�������
	int addColor = MelLib::Random::GetRandomNumberRangeSelect(-40, 40);
	char cAddColor = static_cast<char>(addColor);
	particleSprite.SetAddColor(MelLib::Color(cAddColor, cAddColor, cAddColor, 0));
}

void SandEffect::Update()
{
	// ��ɏオ���ē����x�グ�ď���������
	particleSprite.SetSubColor(MelLib::Color(0, 0, 0, MelLib::Color::ParToUChar(subAlpha)));

	if (subAlpha < 100.0f) 
	{
		subAlpha += FRAME_SUB_ALPHA;
	}
	else
	{
		subAlpha = 100.0f;
	}
}

std::shared_ptr<MelLib::Particle3D> SandEffect::GetNewPtr() const
{
	return std::make_shared<SandEffect>();
}
