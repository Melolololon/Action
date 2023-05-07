#pragma once
#include<GameObject.h>

class Wall : public MelLib::GameObject
{
private:
	/*MelLib::Sprite3D wallSpr;

	MelLib::Vector2 drawAreaLeftUp;
	MelLib::Vector2 drawAreaRightDown = 1;*/

	static MelLib::ADSAMaterial material;

	static unsigned int wallSetCount;

	// ï«ÇÃî‘çÜ(í·Ç¢Ç‚Ç¬Ç©ÇÁè¡Ç¶ÇƒÇ¢Ç≠)
	MelLib::GuiInt wallNum;

	bool deleteFlag = false;
public:

	static void LoadResources();

	Wall();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	std::shared_ptr<MelLib::GameObject>GetNewPtr()override;

};

