#include <iostream>

#include "classes/my_unit_test.h"
#include "classes/Order.h"
#include "classes/AdmenmentUpdate.h"
#include "classes/MarketSimulator.h"
#include "classes/OrderManager.h"
#include "classes/TradingStrategy.h"
#include "classes/MDReader.h"
#include "classes/BookUpdate.h"
#include "classes/BookBuilder.h"
#include <fstream>

void reset_communication(
        std::queue<Order> &strategy_to_ordermanager,
        std::queue<ExecutionOrder> &ordermanager_to_strategy,
        std::queue<Order> &ordermanager_to_simulator,
        std::queue<ExecutionOrder> &simulator_to_ordermanager
        )
{
    while (!strategy_to_ordermanager.empty()) strategy_to_ordermanager.pop();
    while (!ordermanager_to_strategy.empty()) ordermanager_to_strategy.pop();
    while (!ordermanager_to_simulator.empty()) ordermanager_to_simulator.pop();
    while (!simulator_to_ordermanager.empty()) simulator_to_ordermanager.pop();
}

int main() {

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

    MDReader reader("C:\\Users\\laure\\OneDrive - The University of Chicago\\2021 - (5) Spring Semester\\FINM 32700 - Advance Computing for Finance\\final\\spy_ret.csv"
                    ,",",5);

    // Get the data from CSV File
    std::vector<BookUpdate> dataList = reader.getData();

    TradingStrategy ts(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager,
                        bookbuilder_to_strategy);

    MarketSimulator simulator(strategy_to_ordermanager,
                              ordermanager_to_strategy,
                              ordermanager_to_simulator,
                              simulator_to_ordermanager,
                              bookbuilder_to_strategy);

    OrderManager order_manager(strategy_to_ordermanager,
                               ordermanager_to_strategy,
                               ordermanager_to_simulator,
                               simulator_to_ordermanager,
                               bookbuilder_to_strategy);

    BookBuilder book_builder(strategy_to_ordermanager,
                             ordermanager_to_strategy,
                             ordermanager_to_simulator,
                             simulator_to_ordermanager,
                             bookbuilder_to_strategy);

    book_builder.start();
    simulator.start();
    order_manager.start();
    ts.start();

    while(!bookbuilder_to_strategy.empty())
    {
        ts.process_book_update();
        order_manager.handle_order();
        simulator.handle_order();
        // Simulator sends 2 orders one acknowledge, one filled so need keep OM and TS twice after
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts.process_market_response();
        ts.process_market_response();

    }

    // Helper outputs to see what orders were filled, when and what price, useful to compare to python

    // Comparing the three different output where one comes from the
    /*
     *  SIMULATOR
     *  TRADINGSTRATEGY
     *  PYTHON
     */
    // We get that for each the price and point of purchase/sell all match, I didn't know how I could incorporate it
    // in a google test where i'd compare every price but the csv files are in the project files under
    /*
     *  market_output.csv
     *  tradingstrategy_output.csv
     *  python_market_output.csv
     */

    // It is easy to see that they all match with each other.
    std::ofstream f;
    f.open("market_output.csv");
    for (auto &it: simulator.filled_orders){
        f << it;
    }
    f.close();

    std::ofstream f2;
    f2.open("tradingstrategy_output.csv");
    for (auto &it: ts.filled_orders){
        f2 << it;
    }
    f2.close();

    /*
     *
     *          For the Tests, check project_tests by running RUNTESTS | DEBUG (GOOGLE_TEST)
     *
     *          To find the implementation and usage of the Smart Pointer look into OrderManager.cpp

     *          I know I shouldn't have copied all the headers and .cpp file into tests directory
     *          but after spending 2h trying to debug it without success i felt better use of my time
     *          to just proceed on.
     *
     */
    return 0;
}
