#include "ImwHelper.hh"

#include <cv.h>

#include <fstream>
#include <exception>
#include <cmath>
#include <cstring>

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
		unsigned short value = (unsigned char)buffer[0] * 256 + (unsigned char)buffer[1];
		output.at<unsigned short>(i, j) = value;
	    }
    }
    else
    {
	throw std::runtime_error("Problem when opening imw file.");
    }

    return output;
}

cv::Mat
ReadImw (const char* FileName)
{
    char dim[4242];
    char imw[4242];

    strcpy(dim, FileName);
    strcat(dim, ".dim");

    strcpy(imw, FileName);
    strcat(imw, ".imw");

    return ReadImw (dim, imw);
}

cv::Mat
convertTo8U (const cv::Mat	Input,
	     double		nsigma)
{
    unsigned Height = Input.size().height;
    unsigned Width = Input.size().width;
    double nbPix = Width * Height;

    double mu = 0;

    for (unsigned i = 0; i < Height; ++i)
	for (unsigned j = 0; j < Width; ++j)
	{
	    mu += (double)Input.at<unsigned short>(i, j);
	}
    mu /= nbPix;

    std::clog << "Mu = " << mu << std::endl;


    double sigma = 0;
    for (unsigned i = 0; i < Height; ++i)
	for (unsigned j = 0; j < Width; ++j)
	{
	    sigma += ((double)Input.at<unsigned short>(i, j) - mu)
		* ((double)Input.at<unsigned short>(i, j) - mu);
	}
    sigma = sqrt(sigma / (nbPix - 1));

    std::clog << "Sigma = " << sigma << std::endl;


    double thresh = mu + nsigma * sigma;

    cv::Mat output (Height, Width, CV_8U);
    for (unsigned i = 0; i < Height; ++i)
	for (unsigned j = 0; j < Width; ++j)
	{
	    unsigned short value = Input.at<unsigned short>(i, j);
	    if (value > thresh)
		value = thresh;
	    output.at<unsigned char>(i, j) = (double)(value * 255) / thresh;
	}
    
    return output;
}
