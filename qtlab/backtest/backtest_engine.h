#ifndef BACKTEST_ENGINE_H
#define BACKTEST_ENGINE_H

#include "repo/daily_bar_repo.h"
#include "strategy/strategy.h"

#include <memory>

class Backtest_Engine
{
public:
    explicit Backtest_Engine(std::unique_ptr<Daily_Bar_Repo> repo,
                             std::unique_ptr<Strategy> strategy);

private:
    std::unique_ptr<Daily_Bar_Repo> repo_;
    std::unique_ptr<Strategy> strategy_;
};

#endif
