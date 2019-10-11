#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "boost/filesystem.hpp"

std::vector<std::string> listImageFilesInDir(std::vector<std::string> &extensions){
    std::vector<std::string> nFilesFound;
    const boost::filesystem::path & dir_path = "data";
    if ( !boost::filesystem::exists( dir_path ) ) std::cout << "Path not found: " << dir_path << std::endl;
    boost::filesystem::directory_iterator end_itr;
    for ( boost::filesystem::directory_iterator itr( dir_path ); itr != end_itr; ++itr ) {
        if ( boost::filesystem::is_regular_file(itr->status())) {
            if (std::find(extensions.begin(), extensions.end(), itr->path().extension()) != extensions.end()){
                cv::Mat src1 = cv::imread( itr->path().string() );
                if( !src1.empty() ) {
                    nFilesFound.emplace_back(itr->path().string());
                }
            } else {
                std::cout << "Sorry, the file extension " << itr->path().extension() << " is not supported." << std::endl;
                std::cout << "Supported file extensions: ";
                for(auto &extension : extensions){
                    std::cout << extension << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    return nFilesFound;
}

double getTransitionWeight(int current, int start, int max){
    if(current <= start){
        return 0.0;
    } else {
        double x = current - start;
        return (double) x / (max - start);
    }
}

int main( void ) {
        
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
            std::cout << "Cannot save the video to a file" << std::endl;
            std::cin.get();
            return -1;
        }
        for(int imageIndex = 0; imageIndex < num_of_images; imageIndex++){
            std::string imageFile = imageFiles[imageIndex];
            std::cout << imageFile << std::endl;
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
