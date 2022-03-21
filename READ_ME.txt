University of Chicago - FINM32700: Advance Computing for Finance
Final Project - Back Tester
Date: May 31st 2021
Author: Laurent Lanteigne / 12279717



To whomever is going to read this, hi! I just wanted to give context to the folder for the final project.


1. 	""python_model.py"" contains the model in python which consists of a simple momentum strategy, the code is fairly simple the graphical
	output is stored in ""portfolio_value.png"" where you can see the portfolio value over time. 

	The code uses QUANDL and my API KEY so the code will likely not run locally, unless you put your own API KEY. 
	The output of the code is: 	Strategy Terminal PnL: 11624.14$

	There is also another ouput file ""python_market_output.csv"" this file is used to compare the trade orders via Python to C++.


2.	There are duplicates of the classes in ""goog_test/tests/temp"" where I put them there to make the file associations work. I spent a couple hours with CMake to try to make 
	it work more cleanly but hellas and I felt like a better use of my time to proceed forward. While the files are there, it's actually by modifying the ones in the main directory that 
	affects the unittests. 

3.	There are 2 more csv files ""market_output.csv"" and ""tradingstrategy_output.csv"", these are there to compare with the python csv file in order to see which orders are being filled.
	I made two to see if the orders being processed in the trading strategy and the market simulator were the same. Indeed they are, that's a good news! 

4.	To make the code work, you will need to change the directory of the ""spy_ret.csv"" location, that is the file I use to import market data to the C++ project. 

	--> main.cpp line 35
	-->classes/BookBuilder.h line 17
	-->goog_test/tests/temp/BookBuilder.h line 17

5.	There should be 17 Google Tests separated from main. Hopefully my installation will work outside my local computer.

6.	I did an implementation of a SmartPointer class and use it in the OrderManager in the handle_order function. 



Good luck, hopefully I kept everything organized enough and there are enough comments in my unittests to understand their purpose and what they are checking. 

Laurent Lanteigne
	
