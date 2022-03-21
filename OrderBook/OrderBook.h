//
// Created by sebastiend on 23/10/2018.
//

#ifndef IEOR_HOMEWORK8_ORDERBOOK_H
#define IEOR_HOMEWORK8_ORDERBOOK_H
#include <map>
#include <iostream>
#include <boost/optional.hpp>
#include <PriceUpdate.h>

class OrderBook {
    std::map<int, int> bids, asks;
    void add(int price, int amount, bool bid);
    void remove(int price, int amount, bool bid);
public:
    struct BidAsk{
        typedef boost::optional<std::pair<int, int>> Entry;
        Entry bid, ask;
        boost::optional<int> spread() const;
    };
    bool is_empty() const;
    void add_bid(int price, int amount);
    void add_ask(int price, int amount);
    void remove_bid(int price, int amount);
    void remove_ask(int price, int amount);
    void clear();
    void modify_bid(int price, int amount);
    void modify_ask(int price, int amount);
    void handle_price_update(const PriceUpdate &pu);



    BidAsk get_bid_ask() const;
    friend std::ostream & operator << (std::ostream &os, const OrderBook &book);
    friend std::ostream & operator << (std::ostream &os, const OrderBook::BidAsk &ba);

};


#endif //IEOR_HOMEWORK8_ORDERBOOK_H
