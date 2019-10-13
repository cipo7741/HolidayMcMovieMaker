
#include "HolidayMcMovieMaker.hpp"

    mcmoviemaker::HolidayMcMovieMaker::HolidayMcMovieMaker(int argc, char **argv) {
        std::string severityString = "DEBUG";
        initializeLogger(plog::severityFromString(severityString.c_str()));
        
        imageDirectory_ = "exampleImages";
        //configFilename_ = "resources/test/input/config-example.xml";
        if (argc > 1) {
            imageDirectory_ = argv[1];
            //configFilename_ = argv[1];
            LOG_INFO << "HolidayMcMovieMaker. Now running on directory \"" + imageDirectory_.string() + "\"";
        } else if (argc > 2) {
            // "FATAL";"ERROR";"WARN";"INFO";"DEBUG" or "VERBOSE";
            severityString = argv[2];
        } else {
            LOG_DEBUG << "Specify image directory as command line argument. Now running with \"" + imageDirectory_.string() + "\"";
        }
    }

    std::vector<std::string> mcmoviemaker::HolidayMcMovieMaker::listImageFilesInDir(std::vector<std::string> &extensions){
        std::vector<std::string> nFilesFound;
        if ( !boost::filesystem::exists( imageDirectory_ ) ) std::cout << "Path not found: " << imageDirectory_ << std::endl;
        boost::filesystem::directory_iterator end_itr;
        for ( boost::filesystem::directory_iterator itr( imageDirectory_ ); itr != end_itr; ++itr ) {
            if ( boost::filesystem::is_regular_file(itr->status())) {
                if (std::find(extensions.begin(), extensions.end(), itr->path().extension()) != extensions.end()){
                    cv::Mat src1 = cv::imread( itr->path().string() );
                    if( !src1.empty() ) {
                        nFilesFound.emplace_back(itr->path().string());
                    }
                } else {
                    LOG_DEBUG << "Sorry, the file extension " << itr->path().extension() << " is not supported.";
                    LOG_DEBUG << "Supported file extensions: ";
                    for(auto &extension : extensions){
                        LOG_DEBUG << "    " << extension;
                    }
                }
            }
        }
        return nFilesFound;
    }

    double mcmoviemaker::HolidayMcMovieMaker::getTransitionWeight(int current, int start, int max){
        if(current <= start){
            return 0.0;
        } else {
            double x = current - start;
            return (double) x / (max - start);
        }
    }

    int mcmoviemaker::HolidayMcMovieMaker::run() {
        std::vector<std::string> nAllowedExtensions = {".png",".jpg",".JPG"};
        std::vector<std::string> imageFiles = listImageFilesInDir(nAllowedExtensions);
        
        int frames_per_second = 20;
        int img_repeat = 60;
        int transition_start = 40;
        int num_of_images = imageFiles.size();
        
        std::sort(imageFiles.begin(), imageFiles.end(),
              [] (std::string const& a, std::string const& b) { return a < b; });

        if(imageFiles.size() > 0){
            cv::Mat tmp_frame = cv::imread(imageFiles[0]);
            int frame_width = static_cast<int>(tmp_frame.cols);
            int frame_height = static_cast<int>(tmp_frame.rows);
            cv::Size frame_size(frame_width, frame_height);
            cv::VideoWriter oVideoWriter("MyHolidayVideo.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true); 
            if (oVideoWriter.isOpened() == false) {
                LOG_ERROR << "Cannot save the video to a file" << std::endl;
                std::cin.get();
                return -1;
            }
            for(int imageIndex = 0; imageIndex < num_of_images; imageIndex++){
                std::string imageFile = imageFiles[imageIndex];
                LOG_INFO << imageFile;
                cv::Mat frame = cv::imread(imageFile);
                if(imageIndex < num_of_images-1){
                    std::string nextImageFile = imageFiles[imageIndex+1];
                    cv::Mat nextFrame = cv::imread(nextImageFile);
                    //cv::imshow("frame", frame);
                    //cv::waitKey();
                    //cv::imshow("nextFrame", nextFrame);
                    //cv::waitKey();
                    //cv::addWeighted( frame, 0.5, nextFrame, 0.5, 0.0, frame);
                    //cv::imshow("weighted", frame);
                    //cv::waitKey();
                    for(int imageRepeatIndex = 0; imageRepeatIndex < img_repeat; imageRepeatIndex++){
                        if (imageRepeatIndex >= transition_start){
                            double alpha = getTransitionWeight(imageRepeatIndex, transition_start, img_repeat);
                            //std::cout << alpha << std::endl;
                            double beta = ( 1.0 - alpha );
                            cv::Mat dst;
                            cv::addWeighted( frame, beta, nextFrame, alpha, 0.0, dst);
                            oVideoWriter.write(dst);
                            //cv::addWeighted( frame, 0.5, nextFrame, 0.5, 0.0, dst);
                        } else {
                            oVideoWriter.write(frame); 
                        }
                    }
                } else {
                    for(int imageRepeatIndex = 0; imageRepeatIndex < img_repeat; imageRepeatIndex++){
                        oVideoWriter.write(frame);
                    }
                }
                
            }
            oVideoWriter.release();
        }
        return 0;
    }

    void mcmoviemaker::HolidayMcMovieMaker::initializeLogger(int plogLoggingSeverity) {
        // Create the 1st appender.
        static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("mcmoviemaker.log", 100000, 3);
        // Create the 2nd appender.
        static plog::ColorConsoleAppender<plog::TxtAppendFuncFormatter> consoleAppender;
        // Initialize the logger with the both appenders.
        plog::init(plog::Severity(plogLoggingSeverity), &fileAppender).addAppender(&consoleAppender);
    }

