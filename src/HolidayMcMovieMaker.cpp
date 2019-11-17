
#include "HolidayMcMovieMaker.hpp"

    mcmoviemaker::HolidayMcMovieMaker::HolidayMcMovieMaker(int argc, char **argv) {
        // "FATAL";"ERROR";"WARN";"INFO";"DEBUG" or "VERBOSE";
        std::string severityString = "DEBUG";
        initializeLogger(plog::severityFromString(severityString.c_str()));
        imageDirectory_ = "exampleImages";
        //TODO configFilename_ = "resources/test/input/config-example.xml";
        if (argc > 1) {
            imageDirectory_ = argv[1];
            //TODO  configFilename_ = argv[1];
            LOG_INFO << "HolidayMcMovieMaker. Now running on directory \"" + imageDirectory_.string() + "\"";
        } else if (argc > 2) {
            // "FATAL";"ERROR";"WARN";"INFO";"DEBUG" or "VERBOSE";
            severityString = argv[2];
        } else {
            LOG_DEBUG << "Specify image directory as command line argument. Now running with \"" + imageDirectory_.string() + "\"";
        }
    }

    std::vector<std::string> mcmoviemaker::HolidayMcMovieMaker::listImageFilesInDir(const std::vector<std::string> &extensions){
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

    cv::Mat mcmoviemaker::HolidayMcMovieMaker::fitImage(const cv::Mat& src, int wantedWidth, int wantedHeight){
        cv::Mat dst_wide = src.clone();
        if(src.rows > src.cols){
            //cv::Mat dst2 = src.clone();
            int space = static_cast<int> (src.rows - src.cols);
            LOG_INFO << "adding border of " << space << " px";
            //Size(w,h)
            cv::copyMakeBorder( src, dst_wide, 0, 0, space, space, cv::BORDER_CONSTANT, 0 );
            //dst1 = dst2.clone();
        }
        cv::Mat dst_fit = dst_wide.clone();
        cv::resize(dst_wide, dst_fit, cv::Size(wantedWidth, wantedHeight), 0, 0, cv::INTER_LINEAR);
        return dst_fit;
    }

    int mcmoviemaker::HolidayMcMovieMaker::run() {
        std::vector<std::string> nAllowedExtensions = {".png",".jpg",".JPG"};
        std::vector<std::string> nImageFiles = listImageFilesInDir(nAllowedExtensions);


        int frame_width = 1920; // cols
        int frame_height = 1440; // rows
            
        int frames_per_second = 20;
        int img_repeat = 60;
        int transition_start = 40;
        int num_of_images = nImageFiles.size();

        //std::string windowName = "HilidayMcMovieMaker";
        //cv::namedWindow(windowName, cv::WINDOW_KEEPRATIO); // Create Window
        
        
        std::sort(nImageFiles.begin(), nImageFiles.end(),
              [] (std::string const& a, std::string const& b) { return a < b; });

        if(nImageFiles.size() > 0){
            cv::Mat tmp_frame = cv::imread(nImageFiles[0]);
            cv::Size frame_size(frame_width, frame_height);
            cv::VideoWriter oVideoWriter("MyHolidayVideo.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true); 
            if (oVideoWriter.isOpened() == false) {
                LOG_ERROR << "Cannot save the video to a file" << std::endl;
                std::cin.get();
                return -1;
            }
            
            for(int imageIndex = 0; imageIndex < num_of_images; imageIndex++){
                std::string imageFile = nImageFiles[imageIndex];
                LOG_INFO << imageFile;
                cv::Mat frame = cv::imread(imageFile);
                if(frame.rows != frame_height || frame.cols != frame_width){
                    frame = fitImage(frame, frame_width, frame_height);
                    //cv::imshow("HilidayMcMovieMaker", frame);
                    //cv::waitKey();
                }
                LOG_INFO << "width: " << frame.cols << ", height: " << frame.rows;

                 if(imageIndex < num_of_images-1){
                    std::string nextImageFile = nImageFiles[imageIndex+1];
                    cv::Mat nextFrame = cv::imread(nextImageFile);
                    if(nextFrame.rows != frame_height || nextFrame.cols != frame_width){
                        nextFrame = fitImage(nextFrame, frame_width, frame_height);
                        //cv::imshow("HilidayMcMovieMaker", frame);
                        //cv::waitKey();
                    }
                    for(int imageRepeatIndex = 0; imageRepeatIndex < img_repeat; imageRepeatIndex++){
                        if (imageRepeatIndex >= transition_start){
                            double alpha = getTransitionWeight(imageRepeatIndex, transition_start, img_repeat);
                            double beta = ( 1.0 - alpha );
                            cv::Mat dst;
                            cv::addWeighted( frame, beta, nextFrame, alpha, 0.0, dst);
                            oVideoWriter.write(dst);
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
        static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("mcmoviemaker.log", 100000, 3);
        static plog::ColorConsoleAppender<plog::TxtAppendFuncFormatter> consoleAppender;
        plog::init(plog::Severity(plogLoggingSeverity), &fileAppender).addAppender(&consoleAppender);
    }

