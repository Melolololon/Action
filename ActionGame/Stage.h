#pragma once
#include <GameObject.h>

// ステージ
class Stage :
    public MelLib::GameObject
{
private:
    void SetStageData(unsigned int stageNum);

    const int STAGE_NUM;
public:
    static void LoadResources(unsigned int stageNum);

    Stage(unsigned int stageNum);

    void Draw()override;
    std::shared_ptr<GameObject> GetNewPtr()override;
};

