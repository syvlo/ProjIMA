#include <iostream>
#include <vector>

#include <ImwHelper.hh>
#include <cv.h>
#include <highgui.h>

#include "Args.hh"
#include "QuadraticDataTerm.hh"
#include "RayleighDataTerm.hh"
#include "RayleighDataTerm2Vars.hh"
#include "RiceDataTerm.hh"
#include "LogDataTerm.hh"
#include "Engine.hh"
#include "utils.hh"
#include "AlphaBuilder.hh"

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

	AlphaBuilder builder(args);
	std::vector <unsigned> alpha = builder.getQuantificationVector();


	std::vector<cv::Mat> OutputsBV;
	std::vector<cv::Mat> OutputsS;
	std::vector<cv::Mat> OutputsC;

	if (args.getDataTerm() == RAYLEIGH)
	{
		Engine<RayleighDataTerm2Vars<unsigned, unsigned> > engine (alpha);
		engine.Compute(inputs, OutputsBV, OutputsS, OutputsC, args);
	}
	else if (args.getDataTerm() == RICE)
	{
		Engine<RiceDataTerm<unsigned, unsigned> > engine (alpha);
		engine.Compute(inputs, OutputsBV, OutputsS, OutputsC, args);
	}
	else if (args.getDataTerm() == LOG)
	{
		Engine<LogDataTerm<unsigned, unsigned> > engine (alpha);
		engine.Compute(inputs, OutputsBV, OutputsS, OutputsC, args);
	}
	else if (args.getDataTerm() == GAUSSIAN)
	{
		Engine<QuadraticDataTerm<unsigned, unsigned> > engine (alpha);
		engine.Compute(inputs, OutputsBV, OutputsS, OutputsC, args);
	}


	for (unsigned i = 0; i < inputs.size(); ++i)
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


		if (i < OutputsBV.size())
		{
			std::clog << "Writing BV image to " << OBVName << std::endl;
			if (!args.getRadarMode())
				cv::imwrite(OBVName, OutputsBV[i]);
			else
				WriteImw(OutputsBV[i], OBVName.c_str());
		}

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



    return (0);
}
