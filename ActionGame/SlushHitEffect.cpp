#include "SlushHitEffect.h"

SlushHitEffect::SlushHitEffect(const MelLib::Vector3& pos)
	:GameObject("SlushHitEffect")
{
	changeTimer.SetMaxTime(60 * 0.05f);
	changeTimer.SetStopFlag(false);
	effectSprite.Create(MelLib::Texture::Get("slushEffect_0"));
	effectSprite.SetScale(10);
	effectSprite.SetPosition(pos);
	effectSprite.SetBillboardFlag(true,true,true);
}

void SlushHitEffect::Initialize()
{
}

void SlushHitEffect::Update()
{
	if (changeTimer.GetMaxOverFlag()) 
	{
		if (!ret && textureNum == 3)
		{
			ret = true;
		}

		if(!ret)textureNum++;
		else textureNum--;

		

		if (textureNum == -1 && ret)
		{
			eraseManager = true;
			return;
		}

		changeTimer.ResetTimeZero();
		effectSprite.SetTexture(MelLib::Texture::Get("slushEffect_" + std::to_string(textureNum)));
	}

}

void SlushHitEffect::Draw()
{
	//effectSprite.Draw();
}
