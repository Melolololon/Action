#include "EnemyDeadCounter.h"

EnemyDeadCounter* EnemyDeadCounter::GetInstance()
{
    static EnemyDeadCounter e;
    return &e;
}
