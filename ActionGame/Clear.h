#pragma once
#include <Scene.h>
class Clear :
    public MelLib::Scene
{
private:

public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��
};
