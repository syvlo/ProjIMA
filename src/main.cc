#include <iostream>
#include <ImwHelper.hh>
#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "TVL0DecompositionMinimizer.hh"
#include "ComputeByParts.hh"
#include "ComputeByPartsLinear.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"
#include "RayleighDataTerm2Vars.hh"
#include "RiceDataTerm.hh"
#include "LogDataTerm.hh"

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
		std::vector<cv::Mat> inputs;
		const std::vector<char*> inputsNames = args.getInputImages();;

		for (std::vector<char*>::const_iterator it = inputsNames.begin();
			 it != inputsNames.end(); ++it)
		{
			if (!args.getRadarMode())
				inputs.push_back(cv::imread(*it, CV_LOAD_IMAGE_GRAYSCALE));
			else
				inputs.push_back(ReadImw(*it));
		}

		std::vector<cv::Mat> OutputsBV;
		std::vector<cv::Mat> OutputsS;
		std::vector<cv::Mat> OutputsC;


//		RiceDataTerm<unsigned, unsigned>::init();

		TVL0DecompositionMinimizer<RayleighDataTerm2Vars<unsigned, unsigned> > minimizer(alpha, gamma, args.getBetaBV(), args.getBetaS());

		if (args.getNonOptimalMode())
		{
			if (args.getShiftWindow() != args.getFillingWindowSize())
			{
				ComputeByPartsLinear<TVL0DecompositionMinimizer<RayleighDataTerm2Vars<unsigned, unsigned> > > computer(args.getComputeWindowSize(), args.getFillingWindowSize(), args.getShiftWindow(), minimizer);

				computer.compute(inputs);

				OutputsBV = computer.getOutputsBV();
				OutputsS = computer.getOutputsS();
				OutputsC = computer.getOutputsC();
			}
			else
			{
				ComputeByParts<TVL0DecompositionMinimizer<RayleighDataTerm2Vars<unsigned, unsigned> > > computer(args.getComputeWindowSize(), args.getFillingWindowSize(), minimizer);

				computer.compute(inputs);

				OutputsBV = computer.getOutputsBV();
				OutputsS = computer.getOutputsS();
				OutputsC = computer.getOutputsC();
			}
		}
		else
		{
			minimizer.compute(inputs);

			OutputsBV = minimizer.getOutputsBV();
			OutputsS = minimizer.getOutputsS();
			OutputsC = minimizer.getOutputsComplete();
		}

		for (unsigned i = 0; i < OutputsBV.size(); ++i)
		{
			std::string OBVName(args.getOutputImageBV());
			if (OutputsBV.size() > 1)
				OBVName.append(std::to_string(i));

			std::string OSName(args.getOutputImageS());
			if (OutputsS.size() > 1)
				OSName.append(std::to_string(i));

			std::string OCName(args.getOutputImageComplete());
			if (args.getOutputImageComplete())
			{
				if (OutputsC.size() > 1)
					OCName.append(std::to_string(i));
			}


			std::clog << "Writing BV image to " << OBVName << std::endl;
			if (!args.getRadarMode())
				cv::imwrite(OBVName, OutputsBV[i]);
			else
				WriteImw(OutputsBV[i], OBVName.c_str());

			std::clog << "Writing S image to " << OSName << std::endl;
			if (!args.getRadarMode())
				cv::imwrite(OSName, OutputsS[i]);
			else
				WriteImw(OutputsS[i], OSName.c_str());

			if (args.getOutputImageComplete())
			{
				std::clog << "Writing C image to " << OCName << std::endl;
				if (!args.getRadarMode())
					cv::imwrite(OCName, OutputsC[i]);
				else
					WriteImw(OutputsC[i], OCName.c_str());
			}
		}
//		RiceDataTerm<unsigned, unsigned>::destroy();
    }



    return (0);
}
