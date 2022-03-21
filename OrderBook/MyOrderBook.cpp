//
// Created by sebastiend on 24/10/2018.
//

#include "MyOrderBook.h"


bool MyOrderBook::is_empty() const{
    return bids.empty() and asks.empty();
}

void MyOrderBook::add_bid(const PriceUpdate &pu){
    // Add a new bid who's price bigger than first element order book bid side so make book update
    if((pu.get_price() > bids.begin()->first) || (bids.empty())){
        BookUpdate bu = make_book_update(pu);
        bu_bid = bu;
        ++book_update_count;
    }
    MapIter itr = bids.insert(std::pair<price_t, PriceUpdate> (pu.get_price(), pu));
    bids_hash[pu.get_order_id()] = itr;

}
void MyOrderBook::add_ask(const PriceUpdate &pu){
    // Add a new ask who's price less than first element order book bid side so make book update
    if((pu.get_price() < asks.begin()->first) || (asks.empty())){
        BookUpdate bu =make_book_update(pu);
        bu_ask = bu;
        ++book_update_count;
    }
    MapIter itr = asks.insert(std::pair<price_t, PriceUpdate> (pu.get_price(), pu));
    asks_hash[pu.get_order_id()] = itr;
};


void MyOrderBook::remove_bid(const PriceUpdate &pu){
    order_id_t id_ = pu.get_order_id();
    if(bids_hash.find(id_)!=bids_hash.end()){
        MapIter itr = bids_hash[id_];
        // If quantity of order is less or equal than removal quantity we erase order
        if(itr->second.get_quantity() <= pu.get_quantity()){
            bids_hash.erase(id_);
            // if the order we erase on top of the book
            if(pu.get_price()== bids.begin()->first){
                bids.erase(itr);
                //Need to check if asks empty or not
                if(bids.empty()){
                    // If empty, update bookupdate with default constructor
                    BookUpdate bu;
                    bu_bid = bu;
                }
                // If the price of next element is not the same than there is a book update
                else if(pu.get_price() != bids.begin()->first){
                    BookUpdate bu = make_book_update(bids.begin()->second);
                    bu_bid = bu;
                    ++book_update_count;
                }
            }
            // If order we erase is not on top simply erase it
            else
                bids.erase(itr);
        }
            // If quantity of order is more than quantity we remove, simply decrease the quantity
        else {
            quantity_t curr_q = itr->second.get_quantity();
            quantity_t remove_q = pu.get_quantity();
            itr->second.set_quantity(curr_q - remove_q);
            //Need to check if order was on top of book if so need to make book update
            if(itr->second.get_order_id() == id_){
                BookUpdate bu = make_book_update(bids.begin()->second);
                bu_bid = bu;
                ++book_update_count;
            }
        }
    }
};
void MyOrderBook::remove_ask(const PriceUpdate &pu){
    order_id_t id_ = pu.get_order_id();
    if(asks_hash.find(id_)!=asks_hash.end()){
        MapIter itr = asks_hash[id_];
        // If quantity of order is less or equal than removal quantity we erase order
        if(itr->second.get_quantity() <= pu.get_quantity()){
            asks_hash.erase(id_);
            // if the order we erase on top of the book
            if(pu.get_price()== asks.begin()->first){
                asks.erase(itr);
                //Need to check if asks empty or not
                if(asks.empty()){
                    // If empty, update bookupdate with default constructor
                    BookUpdate bu;
                    bu_ask = bu;
                }
                // If the price of next element is not the same than there is a book update
                else if(pu.get_price() != asks.begin()->first){
                    BookUpdate bu = make_book_update(asks.begin()->second);
                    bu_ask = bu;
                    ++book_update_count;
                }
            }
            // If order we erase is not on top simply erase it
            else
                asks.erase(itr);
        }
        // If quantity of order is more than quantity we remove, simply decrease the quantity
        else {
            quantity_t curr_q = itr->second.get_quantity();
            quantity_t remove_q = pu.get_quantity();
            itr->second.set_quantity(curr_q - remove_q);
            //Need to check if order was on top of book if so need to make book update
            if(itr->second.get_order_id() == id_){
                BookUpdate bu = make_book_update(asks.begin()->second);
                bu_ask = bu;
                ++book_update_count;
            }
        }
    }
};

void MyOrderBook::clear()
{
    bids.clear();
    asks.clear();
}

void MyOrderBook::handle_price_update(const PriceUpdate &pu){
    switch(pu.get_action())
    {
        case action_t::NEW:
            if(pu.get_is_buy())
                add_bid(pu);
            else
                add_ask(pu);
            break;
        case action_t::MODIFY:
            if(pu.get_is_buy())
                add_bid(pu);
            else
                add_ask(pu);
            break;
        case action_t::REMOVE:
            if(pu.get_is_buy())
                remove_bid(pu);
            else
                remove_ask(pu);
            break;
    }

}



std::ostream& operator<<(std::ostream &os, const MyOrderBook &book) {
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
        os << it->first << "\t" << it->second.get_quantity() << std::endl;
    }
    os << "BIDS" << std::endl;
    for(auto it = book.bids.begin();
        it!=book.bids.end();
        ++it)
    {
        os << it->first << "\t" << it->second.get_quantity() << std::endl;
    }
}

boost::optional<price_t> MyOrderBook::BidAsk::spread() const{
    boost::optional<price_t> result;
    if(bid.is_initialized() and ask.is_initialized()) {
        result = (ask.get().get_price() - bid.get().get_price());
    }
    else
        result = boost::none;
    return result;
}




MyOrderBook::BidAsk MyOrderBook::get_bid_ask() const {
    BidAsk result;
    auto best_bid = bu_bid;
    if (!bids.empty()) {
        result.bid = best_bid;
    }
    auto best_ask = bu_ask;
    if (!asks.empty()) {
        result.ask = best_ask;
    }
    return result;
}
BookUpdate MyOrderBook::make_book_update(const PriceUpdate &pu) {
    BookUpdate b(0,
                  pu.get_price(),
                  pu.get_quantity(),
                  pu.get_venue(),
                  pu.get_is_buy(),
                  pu.get_symbol(),
                  pu.get_epoch());
    return b;
}







