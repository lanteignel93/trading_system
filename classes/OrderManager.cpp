#include "OrderManager.h"


bool OrderManager::handle_execution_order(){
    if(!is_working)
        return false;
    if (simulator_to_ordermanager.empty())
        return true;

    const ExecutionOrder &e = simulator_to_ordermanager.front();
    simulator_to_ordermanager.pop();
    ordermanager_to_strategy.push(e);

   if ((e.getState() == orderstate::FILLED) || (e.getState() == orderstate::REJECTED)){
       list_orders.erase(e.getID());
       if ((e.getState() == orderstate::FILLED) ){
           int sign = 1;
           if (!e.isBuy()) sign = -1;
           positions[e.getSymbol()] += e.getPrice() * e.getQuantity() * sign;
       }

   }
   else if (e.getState() == orderstate::ACKNOWLEDGED){
       list_orders[e.getID()].setState(orderstate::ACKNOWLEDGED);
   }

    return true;
};


unsigned int OrderManager::get_number_of_open_orders()
{
    int res = 0;
    for (auto &it: list_orders){
        if((it.second.getState() == orderstate::OPEN) || (it.second.getState() == orderstate::ACKNOWLEDGED)){
            res +=1;
        }
    }
    return res;
}


unsigned int OrderManager::get_number_of_non_acknowledged_orders()
{
    int res = 0;
    for (auto &it: list_orders){
        if(it.second.getState() == orderstate::OPEN){
            res +=1;
        }
    }
    return res;
}


int OrderManager::get_position(std::string symbol)
{
    return positions[symbol];
}

bool OrderManager::handle_order(){
    if(!is_working)
        return false;
    if (strategy_to_ordermanager.empty())
        return true;
    const Order &e = strategy_to_ordermanager.front();

    if (e.getOrderType() == ordertype::ADMENDMENT){
        if(list_orders.find(e.getID()) == list_orders.end()){
            return false;
        }
        else{
            if(list_orders[e.getID()].getState() == orderstate::ACKNOWLEDGED){
                if(list_orders[e.getID()].getQuantity() > e.getQuantity()){
                    list_orders[e.getID()].setQuantity(e.getQuantity());

                    simulator_to_ordermanager.pop();
                    list_orders[e.getID()].setState(orderstate::FILLED);
                    simulator_to_ordermanager.push(list_orders[e.getID()]);
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
    }

    orderstate state_ = orderstate::OPEN;
    int sign = 1;
    if (!e.isBuy()) sign = -1;
    if (abs(sign * e.getQuantity() * e.getPrice() + positions[e.getSymbol()]) > 1e6){
        state_ = orderstate::REJECTED;
    }
    if (get_number_of_non_acknowledged_orders() > 10){
        state_ = orderstate::REJECTED;
    }
    SmartPointer<ExecutionOrder> exec_ptr = new ExecutionOrder(e);
    strategy_to_ordermanager.pop();
    exec_ptr->setState(state_);
    if (exec_ptr->getState() == orderstate::REJECTED){
        ordermanager_to_strategy.push(*exec_ptr);
        return true;
    }
    else {
        list_orders[e.getID()] = *exec_ptr;
        order=e;
        ordermanager_to_simulator.push(order);
        return true;
    }
};
