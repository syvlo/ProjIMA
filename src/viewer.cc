#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "ImwHelper.hh"

#define WIN_NAME ("viewer")
#define BORDER (3)

void printHelp()
{
    std::cout << "./Viewer image1 [image2]" << std::endl
	      << "Warning: do not put the extensions for the images."
	      << std::endl
	      << "If 2 images, the first one will be used to compute mu+3sigma."
	      << std::endl;
}

struct data
{
    cv::Mat image8U;
    cv::Mat image8UOriginal;
    cv::Mat image1;
    cv::Mat image2;
    bool doubleImage;
};

void MouseCallBack(int event, int x, int y, int flags, void* userdata)
{
    struct data* Data = (struct data*)userdata;
    Data->image8U = Data->image8UOriginal.clone();
    if (event == cv::EVENT_MOUSEMOVE)
    {
	if (!Data->doubleImage)
	{
	    char buffer[8];
	    sprintf(buffer,"%d", (short)Data->image1.at<short> (y, x));
	    cv::putText(Data->image8U, buffer, cv::Point(10, 15), 0, 0.3, 255);
	    cv::imshow(WIN_NAME, Data->image8U);
	}
	else
	{
	    char buffer[8];
	    buffer[0] = 0;
	    if (x < Data->image1.size().width)
	    {
		sprintf(buffer,"%d", (short)Data->image1.at<short> (y, x));
	    }
	    if (x > Data->image1.size().width + BORDER)
	    {
		sprintf(buffer,"%d", (short)Data->image2.at<short> (y, x - Data->image1.size().width - BORDER));
	    }
	    cv::putText(Data->image8U, buffer, cv::Point(10, 15), 0, 0.3, 255);
	    cv::imshow(WIN_NAME, Data->image8U);
	}
    }
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

	unsigned width = img1_8U.size().width + img2_8U.size().width + BORDER;
	wholeImage = cv::Mat(height, width, CV_8U);

	cv::Mat left(wholeImage, cv::Rect(0, 0, img1_8U.size().width,
					  img1_8U.size().height));
	img1_8U.copyTo(left);
	cv::Mat right(wholeImage, cv::Rect(img1_8U.size().width + BORDER, 0,
					   img2_8U.size().width,
					   img2_8U.size().height));
	img1_8U.copyTo(right);
    }

    struct data userData;
    userData.image8U = wholeImage;
    userData.image8UOriginal = wholeImage;
    userData.image1 = img1;
    userData.image2 = img2;
    userData.doubleImage = (argc == 3);

    cv::namedWindow(WIN_NAME, 1);
    cv::setMouseCallback(WIN_NAME, MouseCallBack, &userData);
    cv::imshow(WIN_NAME, wholeImage);
    cv::waitKey(0);

    return (0);
}
