#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "TVL0DecompositionMinimizer.hh"
#include "ComputeByParts.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"
#include "RayleighDataTerm2Vars.hh"
#include "RiceDataTerm.hh"
#include "LogDataTerm.hh"
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

    //Alpha definition (labels for BV image).
    std::vector<unsigned> alpha;
    if (!args.getRadarMode())
    	for (unsigned i = 0; i < 255; ++i)
    	    alpha.push_back(i);
    else
    {
		//For almost all radar distribution, this should be used.
    	for (unsigned i = 0; i < 800; i += 40)
    	    alpha.push_back(i);
    	for (unsigned i = 800; i < 3000; i += 100)
    	    alpha.push_back(i);


		// //For a log (* 100) distribution
		// for (unsigned i = 0; i < 500; i+= 100)
		// 	alpha.push_back(i);
		// for (unsigned i = 500; i < 1500; i += 40)
		// 	alpha.push_back(i);
		// for (unsigned i = 1500; i < 2000; i += 100)
		// 	alpha.push_back(i);
    }

    //Gamma definition (labels for S image).
    std::vector<unsigned> gamma;
    for (unsigned i = 0; i < 10000; i += 100)
    {
		gamma.push_back(i);
    }
    for (unsigned i = 10000; i < 100000; i += 1000)
    {
		gamma.push_back(i);
    }


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

		cv::Mat OutputBV;
		cv::Mat OutputS;
		cv::Mat OutputC;

		TVL0DecompositionMinimizer<RayleighDataTerm2Vars<unsigned, unsigned> > minimizer(alpha, gamma, args.getBetaBV(), args.getBetaS());

		if (args.getNonOptimalMode())
		{
			ComputeByParts<TVL0DecompositionMinimizer<RayleighDataTerm2Vars<unsigned, unsigned> > > computer(100, 50, minimizer);
			computer.compute(input);

			OutputBV = computer.getOutputBV();
			OutputS = computer.getOutputS();
			OutputC = computer.getOutputC();
		}
		else
		{
			minimizer.compute(input);

			OutputBV = minimizer.getOutputBV();
			OutputS = minimizer.getOutputS();
			OutputC = minimizer.getOutputComplete();
		}

		if (!args.getRadarMode())
		    cv::imwrite(args.getOutputImageBV(), OutputBV);
		else
		    WriteImw(OutputBV, args.getOutputImageBV());
		if (!args.getRadarMode())
		    cv::imwrite(args.getOutputImageS(), OutputS);
		else
		    WriteImw(OutputS, args.getOutputImageS());
		if (args.getOutputImageComplete())
		{
		    if (!args.getRadarMode())
			cv::imwrite(args.getOutputImageComplete(), OutputC);
		    else
				WriteImw(OutputC, args.getOutputImageComplete());
		}


    }

    return (0);
}
