#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "TVL0DecompositionMinimizer.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"
#include "RiceDataTerm.hh"
#include "ImwHelper.hh"

int main (int argc, char* argv[])
{
    Args args(argc, argv);
    if (args.getHelp()) //Help was asked and has been printed, leave.
	return (0);

    if (!args.checkConsistency())
    {
	std::cerr << "Error: in non-WindowMode, you have to define input and output images." << std::endl;
	args.printHelp();
	return (1);
    }

    std::cout << args << std::endl;

    //    FIX ME
    //    Alpha definition (labels for BV image).
    std::vector<unsigned> alpha;
    if (!args.getRadarMode())
    	for (unsigned i = 0; i < 255; ++i)
    	    alpha.push_back(i);
    else
    {
    	// for (unsigned i = 0; i < 800; i += 10)
    	//     alpha.push_back(i);
    	// for (unsigned i = 800; i < 3000; i += 100)
    	//     alpha.push_back(i);
    	for (unsigned i = 0; i < 3000; i += 30)
    	    alpha.push_back(i);
    }

    //Gamma definition (labels for S image).
    std::vector<unsigned> gamma;
    for (unsigned i = 0; i < 10000; i += 100)
    {
	gamma.push_back(i);
    }

    TVL0DecompositionMinimizer<RiceDataTerm<unsigned, unsigned> > minimizer(alpha, gamma, args.getBetaBV(), args.getBetaS());

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
	    if (!args.getRadarMode())
		cv::imwrite(args.getOutputImageS(), minimizer.getOutputS());
	    else
		WriteImw(minimizer.getOutputS(), args.getOutputImageS());
	}
	else
	{
	    std::cerr << "Something wrong happened during computation." << std::endl;
	    return (1);
	}
    }

    return (0);
}
