#include "AlphaBuilder.hh"
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <ctime>

#include <ImwHelper.hh>
#include <cv.h>
#include <highgui.h>


AlphaBuilder::AlphaBuilder(const Args& args)
{
	if (args.getQPFilenameInput() != NULL)
		this->initFromFile(args.getQPFilenameInput(), args.getRadarMode());
	else
	{
		if (!args.getRadarMode())
		{
			this->initFromHisto(args.getQPNbLevels(), args.getQPPercentage(),
								cv::imread(args.getInputImages()[0], CV_LOAD_IMAGE_GRAYSCALE),
								false);
		}
		else
		{
			this->initFromHisto(args.getQPNbLevels(), args.getQPPercentage(),
								ReadImw(args.getInputImages()[0]), true);
		}
	}

	if (args.getQPFilenameOutput() != NULL)
		this->saveQuantificationProfile(args.getQPFilenameOutput(),
										args.getInputImages()[0]);

	std::cout << "Using: ";
	this->printQuantificationVector();
}

AlphaBuilder::AlphaBuilder(unsigned nbLevels, double percentage,
						   cv::Mat input, bool isSAR)
{
	this->initFromHisto(nbLevels, percentage, input, isSAR);
}

AlphaBuilder::AlphaBuilder(const char* file, bool isSAR)
{
	this->initFromFile(file, isSAR);
}

AlphaBuilder::~AlphaBuilder()
{
}

std::vector<unsigned>
AlphaBuilder::getQuantificationVector() const
{
	return QuantificationVector_;
}

void
AlphaBuilder::initFromHisto(unsigned nbLevels, double percentage,
							cv::Mat input, bool isSAR)
{
	unsigned type = input.type();

	//Find min and max.
	unsigned min = std::numeric_limits<unsigned>::max();
	unsigned max = 0;

	for (int i = 0; i < input.size().height; ++i)
		for (int j = 0; j < input.size().width; ++j)
		{
			unsigned value;
			if (type == CV_8U)
				value = input.at<unsigned char> (i, j);
			else
				value = input.at<unsigned short> (i, j);

			if (value < min)
				min = value;
			if (value > max)
				max = value;
		}

	unsigned nbBins = max - min;//Takes a lot of memory, bur more accurate and
	//easy to code.

	std::vector<unsigned> bins(nbBins, 0);

	for (int i = 0; i < input.size().height; ++i)
		for (int j = 0; j < input.size().width; ++j)
		{
			unsigned value;
			if (type == CV_8U)
				value = input.at<unsigned char> (i, j);
			else
				value = input.at<unsigned short> (i, j);

			bins[value - min] += 1;
		}

	unsigned nbPixel = input.size().height * input.size().width;
	double step = percentage * (double)nbPixel / (double)nbLevels;
	double cumulatedMinus1 = 0;
	unsigned cumulated = bins[0];
	unsigned nbChosen = 1;

	QuantificationVector_.push_back(min);
	for (unsigned i = 1; i < nbBins && nbChosen < nbLevels; ++i)
	{
		cumulated += bins[i];
		if (cumulated > (cumulatedMinus1 + step))
		{
			QuantificationVector_.push_back(i + min);
			cumulatedMinus1 = step * QuantificationVector_.size() - 1;
			++nbChosen;
		}
	}

	std::cout << "Quantification covers " << cumulated / (double)nbPixel * 100
			  << "% of the histogram." << std::endl;

	this->postProcess(isSAR);
}


void
AlphaBuilder::initFromFile(const char* file, bool isSAR)
{
	std::ifstream ifs(file, std::ifstream::in);
	if (!ifs.is_open())
	{
		std::cerr << "Could not open file " << file << std::endl;
		exit(1);
	}

	std::string line;
	unsigned linenb = 1;
	while (std::getline(ifs, line))
	{
		std::istringstream iss(line);

		if (iss.str().size() == 0 || iss.str().at(0) == '#')
			continue;//Ignoring lines starting with #.

		unsigned val1;
		iss >> val1;
		if (iss.eof())
			QuantificationVector_.push_back(val1);
		else
		{
			unsigned val2, val3;
			iss >> val2;

			if (iss.eof())
			{
				std::cerr << "Error while reading " << file << " at line "
						  << linenb << ":" << std::endl
						  << "You must give either one or three values."
						  << std::endl;
				exit(1);
			}

			iss >> val3;
			if (!iss.eof())
			{
				std::cerr << "Warning while reading " << file << " at line "
						  << linenb << ":" << std::endl
						  << "End of line ignored." << std::endl;
			}

			for (unsigned i = val1; i <= val2; i += val3)
				QuantificationVector_.push_back(i);
		}

		++linenb;
	}

	this->postProcess(isSAR);
}


void
AlphaBuilder::postProcess(bool isSAR)
{
	std::sort(QuantificationVector_.begin(), QuantificationVector_.end());
	if (isSAR && QuantificationVector_[0] < 1)
		QuantificationVector_[0] = 1;
}

void
AlphaBuilder::saveQuantificationProfile(const char* filename,
										const char* imagefilename) const
{
	std::ofstream ofs (filename, std::ifstream::out);
	if (!ofs.is_open())
	{
		std::cerr << "Error: Could not write in file " << filename << std::endl;
		exit(1);
	}

	if (imagefilename != NULL)
		ofs << "# Construced from " << imagefilename << std::endl;

	std::time_t temps;
	struct std::tm datetime;
	char  format[32];

	std::time(&temps);
	datetime = *localtime(&temps);
	std::strftime(format, 32, "%Y-%m-%d %H-%M", &datetime);

	ofs << "# Generated on: " << format << std::endl;

	for (std::vector<unsigned>::const_iterator it = QuantificationVector_.begin();
		 it != QuantificationVector_.end(); ++it)
	{
		ofs << *it << std::endl;
	}

}


void
AlphaBuilder::printQuantificationVector() const
{
	bool first = true;

	std::cout << "[";

	for (std::vector<unsigned>::const_iterator it = QuantificationVector_.begin();
		 it != QuantificationVector_.end(); ++it)
	{
		if (first)
			first = false;
		else
			std::cout << ", ";

		std::cout << *it;
	}

	std::cout << "]" << std::endl;
}
