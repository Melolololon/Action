#pragma once
#include <Scene.h>
class Title :
    public MelLib::Scene
{
private:
	void LoadResources();

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��
};

