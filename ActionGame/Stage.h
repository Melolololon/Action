#pragma once
#include <GameObject.h>

// ステージ
class Stage :
    public MelLib::GameObject
{
private:
    void SetStageData(unsigned int stageNum);

    // ステージ番号
    // 0はテストステージ
    const int STAGE_NUM;


public:
    static void LoadResources(unsigned int stageNum);

    Stage(unsigned int stageNum);
    void Initialize()override;
    void Draw()override;
    std::shared_ptr<GameObject> GetNewPtr()override;
};

