//
// Created by sebastiend on 24/10/2018.
//

#ifndef MyOrderBookDEMO_MYMyOrderBook_H
#define MyOrderBookDEMO_MYMyOrderBook_H


#include <map>
#include <iostream>
#include <boost/optional.hpp>
#include <PriceUpdate.h>
#include <BookUpdate.h>
#include <unordered_map>


typedef std::multimap<price_t,PriceUpdate>::iterator MapIter;
typedef std::multimap<price_t,PriceUpdate>::reverse_iterator MapReverseIter;
typedef std::unordered_map<order_id_t ,MapIter>::iterator HashmapIter;

// Multimaps: price -> order
typedef std::multimap<price_t,PriceUpdate,std::greater<price_t> > BidsMap;
typedef std::multimap< price_t,PriceUpdate,std::less<price_t> > AsksMap;

// Hashmaps: order id -> multimap iterator (for quick lookup by order ID)
typedef std::unordered_map<order_id_t,MapIter> BidsHash;
typedef std::unordered_map< order_id_t,MapIter> AsksHash;



class MyOrderBook {
    /* HOMEWORK THIS IS THE DATA STRUCTURE YOU NEED TO CHANGE */
    BidsHash bids_hash;
    AsksHash asks_hash;
    BidsMap bids;
    AsksMap asks;
    unsigned int book_update_count = 0;
    BookUpdate bu_bid;
    BookUpdate bu_ask;

public:
    /* You will need to replace BidAsk by BookUpdate */
    struct BidAsk{
        typedef boost::optional<BookUpdate> Entry;
        Entry bid, ask;
        boost::optional<price_t> spread() const;
    };
    bool is_empty() const;
    void add_bid(const PriceUpdate &pu);
    void add_ask(const PriceUpdate &pu);
    void remove_bid(const PriceUpdate &pu);
    void remove_ask(const PriceUpdate &pu);
    void handle_price_update(const PriceUpdate &pu);
    void clear();
    BidAsk get_bid_ask() const;
    BookUpdate make_book_update(const PriceUpdate &pu);

    friend std::ostream & operator << (std::ostream &os, const MyOrderBook &book);

    // helper functions if needed for unit tests.

    BidsHash get_bids_hash(){return bids_hash;}
    AsksHash get_asks_hash(){return asks_hash;}
    BidsMap get_bids(){return bids;}
    AsksMap get_asks(){return asks;}
    unsigned int get_count() {return book_update_count; }
};



#endif //MyOrderBookDEMO_MYMyOrderBook_H
