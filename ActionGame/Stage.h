#pragma once
#include <GameObject.h>

// �X�e�[�W
class Stage :
    public MelLib::GameObject
{
private:
    void SetStageData(unsigned int stageNum);
public:
    static void LoadResources(unsigned int stageNum);

    Stage(unsigned int stageNum);

    void Draw()override;

};

