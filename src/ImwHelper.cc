#include "ImwHelper.hh"

#include <cv.h>

#include <fstream>
#include <exception>

cv::Mat
ReadImw (const char* DimFileName,
	 const char* ImwFileName)
{
    std::ifstream dimFile (DimFileName, std::ifstream::in);
    int height = -1;
    int width = -1;

    if (dimFile.is_open())
    {
	dimFile >> height;
	dimFile >> width;
	dimFile.close();
    }
    else
    {
	throw std::runtime_error("Problem when opening dim file.");
    }

    std::clog << "Opening " << ImwFileName << " with dimensions h = " << height
	      << " and width = " << width << std::endl;

    cv::Mat output(height, width, CV_16U);

    std::ifstream imwFile (ImwFileName, std::ifstream::in);
    if (imwFile.is_open())
    {
	char buffer[2];
	for (int i = 0; i < height; ++i)
	    for (int j = 0; j < width; ++j)
	    {
		imwFile.read(buffer, 2);
		output.at<unsigned short>(i, j) = (unsigned short)*buffer;
	    }
    }
    else
    {
	throw std::runtime_error("Problem when opening imw file.");
    }

    return output;
}
