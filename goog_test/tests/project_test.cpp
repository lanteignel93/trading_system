#include <gtest/gtest.h>
#include "temp/my_unit_test.h"
#include "temp/Order.h"
#include "temp/AdmenmentUpdate.h"
#include "temp/MarketSimulator.h"
#include "temp/OrderManager.h"
#include "temp/TradingStrategy.h"
#include "temp/MDReader.h"
#include "temp/BookUpdate.h"
#include "temp/BookBuilder.h"

TEST(goog_test, reader){


    MDReader reader("C:\\Users\\laure\\OneDrive - The University of Chicago\\2021 - (5) Spring Semester\\FINM 32700 - Advance Computing for Finance\\final\\spy_ret.csv"
            ,",",5);

    // Get the data from CSV File
    std::vector<BookUpdate> dataList = reader.getData();

    BookUpdate bu = dataList.front();
    // Have to check differently since we are using doubles instead of ints for price
    EXPECT_TRUE(abs(bu.get_price()-154.864) < 1e-3);
    EXPECT_EQ(bu.get_epoch(),1);
    EXPECT_EQ(bu.get_quantity(), 100);
}


TEST(goog_test, moving_averages1)
{
    /*
     *      Testing Moving Averages after seeing 25 Book Updates
     */
    Signal sig1;

    for(int k=0;k<25;k++){
        sig1.insert_book_update(BookUpdate(0,k,k*100,"LL",true,"CPP",k));
    }
    // Last 10 values will be 15 to 24 so average is 19.5
    EXPECT_EQ(sig1.get_10day_moving_average(), 19.5);
    // We should expect 12 by symmetry
    EXPECT_EQ(sig1.get_25day_moving_average(),12);

}

TEST(goog_test, moving_averages2)
{
    /*
     *      Testing Moving Averages after seeing 0 and 1 Book Update
     */
    Signal sig1;

    EXPECT_EQ(sig1.get_10day_moving_average(), 0);
    EXPECT_EQ(sig1.get_25day_moving_average(),0);

    sig1.insert_book_update(BookUpdate(0,1,1*100,"LL",true,"CPP",1));
    EXPECT_EQ(sig1.get_10day_moving_average(), 1);
    EXPECT_EQ(sig1.get_25day_moving_average(),1);

}

TEST(goog_test, signal_open)
{
    /*
     *      Testing if open and close signal properly works
     *      In this case if 10MA > 25MA and we are not long then we need to switch long
     */
    Signal sig1;

    for(int k=0;k<25;k++){
        sig1.insert_book_update(BookUpdate(0,k,k*100,"LL",true,"CPP",k));
    }
    EXPECT_TRUE(sig1.open_position());
    EXPECT_FALSE(sig1.close_position());

    // Now since we are long if we try and check if we can open a position we should get false because
    // in our strategy we cannot double down when we are long we need to exit position first
    EXPECT_FALSE(sig1.open_position());

    // the first time it is true, but now that we are long the second time should return false until we sell
}

TEST(goog_test, signal_close1)
{
    /*
     *      Testing if close signal properly works
     *      In this case if 10MA < 25MA and we are not long then we cannot close position
     */
    Signal sig1;

    for(int k=25;k>0;k--){
        sig1.insert_book_update(BookUpdate(0,k,k*100,"LL",true,"CPP",k));
    }
    EXPECT_FALSE(sig1.open_position());
    EXPECT_FALSE(sig1.close_position());

}

TEST(goog_test, signal_close2)
{
    /*
     *      Testing if close signal properly works
     *      In this case if 10MA < 25MA and we are not long then we cannot close position
     */
    Signal sig1;

    for(int k=0;k<25;k++){
        sig1.insert_book_update(BookUpdate(0,k,k*100,"LL",true,"CPP",k));
    }
    EXPECT_TRUE(sig1.open_position());
    // Our position should be long so now when 10MA < 25MA we should sell
    for(int k=25;k>0;k--){
        sig1.insert_book_update(BookUpdate(0,k,k*100,"LL",true,"CPP",k+25));
    }
    EXPECT_TRUE(sig1.close_position());

}

TEST(goog_test, pnl)
{

    /*
     *
     *  Test the PnL of the strategy, to be compared with Python file for output value
     *
     */
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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
    // TEST IF PNL MATCH FROM THE TRADINGSTRATEGY POINT OF VIEW
    EXPECT_TRUE(abs(ts.get_pnl()-11624.14)<1e-2);

    // TEST IF PNL MATCH FROM THE SIMULATOR POINT OF VIEW
    EXPECT_TRUE(abs(simulator.get_pnl()-11624.14)<1e-2);

    // since pnl match we should observe the amount of buy and sell orders to match
    // from python code we know it should be 36 buy and 36 sells
    EXPECT_EQ(ts.getCountBuy(), 36);
    EXPECT_EQ(ts.getCountSell(), 36);
    EXPECT_EQ(ts.getCountSell(), simulator.getCountSell());
    EXPECT_EQ(ts.getCountBuy(), simulator.getCountBuy());

}

