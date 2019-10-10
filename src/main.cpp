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
                    //cv::imshow( itr->path().filename().string() , src1 );
                    //cv::waitKey(0);
                    //cv::destroyWindow(itr->path().filename().string()); //destroy the created window
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
        x = current - start;
        return (double) x / max;
    }
}

int main( void ) {
        
    std::vector<std::string> nAllowedExtensions = {".png",".jpg",".JPG"};
    std::vector<std::string> imageFiles = listImageFilesInDir(nAllowedExtensions);
    
    int frames_per_second = 10;

    if(imageFiles.size() > 0){
        cv::Mat tmp_frame = cv::imread(imageFiles[0]);
        int frame_width = static_cast<int>(tmp_frame.cols); //get the width of frames of the video
        std::cout << frame_width << std::endl; 
        int frame_height = static_cast<int>(tmp_frame.rows); //get the height of frames of the video
        std::cout << frame_height << std::endl; 
        cv::Size frame_size(frame_width, frame_height);
        //Create and initialize the VideoWriter object 
        cv::VideoWriter oVideoWriter("MyHolidayVideo.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true); 
        //If the VideoWriter object is not initialized successfully, exit the program
        if (oVideoWriter.isOpened() == false) {
            std::cout << "Cannot save the video to a file" << std::endl;
            std::cin.get(); //wait for any key press
            return -1;
        }
        for(long unsigned int i = 0; i < imageFiles.size(); i++){
            std::string imageFile = imageFiles[i];
            std::cout << imageFile << std::endl;
            cv::Mat frame = cv::imread(imageFile);
            if(i < imageFiles.size()-1){
                std::string nextImageFile = imageFiles[i];
                cv::Mat nextFrame = cv::imread(imageFile);
            }
            //cv::imshow( imageFile , frame );
            //cv::waitKey(0);
            //cv::destroyWindow( imageFile ); //destroy the created window
            for(int i = 0; i < 100; i++){
                
                alpha = (double) alpha_slider/alpha_slider_max ;
                beta = ( 1.0 - alpha );
                addWeighted( src1, alpha, src2, beta, 0.0, dst);
                imshow( "Linear Blend", dst );
                
                oVideoWriter.write(frame); 
            }
        }
        oVideoWriter.release();
    }
    return 0;
}
