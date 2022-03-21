//
// Created by sebastiend on 23/10/2018.
//

#include "OrderBook.h"


bool OrderBook::is_empty() const{
    return bids.empty() and asks.empty();
}

void OrderBook::add_bid(int price, int amount){
    add(price,amount, true);
}
void OrderBook::add_ask(int price, int amount){
    add(price,amount, false);
};

void OrderBook::modify_bid(int price, int amount){
    add(price,-amount, true);
}
void OrderBook::modify_ask(int price, int amount){
    add(price,-amount, false);
};

void OrderBook::clear()
{
    asks.clear();
    bids.clear();
}

void OrderBook::add(int price, int amount, bool bid) {
    if(bid)
        bids[price]+=amount;
    else
        asks[price]+=amount;
}



void OrderBook::remove_bid(int price, int amount){
    remove(price,amount, true);
}
void OrderBook::remove_ask(int price, int amount){
    remove(price,amount, false);
};

void OrderBook::remove(int price, int amount, bool bid) {
    auto& table = bid? bids : asks;
    auto it = table.find(price);
    if (it!=table.end())
    {
        it->second -= amount;
        if (it->second == 0){
            table.erase(it);
        }
    }
}


std::ostream& operator<<(std::ostream &os, const OrderBook &book) {
    if (book.is_empty())
    {
        os << "ORDER BOOK EMPTY";
        return os;
    }

    os << "ASKS" << std::endl;
    for(auto it = book.asks.begin();
        it!=book.asks.end();
        ++it)
    {
        os << it->first << "\t" << it->second << std::endl;
    }
    os << "BIDS" << std::endl;
    for(auto it = book.bids.begin();
        it!=book.bids.end();
        ++it)
    {
        os << it->first << "\t" << it->second << std::endl;
    }
    return os;
}


void OrderBook::handle_price_update(const PriceUpdate &pu)
{
    switch(pu.get_action())
    {
        case action_t::NEW:
            if(pu.get_is_buy())
                add_bid(pu.get_price(),pu.get_quantity());
            else
                add_ask(pu.get_price(),pu.get_quantity());
            break;
        case action_t::MODIFY:
            if(pu.get_is_buy())
                add_bid(pu.get_price(),pu.get_quantity());
            else
                add_ask(pu.get_price(),pu.get_quantity());
            break;
        case action_t::REMOVE:
            if(pu.get_is_buy())
                remove_bid(pu.get_price(),pu.get_quantity());
            else
                remove_ask(pu.get_price(),pu.get_quantity());
            break;
    }

}


std::ostream& operator<<(std::ostream &os, const OrderBook::BidAsk &ba) {
    auto print = [&](const OrderBook::BidAsk::Entry &e, const std::string &text)
    {
        bool have_value = e.is_initialized();
        if(have_value)
        {
            auto value = e.get();
            os << value.second << text << "s @" << value.first;

        } else{
            os << "NO" << text;
        }
    };
    print(ba.bid, "bid");
    os << ", ";
    print(ba.ask, "ask");
    return os;
}

boost::optional<int> OrderBook::BidAsk::spread() const{
    boost::optional<int> result;
    if(bid.is_initialized() and ask.is_initialized())
        result=ask.get().first - bid.get().first;
    return result;
}




OrderBook::BidAsk OrderBook::get_bid_ask() const {
    BidAsk result;
    auto best_bid = bids.rbegin();
    if(best_bid != bids.rend() )
        result.bid = *best_bid;
    auto best_ask = asks.begin();
    if(best_ask != asks.end() )
        result.ask = *best_ask;
    return result;
}