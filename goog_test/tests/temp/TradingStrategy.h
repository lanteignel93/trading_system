#ifndef LAURENT_FINAL_PROJECT_TRADINGSTRATEGY_H
#define LAURENT_FINAL_PROJECT_TRADINGSTRATEGY_H



#include "AppBase.h"
#include "BookUpdate.h"
#include "Order.h"
#include "AdmenmentUpdate.h"
#include <unordered_map>
#include <string>
#include <iostream>

class Signal{
private:
    std::vector<BookUpdate *> book;
    bool is_long = false;
public:
    Signal()= default;
    ~Signal()= default;
    void insert_book_update(BookUpdate bu);
    double get_10day_moving_average();
    double get_25day_moving_average();
    bool open_position();
    bool close_position();
    bool is_tradeable(BookUpdate &bu);
    std::vector<BookUpdate *> getBook(){return book;}
};

class Execution{
private:
    Order e;
    bool tradeable;
public:

    Execution():tradeable(false){}
    bool insert_order(long timestamp_,
                      bool is_buy_,
                      double price_,
                      unsigned int quantity_,
                      const char * venue_,
                      const char * symbol_,
                      ordertype type_,
                      unsigned int id_);
    bool is_tradeable() {return tradeable;}
    void set_tradeable(bool is_tradable){tradeable=is_tradable;};
    Order & get_order() {return e;}
};

class TradingStrategy : public AppBase {
private:
    Signal signal;
    Execution execution;
    int order_id;
    std::unordered_map<std::string,int> positions;
    unsigned int number_of_rejections = 0;
    unsigned int number_of_fills = 0;
    double pnl_out = 0;
    int count_sell_out = 0;
    int count_buy_out = 0;
    double pnl_in = 0;
    int count_sell_in = 0;
    int count_buy_in = 0;

public:
    std::vector<std::string> filled_orders;
    TradingStrategy(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_
    ):
    AppBase(strategy_to_ordermanager_,
              ordermanager_to_strategy_,
              ordermanager_to_simulator_,
              simulator_to_ordermanager_,
              bookbuilder_to_strategy_),
              signal(),
              execution(),
              order_id(1){}
    virtual void start() {is_working=true;}
    virtual void stop() {
        positions.clear();
        is_working=false;
    }

    bool process_admendent_update(AdmenmentUpdate &au);
    bool process_book_update(BookUpdate &bu);
    bool process_book_update();
    bool process_execution();
    bool process_market_response();
    int get_position(std::string symbol);
    unsigned int get_number_of_rejections();
    unsigned int get_number_of_fills();
    void reset_position();
    double get_pnl();
    int getCountBuy(){return count_buy_out;}
    int getCountSell(){return count_sell_out;}

};

#endif //LAURENT_FINAL_PROJECT_TRADINGSTRATEGY_H