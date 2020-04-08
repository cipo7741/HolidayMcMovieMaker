# Holiday Video Maker (⚠️ work in progress)

Given a directory it shall parse all images and create a video out of it.

## Personal wish list

- *a configuration file can define*
    - music
	- normal visibility time
	- transition type
	- highlight images
		- longer visible
		- zoomable coordinates
		- change transition

## Getting Started

    for file in *.JPG; do convert $file -resize 1920x1440 -size 1920x1440 xc:black +swap -gravity center -composite black/$file; done

    ffmpeg -framerate 1/3 -pattern_type glob -i 'black/*.JPG' -i ~/Music/KieLoKaz_-_01_-_Reunion_of_the_Spaceducks_Kielokaz_ID_365.mp3 -c:a copy -shortest -c:v libx264 -r 30 out.mp4    

### Prerequisites

Currently uses CMake, Boost and OpenCV.

### Installing

- see Getting Started

## Running the tests

- not implemented yet

## Built With

* [CMake](https://cmake.org/) - CMake is an open-source, cross-platform family of tools designed to build, test and package software.
* [Boost](https://www.boost.org/) - Boost provides free peer-reviewed portable C++ source libraries.
* [OpenCV](https://opencv.org/) - OpenCV (Open Source Computer Vision Library) is an open source computer vision and machine learning software library.

## Contributing

- one man show

## Versioning

- no versions yet

## Authors

- me

## License

This project (cipo7741/HolidayMcMovieMaker) is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

* Hat tip to CMake, Boost and OpenCV.
