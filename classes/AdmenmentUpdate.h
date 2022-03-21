#ifndef LAURENT_FINAL_PROJECT_ADMENMENTUPDATE_H
#define LAURENT_FINAL_PROJECT_ADMENMENTUPDATE_H

#include "BookUpdate.h"

class AdmenmentUpdate {
private:
    level_t level;
    quantity_t quantity;
    char venue[20];
    char symbol[20];
    bool is_buy;
    unsigned int id;
public:
    AdmenmentUpdate(level_t level_,
                    quantity_t quantity_,
                    const char *venue_,
                    bool is_buy_,
                    const char *symbol_,
                    unsigned int id_):
            level(level_),
            quantity(quantity_),
            is_buy(is_buy_),
            id(id_)
    {
        strcpy(venue,venue_);
        strcpy(symbol,symbol_);
    }
    quantity_t get_quantity(){return quantity;}
    level_t get_level(){return level;}
    const char * get_venue(){return venue;}
    bool get_is_buy(){return is_buy;}
    const char * get_symbol(){return symbol;}
    unsigned int getId(){return id;}
};


#endif //LAURENT_FINAL_PROJECT_ADMENMENTUPDATE_H