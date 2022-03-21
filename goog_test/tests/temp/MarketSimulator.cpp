//
// Created by sebastiend on 06/10/2018.
//

#include "MarketSimulator.h"
#include <fstream>

MarketSimulator::MarketSimulator(
        std::queue<Order> &strategy_to_ordermanager_,
        std::queue<ExecutionOrder> &ordermanager_to_strategy_,
        std::queue<Order> &ordermanager_to_simulator_,
        std::queue<ExecutionOrder> &simulator_to_ordermanager_,
        std::queue<BookUpdate> &bookbuilder_to_strategy_
        ):AppBase(strategy_to_ordermanager_,
                ordermanager_to_strategy_,
                ordermanager_to_simulator_,
                 simulator_to_ordermanager_,
                 bookbuilder_to_strategy_),
                 execution_id(0),
                 pnl(0)
     {
        list_symbols = {"SPY"};
     }


bool MarketSimulator::handle_order(){
    if(!is_working)
        return false;
    if(ordermanager_to_simulator.empty())
        return true;
    const Order &o = ordermanager_to_simulator.front();
    ordermanager_to_simulator.pop();

    const bool is_tradeable = list_symbols.find(o.getSymbol()) != list_symbols.end();

    ExecutionOrder new_execution(o);
    if(is_tradeable){
        new_execution.setState(o.getQuantity()<=100?orderstate::ACKNOWLEDGED:orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);
        simulator_to_ordermanager.push(new_execution);
        new_execution.setState(orderstate::FILLED);
        std::string side_;
        if(new_execution.isBuy()){
            side_ = "BUY";
            count_buy++;
            pnl -= new_execution.getPrice() * new_execution.getQuantity();
        }
        else{
            count_sell++;
            side_ = "SELL";
            pnl += new_execution.getPrice() * new_execution.getQuantity();
        }
        double price_ = new_execution.getPrice();
        ++print_counter;
        if (print_counter % 10000 == 0) {
            std::cout << "simulator push a fill|" <<
                      new_execution.getTimeStamp() << "|" <<
                      price_ << "|" <<
                      side_ << "|" <<
                      new_execution.getQuantity() << "|" <<
                      std::endl;
        }
        std::string s = std::to_string(new_execution.getTimeStamp()) + "," +
        std::to_string(price_) + ","  + side_ + "\n";
        filled_orders.push_back(s);
        simulator_to_ordermanager.push(new_execution);
    }
    else{
        new_execution.setState(orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);
        simulator_to_ordermanager.push(new_execution);
    }
    return true;
}

double MarketSimulator::get_pnl() {
    return pnl;
};


void MarketSimulator::reset_position(){
    pnl = 0;
    count_sell = 0;
    count_buy = 0;
}