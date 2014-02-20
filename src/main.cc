#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "TVL0DecompositionMinimizer.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"
#include "ImwHelper.hh"

int main (int argc, char* argv[])
{
    Args args(argc, argv);
    if (!args.checkConsistency())
    {
	std::cerr << "Error: in non-WindowMode, you have to define input and output images." << std::endl;
	args.printHelp();
	return (1);
    }

    std::cout << args << std::endl;

    // cv::Mat tmp = ReadImw("test");
    // cv::Rect roi (100, 100, 100, 100);
    // cv::Mat cropped = tmp(roi).clone();
    // WriteImw(cropped, "cropped");

    //FIX ME
    //Alpha definition (labels for BV image).
    std::vector<unsigned> alpha;
    if (!args.getRadarMode())
	for (unsigned i = 0; i < 255; ++i)
	    alpha.push_back(i);
    else
    {
	for (unsigned i = 0; i < 800; i += 2)
	    alpha.push_back(i);
	for (unsigned i = 800; i < 3000; i += 10)
	    alpha.push_back(i);
    }

    //Gamma definition (labels for S image).
    unsigned gamma_tmp[] = {0, 1, 2};
    std::vector<unsigned> gamma (gamma_tmp, gamma_tmp + sizeof(gamma_tmp) / sizeof(gamma_tmp));

    TVL0DecompositionMinimizer<RayleighDataTerm<unsigned, unsigned> > minimizer(alpha, gamma, args.getBetaBV(), args.getBetaS());

    if (args.getWindowMode())
    {
	//FIX ME
	std::cerr << "Window mode not handled." << std::endl;
	return (1);
    }
    else
    {
	cv::Mat input;
	if (!args.getRadarMode())
	    input = cv::imread(args.getInputImage(), CV_LOAD_IMAGE_GRAYSCALE);
	else
	    input = ReadImw(args.getInputImage());

	if (minimizer.compute(input))
	{
	    if (!args.getRadarMode())
		cv::imwrite(args.getOutputImageBV(), minimizer.getOutputBV());
	    else
		WriteImw(minimizer.getOutputBV(), args.getOutputImageBV());
	    //cv::imwrite(args.getOutputImageS(), minimizer.getOutputS());
	}
	else
	{
	    std::cerr << "Something wrong happened during computation." << std::endl;
	    return (1);
	}
    }

    return (0);
}
