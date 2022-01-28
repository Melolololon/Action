#pragma once
#include<GameObject.h>

class Wall : public MelLib::GameObject
{
private:
	MelLib::Sprite3D wallSpr;

	MelLib::Vector2 drawAreaLeftUp;
	MelLib::Vector2 drawAreaRightDown = 1;

	static unsigned int wallSetCount;
public:
	static void LoadResources();


	Wall(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector2& scale);

	void Update()override;
	void Draw()override;


};

