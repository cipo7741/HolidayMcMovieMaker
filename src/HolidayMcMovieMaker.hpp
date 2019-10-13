/*
 * This file is part of the project "HolidayMcMovieMaker"
 * 
 * It is just a fun project to watch a video of some holiday memories. 
 * 
 * @author: claudia.sichting@uni-jena.de
 * October 2019
*/

#ifndef HOLIDAY_MC_MOVIE_MAKER_HPP
#define HOLIDAY_MC_MOVIE_MAKER_HPP

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

#include "boost/filesystem.hpp"

#include <thirdparty/plog/Severity.h>
#include <thirdparty/plog/Formatters/CsvFormatter.h>
#include <thirdparty/plog/Formatters/TxtAppendFuncFormatter.hpp>
#include <thirdparty/plog/Appenders/ColorConsoleAppender.h>
#include <thirdparty/plog/Log.h>


namespace mcmoviemaker {

    /** \brief This class acts as access point to the
     * HolidayMcMovieMaker framework.
     *
     * Initializes the logger:
     * Logger: https://github.com/SergiusTheBest/plog#introduction
     *
     * Reads the image directory from argv:
     * e.g. ./HolidayMcMovieMaker "awesome/holiday_images"
     *
     * Only method HolidayMcMovieMaker::run() starts the video creation.
     *
     * @see Config::getType()
     * @see FittingController::run()
     * @see SimpleController::run()
     *
     */
    class HolidayMcMovieMaker {
    public:

        /// init logger, init configFilename
        HolidayMcMovieMaker(int argc, char **argv);

        std::vector<std::string> listImageFilesInDir(std::vector<std::string> &extensions);
        
        double getTransitionWeight(int current, int start, int max);

        /**
         * Reads the xml config file into Config. @see Config
         * Starts SimpleController or FittingController.
         * FittingController will internally call SimpleController.
         */
        int run();

    private:

        /// logging parameter @see plog::Severity
        void initializeLogger(int plogLoggingSeverity = plog::info);

        //std::string configFilename_;
        
        boost::filesystem::path imageDirectory_;


    };
}


#endif //HOLIDAY_MC_MOVIE_MAKER_HPP