TEST(goog_test, TSProcessBookUpdate){

    /*
     *
     *  TESTING process_book_update in TradingStrategy
     */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k =0; k<10; k++){
        BookUpdate bu(0,k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }
    // We have processed 10 orders so the 10MA = 25MA so there should be no orders in the strategy to order queue

    // We are going to process 10 more orders so now 10MA > 25MA but it happened at the first process book update
    // So we should expect the price of the order in the strategy_to_ordermanager to be 11 and not higher
    EXPECT_TRUE(strategy_to_ordermanager.empty());
    for(int k =0; k<10; k++){
        BookUpdate bu(0,11+k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }
    const Order&e = strategy_to_ordermanager.front();
    EXPECT_EQ(e.getPrice(), 11);

    // Let's make sure there is nothing else in the queue
    strategy_to_ordermanager.pop();
    EXPECT_TRUE(strategy_to_ordermanager.empty());

    // We indeed stop pushing orders to the ordermanager once we sent the order to go long our signal after
    // prevents us from making more sending more orders until we exit our current position
}


TEST(goog_test, OMHandleOrder1){

    /*
     *
     *  TESTING  handle_order in OrderManager
     */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k =0; k<11; k++){
        BookUpdate bu(0,k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }
    //
    EXPECT_TRUE(order_manager.handle_order());
    // Checking if the right order was sent to simulator
    const Order&e = ordermanager_to_simulator.front();
    EXPECT_EQ(e.getPrice(), 10);
    EXPECT_EQ(e.getTimeStamp(), 10);
}

TEST(goog_test, SimulatorHandleOrder1){

    /*
     *
     *  TESTING handle_order in MarketSimulator
     */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k =0; k<11; k++){
        BookUpdate bu(0,k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }
    //
    order_manager.handle_order();
    EXPECT_FALSE(ordermanager_to_simulator.empty());
    EXPECT_TRUE(simulator_to_ordermanager.empty());
    simulator.handle_order();
    EXPECT_FALSE(simulator_to_ordermanager.empty());
    const ExecutionOrder exec_o = simulator_to_ordermanager.front();
    EXPECT_EQ(exec_o.getState(), orderstate::ACKNOWLEDGED);
    EXPECT_EQ(exec_o.getPrice(), 10);


    // Simulator also sends a second order with a different state, let's check
    simulator_to_ordermanager.pop();
    const ExecutionOrder exec_o2 = simulator_to_ordermanager.front();
    EXPECT_EQ(exec_o2.getState(), orderstate::FILLED);
    EXPECT_EQ(exec_o2.getPrice(), 10);
}

TEST(goog_test, SimulatorHandleOrder2){

    /*
     *
     *  TESTING handle_order in MarketSimulator
     *  I have condition that the quantity must be less or equal to 100
     *  This is because the strategy involves buying SPY in chunks of 100
     *  and we don't want to allow for a bigger quantity that could blowout the account
     *
     */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    // Same as last code but quantity > 100 so we should get reject
    for(int k =0; k<11; k++){
        BookUpdate bu(0,k,1000,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }
    //
    order_manager.handle_order();
    EXPECT_FALSE(ordermanager_to_simulator.empty());
    EXPECT_TRUE(simulator_to_ordermanager.empty());
    simulator.handle_order();
    EXPECT_FALSE(simulator_to_ordermanager.empty());
    const ExecutionOrder exec_o = simulator_to_ordermanager.front();
    EXPECT_EQ(exec_o.getState(), orderstate::REJECTED);
    EXPECT_EQ(exec_o.getPrice(), 10);

}

TEST(goog_test, SimulatorHandleOrder3){

    /*
     *
     *  TESTING handle_order in MarketSimulator
     *  We are only allowed to trade SPY let's see what happens if something else comes in
     */

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k =0; k<11; k++){
        BookUpdate bu(0,k,100,"LL",true,"CPP",k);
        ts.process_book_update(bu);
    }
    //
    order_manager.handle_order();
    simulator.handle_order();
    const ExecutionOrder exec_o = simulator_to_ordermanager.front();
    EXPECT_EQ(exec_o.getState(), orderstate::REJECTED);

}

TEST(goog_test, TSnumber_rejection){

    /*
     *
     *  In this unit test we will check if OrderManager sends a rejected order
     *  Given quantity will be < 100 but the total position would be over 1e6 we will
     *  reject the order
     *
     */
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k =0; k<11; k++){
        BookUpdate bu(0,k*1e4,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }
    // 10 MA > 25MA we should have sent an order but position size will be bigger than 1e6 so no order to simulator
    order_manager.handle_order();
    EXPECT_TRUE(ordermanager_to_simulator.empty());

    //  We should have sent a reject order to the TS let's check if it's there
    EXPECT_FALSE(ordermanager_to_strategy.empty());

    ts.process_market_response();
    EXPECT_EQ(ts.get_number_of_rejections(), 1);
}

TEST(goog_test, TSnumber_of_fills){


    /*
     *
     *          Testing the number of fills in the Trading Strategy
     */
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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


    for(int k =0; k<25; k++){
        BookUpdate bu(0,k*k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts.process_market_response();
        ts.process_market_response();
    }
    // We expect to have one filled orders given 10MA > 25 MA
    EXPECT_EQ(ts.get_number_of_fills(),1);

    // At some point the 25MA > 10MA
    for(int k =25; k>0; k--){
        BookUpdate bu(0,k*k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts.process_market_response();
        ts.process_market_response();
    }
    // we should have sold our position at some point so number of filled orders should be 2
    EXPECT_EQ(ts.get_number_of_fills(),2);
}

TEST(goog_test, TSandOM_get_Position){

    /*
     *
     *          Testing the get position from TS and OM
     */
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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


    for(int k =0; k<25; k++){
        BookUpdate bu(0,k*k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts.process_market_response();
        ts.process_market_response();
    }
    // 10th element so price should be 100, quantity 100 so position should be 10000
    EXPECT_EQ(ts.get_position("SPY"), 10000);
    EXPECT_EQ(order_manager.get_position("SPY"), 10000);

    // At some point the 25MA > 10MA so we should have sell at the 11th iteration so k = 14
    // 14**2 = 196 so our position should be 10000 - 19600 = - 9600

    for(int k =25; k>0; k--){
        BookUpdate bu(0,k*k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts.process_market_response();
        ts.process_market_response();
    }

    EXPECT_EQ(ts.get_position("SPY"), -9600);
    EXPECT_EQ(order_manager.get_position("SPY"), -9600);
}

TEST(goog_test, OMget_number_open_orders)
{
    /*
     *
     *  Because our strategy doesn't permit multiple positions we need to manually add orders to queue
     *  to check function
     */
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k=0;k<10;k++) {
        Order o(0, true, k, k, 100, "LL", "SPY");
        ExecutionOrder e(o);
        strategy_to_ordermanager.push(e);
        order_manager.handle_order();
    }
    EXPECT_EQ(order_manager.get_number_of_non_acknowledged_orders(), 10);


    simulator.handle_order();
    order_manager.handle_execution_order();
    ts.process_market_response();

    EXPECT_EQ(order_manager.get_number_of_non_acknowledged_orders(), 9);
    EXPECT_EQ(order_manager.get_number_of_open_orders(), 10);
}

TEST(goog_test, AdmenmentUpdate)
{
    /*
     *
     *      Last test will check if admenment is properly done
     *
     */
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;

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

    for(int k =0; k<11; k++){
        BookUpdate bu(0,k,100,"LL",true,"SPY",k);
        ts.process_book_update(bu);
    }

    const Order custom_unit_order =strategy_to_ordermanager.front();

    EXPECT_TRUE(order_manager.handle_order());
    EXPECT_TRUE(simulator.handle_order());

    const ExecutionOrder custom_unit_exec =simulator_to_ordermanager.front();
    // the simulator will send an acknowledge order first
    EXPECT_EQ(custom_unit_exec.getState(), orderstate::ACKNOWLEDGED);
    EXPECT_TRUE(order_manager.handle_execution_order());


    AdmenmentUpdate au1(0,50,"LL",true,"SPY", custom_unit_order.getID());

    EXPECT_TRUE(ts.process_admendent_update(au1));
    // We update the order before the order manager executes the 2nd order from market simulator which is filled
    EXPECT_TRUE(order_manager.handle_order());

    order_manager.handle_execution_order();
    order_manager.handle_execution_order();

    ts.process_market_response();
    ts.process_market_response();

    // Check if successfully amendment
    EXPECT_EQ(order_manager.get_position("SPY"), 500);
    EXPECT_EQ(ts.get_position("SPY"), 500);

    // We succesfully amendment the quantity before execution!
}
