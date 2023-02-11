#pragma once
#include <GameObject.h>

// ブレンダーで床と壁分離するの面倒だけど、まとめて読み込むと無駄な当たり判定が増える
// 法線で床と壁か判定する処理あるから、それを使って壁の判定を消せばいい?それで、壁の判定は今まで通り個別で用意

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

