#include "Option.h"

#include<Input.h>

Option* Option::GetInstance()
{
	static Option o;
	return &o;
}

void Option::LoadResources()
{
}

void Option::Initialize()
{
	LoadResources();
}

void Option::Update()
{

	// çÄñ⁄ïœçX
	if (MelLib::Input::PadButtonTrigger(1, MelLib::PadButton::UP)
		/*|| MelLib::Input::LeftStickUp(1, 30.0f)*/)
	{
		currentSelect--;
		if (currentSelect < 0)currentSelect = OptionMenu::NUM_MAX;
	}
	else if(MelLib::Input::PadButtonTrigger(1, MelLib::PadButton::DOWN)
		/*|| MelLib::Input::LeftStickDown(1, 30.0f)*/)
	{
		currentSelect--;
		if (currentSelect > OptionMenu::NUM_MAX)currentSelect = 0;
	}

}

void Option::Draw()
{
}
