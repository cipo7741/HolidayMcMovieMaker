/*
 * This file is part of the project "HolidayMcMovieMaker"
 * 
 * It is just a fun project to watch a video of some holiday memories. 
 * 
 * @author: claudia.sichting@uni-jena.de
 * October 2019
*/

//#define CATCH_CONFIG_RUNNER

//#include <catch.hpp>
#include <HolidayMcMovieMaker.hpp>

namespace mc = mcmoviemaker;

/// TODO: runs all tests at 'src/test/'
//int runTests() {
//    int result = Catch::Session().run();
//    return (result < 0xff ? result : 0xff);
//}

// Let's start the HolidayMcMovieMaker and run some tests afterwards.
int main(int argc, char **argv) {
    /// 1. init HolidayMcMovieMaker with command line arguments
    /// 1.1. should contain the XML configuration file
    mc::HolidayMcMovieMaker holidayMcMovieMaker(argc, argv);
    return holidayMcMovieMaker.run();
    /// 2. runTests() needs <code>#define CATCH_CONFIG_RUNNER</code>
    // return runTests();

}
