#ifndef LAURENT_FINAL_PROJECT_MARKETSIMULATOR_H
#define LAURENT_FINAL_PROJECT_MARKETSIMULATOR_H
#include "AppBase.h"
#include "Order.h"
#include <set>
#include <queue>
#include <string>
#include <iostream>

class MarketSimulator : public AppBase {
public:
    unsigned execution_id;
    std::set<std::string> list_symbols;
    std::vector<std::string> filled_orders;
    double pnl = 0;
    int count_buy = 0;
    int count_sell = 0;
    int print_counter = 0;
    MarketSimulator(
             std::queue<Order> &strategy_to_ordermanager_,
             std::queue<ExecutionOrder> &ordermanager_to_strategy_,
             std::queue<Order> &ordermanager_to_simulator_,
             std::queue<ExecutionOrder> &simulator_to_ordermanager_,
             std::queue<BookUpdate> &bookbuilder_to_strategy_
    );
    bool handle_order();

    virtual void start() {is_working=true;}
    virtual void stop() {is_working=false;}
    double get_pnl();
    int getCountBuy(){return count_buy;}
    int getCountSell(){return count_sell;}
    void reset_position();

};


#endif //LAURENT_FINAL_PROJECT_MARKETSIMULATOR_H
