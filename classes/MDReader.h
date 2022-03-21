#ifndef LAURENT_FINAL_PROJECT_MDREADER_H
#define LAURENT_FINAL_PROJECT_MDREADER_H


#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "BookUpdate.h"
using namespace boost::posix_time;

using namespace std;

class MDReader
{
    const std::string fileName;
    const std::string delimeter;
    const unsigned int number_of_line;
    const bool is_header;





public:
    MDReader(std::string filename,
            std::string delm = ",",
            unsigned int number_of_line_ = 10,
            bool is_header_ = true) :
            fileName(filename),
            delimeter(delm),
            number_of_line(number_of_line_),
            is_header(is_header_)
    { }

    /*
    * Parses through csv file line by line and returns the data
    * in vector of vector of strings.
    */
    std::vector<BookUpdate> getData()
    {
        unsigned int current_number_of_line=0;
        std::ifstream file(fileName);

        if (!file)
        {
            cerr << "File could not be opened!\n"; // Report error
            cerr << "Error code: " << strerror(errno); // Get some info as to why
            exit(1);
        }

        std::vector<BookUpdate> dataList;

        std::string line = "";
        // Iterate through each line and split the content using delimeter
        bool is_header_handled=false;
        unsigned int day_since_start = 0;
        while (getline(file, line))
        {
            if(is_header and !is_header_handled)
            {
                is_header_handled=true;
                continue;
            }
            line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
            std::vector<std::string> vec;
            boost::algorithm::split(vec, line, boost::is_any_of(delimeter));

            day_since_start ++;

            BookUpdate b1(0,std::stod(vec[1]),100,"LL",true,"SPY",day_since_start);
           // BookUpdate b2(0,std::stod(vec[1]),100,"LL",false,"SPY",day_since_start);
            dataList.push_back(b1);
           // dataList.push_back(b2);

            if(number_of_line!=0 and current_number_of_line++>=number_of_line)
            {
                break;
            }
        }
        // Close the File
        file.close();

        return dataList;
    }
};

#endif //LAURENT_FINAL_PROJECT_MDREADER_H