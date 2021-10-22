#pragma once
#include <Scene.h>
class Title :
    public MelLib::Scene
{
private:
	void LoadResources();

public:
	void Initialize()override;//‰Šú‰»
	void Update()override;
	void Draw()override;
	void Finalize()override;//I—¹ˆ—
	Scene* GetNextScene()override;//Ÿ‚ÌƒV[ƒ“
};

