#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "TVL0DecompositionMinimizer.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"

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

    //FIX ME
    std::vector<unsigned> alpha;
    for (unsigned i = 0; i < 255; ++i)
    	alpha.push_back(i);
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
	cv::Mat input = cv::imread(args.getInputImage(), CV_LOAD_IMAGE_GRAYSCALE);
	if (minimizer.compute(input))
	{
	  cv::imwrite(args.getOutputImageBV(), minimizer.getOutputBV());
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
