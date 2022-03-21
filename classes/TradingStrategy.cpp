#include "TradingStrategy.h"

void Signal:: insert_book_update(BookUpdate bu){
    book.push_back(new BookUpdate(bu));
};
double Signal::get_10day_moving_average(){

    double sum = 0;
    unsigned int count = 0;
    if(book.empty()){return 0;}
    for(int i = book.size() - 1; i >= 0; i--){
        sum += book[i]->get_price();
        ++count;
        double val = (double) sum/ (double)count;
        if(count == 10) return val;
        if(i == 0) return val;
    }
}
double Signal::get_25day_moving_average(){

    double sum = 0;
    unsigned int count = 0;
    if(book.empty()){return 0;}
    for(int i = book.size() - 1; i >= 0; i--){
        sum += book[i]->get_price();
        ++count;
        double val = (double) sum/ (double)count;
        if(count == 25) return val;
        if(i == 0) return val;
    }
}
bool Signal::open_position(){
    // Check if we are not long and 10MA > 25MA then open position
    if((!is_long) && (get_10day_moving_average() > get_25day_moving_average())) {
        is_long = true;
        return true;
    }
    else
        return false;
}
bool Signal::close_position() {
    // If long and 10MA < 25MA then close position
    if((is_long) && (get_10day_moving_average() < get_25day_moving_average())) {
        is_long = false;
        return true;
    }
    else
        return false;
}
bool Signal::is_tradeable(BookUpdate &bu){
    return true;
}

bool Execution::insert_order(long timestamp_,
                  bool is_buy_,
                  double price_,
                  unsigned int quantity_,
                  const char * venue_,
                  const char * symbol_,
                  ordertype type_,
                  unsigned int id_)
{
    e.setTime(timestamp_);
    e.setSide(is_buy_);
    e.setPrice(price_);
    e.setQuantity(quantity_);
    e.setVenue(venue_);
    e.setType(type_);
    e.setSymbol(symbol_);
    e.setOrderID(id_);

    return true;
}

bool TradingStrategy::process_book_update(BookUpdate &bu){
    if(!is_working)
        return false;
    if (signal.is_tradeable(bu)) {
        signal.insert_book_update(bu);
        bool side_;
        if(signal.open_position()){
            side_ = true;
            count_buy_out++;
            pnl_out -= bu.get_price() * bu.get_quantity();
        }
        else if (signal.close_position()){
            side_ = false;
            count_sell_out++;
            pnl_out += bu.get_price() * bu.get_quantity();
        }
        else {
            return false;
        }
        execution.insert_order(bu.get_epoch(),side_,
                               bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
        execution.set_tradeable(true);
    }
    return process_execution();
}


bool TradingStrategy::process_book_update(){
    if(!is_working)
        return false;
    if(bookbuilder_to_strategy.empty())
        return true;
    BookUpdate &bu = bookbuilder_to_strategy.front();
    bookbuilder_to_strategy.pop();
    return process_book_update(bu);
}

bool TradingStrategy::process_execution() {
    if(!is_working)
        return false;
    Order order;
    if (execution.is_tradeable()) {
        order.setTime(execution.get_order().getTime());
        order.setType(execution.get_order().getOrderType());
        order.setVenue(execution.get_order().getVenue());
        order.setQuantity(execution.get_order().getQuantity());
        order.setPrice(execution.get_order().getPrice());
        order.setOrderID(execution.get_order().getID());
        order.setSymbol(execution.get_order().getSymbol());
        order.setSide(execution.get_order().getSide());
        execution.set_tradeable(false);
        strategy_to_ordermanager.push(order);
    }
    return true;
};

bool TradingStrategy::process_market_response()
{
    if(ordermanager_to_strategy.empty()) {
        return true;
    }
    ExecutionOrder &order = ordermanager_to_strategy.front();
    double p = order.getPrice();
    double q = order.getQuantity();
    const char * s = order.getSymbol();
    std::string s_ = std::string(s);

    ordermanager_to_strategy.pop();
    if(order.getState() == orderstate:: ACKNOWLEDGED) return true;
    if(order.getState() == orderstate::FILLED) {
        std::string side_;
        if (order.isBuy()) {
            side_ = "BUY";
            count_buy_in++;
            pnl_in -= p*q;
            positions[s_] += p * q;
        } else {
            side_ = "SELL";
            count_sell_in++;
            pnl_in += p*q;
            positions[s_] -= p * q;
        }
        std::string temp_s = std::to_string(order.getTimeStamp()) + "," +
                        std::to_string(p) + ","  + side_ + "\n";
        filled_orders.push_back(temp_s);
        number_of_fills += 1;
    }

    else if(order.getState() == orderstate::REJECTED){
        number_of_rejections +=1;
    }
    return true;
}

bool TradingStrategy::process_admendent_update(AdmenmentUpdate &au){
    if(!is_working)
        return false;
    execution.insert_order(0,au.get_is_buy(),
                           0,au.get_quantity(),au.get_venue(),au.get_symbol(),ordertype::ADMENDMENT,au.getId());
    execution.set_tradeable(true);
    return process_execution();
}

int TradingStrategy::get_position(std::string symbol)
{
    return positions[symbol];
}

unsigned int TradingStrategy::get_number_of_rejections() {
    return number_of_rejections;
}


unsigned int TradingStrategy::get_number_of_fills() {
    return number_of_fills;
}

double TradingStrategy::get_pnl() {
    return pnl_out;
}

void TradingStrategy::reset_position(){
    positions.clear();
    pnl_out = 0;
    count_sell_out = 0;
    count_buy_out = 0;
    pnl_in = 0;
    count_sell_in = 0;
    count_buy_in = 0;
}
