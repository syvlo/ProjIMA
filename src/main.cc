#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "TVL0DecompositionMinimizer.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"
#include "RayleighDataTerm2Vars.hh"
#include "RiceDataTerm.hh"
#include "LogDataTerm.hh"
#include "ImwHelper.hh"

#define COMPUTE_WINDOW_SIZE (100)
#define FILL_WINDOW_SIZE (50)

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

		unsigned nbNodes = input.size().width * input.size().height * alpha.size() + 2;
		unsigned nbArcs = 6 * nbNodes;
		unsigned nbBytes = (nbNodes + nbArcs) * 8; //Weights in double.
		std::clog << "Éstimation de la mémoire pour la construction du graph: "
				  << nbBytes / (1024 * 1024) << "Mo" << std::endl;

		cv::Mat outputBV(input.size(), input.type());
		cv::Mat outputS(input.size(), input.type());
		cv::Mat outputC(input.size(), input.type());

		for (int i = 0; i < input.size().height - FILL_WINDOW_SIZE;
			 i += FILL_WINDOW_SIZE)
		{
			for (int j = 0; j < input.size().width - FILL_WINDOW_SIZE;
				 j += FILL_WINDOW_SIZE)
			{
				int endi = i + COMPUTE_WINDOW_SIZE;
				if (endi >= input.size().height)
					endi = input.size().height - 1;
				int endj = j + COMPUTE_WINDOW_SIZE;
				if (endj >= input.size().width)
					endj = input.size().width - 1;

				cv::Rect ComputeRegion(j, i, endj - j, endi - i);
				cv::Mat inputCropped = input(ComputeRegion);


				int startIFill = i + (COMPUTE_WINDOW_SIZE - FILL_WINDOW_SIZE) / 2;
				if (i == 0)
					startIFill = 0;
				int endIFill = i + (COMPUTE_WINDOW_SIZE + FILL_WINDOW_SIZE) / 2;
				if (endi == input.size().height - 1)
					endIFill = input.size().height - 1;

				int startJFill = j + (COMPUTE_WINDOW_SIZE - FILL_WINDOW_SIZE) / 2;
				if (j == 0)
					startJFill = 0;
				int endJFill = j + (COMPUTE_WINDOW_SIZE + FILL_WINDOW_SIZE) / 2;
				if (endj == input.size().width - 1)
					endJFill = input.size().width - 1;
				std::clog << "Compute on rectangle from (" << i << ", " << j << ") to ("
						  << endi << ", " << endj << ")" << std::endl;
				std::clog << "And filling from (" << startIFill << ", " << startJFill
						  << ") to (" << endIFill << ", " << endJFill << ")" << std::endl;

				cv::Rect FillRegion(startJFill, startIFill, endJFill - startJFill,
									endIFill - startIFill);

				TVL0DecompositionMinimizer<RayleighDataTerm2Vars<unsigned, unsigned> > minimizer(alpha, gamma, args.getBetaBV(), args.getBetaS());
				if (minimizer.compute(inputCropped))
				{
					WriteImw(minimizer.getOutputBV(), "test");
					cv::Mat oBV(outputBV, FillRegion);
					minimizer.getOutputBV().copyTo(oBV);
					WriteImw(outputBV, "test2");
					outputS(FillRegion) = minimizer.getOutputS();
					outputC(FillRegion) = minimizer.getOutputComplete();
				}
				else
				{
					std::cerr << "Something wrong happened during computation." << std::endl;
					return (1);
				}

			}
		}

		if (!args.getRadarMode())
			cv::imwrite(args.getOutputImageBV(), outputBV);
		else
			WriteImw(outputBV, args.getOutputImageBV());
		if (!args.getRadarMode())
			cv::imwrite(args.getOutputImageS(), outputS);
		else
			WriteImw(outputS, args.getOutputImageS());
		if (args.getOutputImageComplete())
		{
			if (!args.getRadarMode())
				cv::imwrite(args.getOutputImageComplete(), outputC);
			else
				WriteImw(outputC, args.getOutputImageComplete());
		}


    }

    return (0);
}
