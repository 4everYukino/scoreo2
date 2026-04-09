#ifndef TRADE_SIGNAL_H
#define TRADE_SIGNAL_H

enum class Trade_Action
{
    HOLD,
    BUY,
    SELL,
};

struct Trade_Signal
{
    Trade_Action action = Trade_Action::HOLD;

    unsigned long shares = 0;
};

#endif
