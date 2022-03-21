//
// Created by sebastiend on 24/10/2018.
//

#ifndef ORDERBOOKDEMO_PRICEUPDATE_H
#define ORDERBOOKDEMO_PRICEUPDATE_H


#include <string.h>

typedef unsigned int price_t;
typedef unsigned int quantity_t;
typedef unsigned int order_id_t;
typedef enum  {NEW,MODIFY,REMOVE} action_t;


class PriceUpdate {
private:
    order_id_t order_id;
    price_t price;
    quantity_t quantity;
    char venue[20];
    char symbol[20];
    bool is_buy;
    unsigned int epoch_time;
    action_t action;

public:
    PriceUpdate(action_t action_,
               price_t price_,
               quantity_t quantity_,
               const char *venue_,
               bool is_buy_,
               const char *symbol_,
               unsigned int epoch_time_,
               order_id_t order_id_):
            action(action_),
            price(price_),
            quantity(quantity_),
            is_buy(is_buy_),
            epoch_time(epoch_time_),
            order_id(order_id_)
    {
        strcpy(venue,venue_);
        strcpy(symbol,symbol_);
    }
    price_t get_price() const {return price;}
    quantity_t get_quantity() const {return quantity;}
    order_id_t get_order_id() const {return order_id;}
    const char * get_venue() const {return venue;}
    bool get_is_buy() const {return is_buy;}
    const char * get_symbol() const {return symbol;}
    const action_t get_action() const {return action;}
    const unsigned int get_epoch() const {return epoch_time;}
    void set_quantity(quantity_t q_){quantity = q_;}
};



#endif //ORDERBOOKDEMO_PRICEUPDATE_H
