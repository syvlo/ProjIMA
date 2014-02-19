#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "ImwHelper.hh"

void printHelp()
{
    std::cout << "./Viewer image1 [image2]" << std::endl
	      << "Warning: do not put the extensions for the images."
	      << std::endl
	      << "If 2 images, the first one will be used to compute mu+3sigma."
	      << std::endl;
}

int main (int argc, char* argv[])
{
    if (argc == 1 || argc > 3)
    {
	printHelp();
	return (1);
    }

    cv::Mat img1 = ReadImw(argv[1]);
    cv::Mat img2;

    if (argc == 3)
	img2 = ReadImw(argv[2]);

    double thresh;
    cv::Mat img1_8U = convertTo8U(img1, 3, &thresh);
    cv::Mat img2_8U;

    if (argc == 3)
	img2_8U = convertTo8UUsingThresh(img2, thresh);

    cv::Mat wholeImage;
    if (argc == 2)
    {
	wholeImage = img1_8U;
    }
    else
    {
	unsigned height = img1_8U.size().height;
	if (img2_8U.size().height > height)
	    height = img2_8U.size().height;

	unsigned width = img1_8U.size().width + img2_8U.size().width + 3;
	wholeImage = cv::Mat(height, width, CV_8U);

	cv::Mat left(wholeImage, cv::Rect(0, 0, img1_8U.size().width,
					  img1_8U.size().height));
	img1_8U.copyTo(left);
	cv::Mat right(wholeImage, cv::Rect(img1_8U.size().width + 3, 0,
					   img2_8U.size().width,
					   img2_8U.size().height));
	img1_8U.copyTo(right);
    }

    cv::imshow("Viewer", wholeImage);
    cv::waitKey(0);

    return (0);
}
