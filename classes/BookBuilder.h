#ifndef LAURENT_FINAL_PROJECT_BOOKBUILDER_H
#define LAURENT_FINAL_PROJECT_BOOKBUILDER_H
#include "AppBase.h"
#include "MDReader.h"

class BookBuilder: public AppBase {
public:
    BookBuilder(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_
    );
    virtual void start(){

        MDReader reader("C:\\Users\\laure\\OneDrive - The University of Chicago\\2021 - (5) Spring Semester\\FINM 32700 - Advance Computing for Finance\\final\\spy_ret.csv",",",0); //MODIFY THIS LINE AND THE LINE 39 in main.cpp

        // Get the data from CSV File
        std::vector<BookUpdate> dataList = reader.getData();

        // Send the book update to the trading strategy
        for(const BookUpdate &bu : dataList)
        {
            bookbuilder_to_strategy.push(bu);
        }
    };
    virtual void stop(){

    };

};


#endif //LAURENT_FINAL_PROJECT_BOOKBUILDER_H
