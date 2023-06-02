#include "PlayerDamageEffect.h"

PlayerDamageEffect::PlayerDamageEffect()
{
	spr.Create(MelLib::Color(255, 0, 0, 0));
	spr.SetScale(2000);
	alpha.SetAddPar(3.0f);
	alpha.SetEnd(100.0f);
}

void PlayerDamageEffect::Update()
{
	if (!startEffect)return;

	if (returnAlpha) 
	{
		alpha.SetAddPar(-3.0f);
		if (alpha.GetValue() <= 0)
		{
			alpha.SetPar(0.0f);
			returnAlpha = false;
			startEffect = false;
		}
	}
	else
	{
		alpha.SetAddPar(3.0f);
		if (alpha.GetPar() >= 100.0f)
		{
			alpha.SetPar(100.0f);
			returnAlpha = true;
		}
	}

	alpha.EaseOut();

	float setAlpha = alpha.GetValue();
	if (setAlpha >= 100.0f)setAlpha = 100.0f;
	else if(setAlpha <= 0.0f)setAlpha = 0.0f;
	spr.SetAddColor(MelLib::Color(0, static_cast<unsigned char>(setAlpha)));
}

void PlayerDamageEffect::Draw()
{
	if (!startEffect)return;
	spr.Draw();
}
