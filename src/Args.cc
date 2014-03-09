#include "Args.hh"
#include <cstdlib>
#include <cstring>

Args::Args (int argc, char* argv[])
{
    //Default Values
    InputImage_ = NULL;
    OutputImageBV_ = NULL;
    OutputImageS_ = NULL;
    OutputImageComplete_ = NULL;
    BetaS_ = DEFAULT_BBV;
    BetaBV_ = DEFAULT_BS;
    WindowMode_ = DEFAULT_WINDOW_MODE;
    RadarMode_ = DEFAULT_RADAR_MODE;
    Help_ = false;

    for (int i = 1; i < argc; ++i)
    {
	if (!strcmp("-BBV", argv[i]) || !strcmp("--BetaBV", argv[i]))
	    BetaBV_ = atof(argv[++i]);
	else if (!strcmp("-BS", argv[i]) || !strcmp("--BetaS", argv[i]))
	    BetaS_ = atof(argv[++i]);
	else if (!strcmp("-i", argv[i]) || !strcmp("--InputImage", argv[i]))
	{
	    InputImage_ = new char[strlen(argv[++i])];
	    std::strcpy(InputImage_, argv[i]);
	}
	else if (!strcmp("-oBV", argv[i]) || !strcmp("--OutputImageBV", argv[i]))
	{
	    OutputImageBV_ = new char[strlen(argv[++i])];
	    std::strcpy(OutputImageBV_, argv[i]);
	}
	else if (!strcmp("-oS", argv[i]) || !strcmp("--OutputImageS", argv[i]))
	{
	    OutputImageS_ = new char[strlen(argv[++i])];
	    std::strcpy(OutputImageS_, argv[i]);
	}
	else if (!strcmp("-oC", argv[i]) || !strcmp("--OutputImageComplete", argv[i]))
	{
	    OutputImageComplete_ = new char[strlen(argv[++i])];
	    std::strcpy(OutputImageComplete_, argv[i]);
	}
	else if (!strcmp("-w", argv[i]) || !strcmp("--Window", argv[i]))
	    WindowMode_ = true;
	else if (!strcmp("-r", argv[i]) || !strcmp("--Radar", argv[i]))
	    RadarMode_ = true;
	else if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i]))
	{
	    printHelp();
	    Help_ = true;
	}
	else
	    std::cerr << "Warning: unknown parameter: " << argv[i] << std::endl;
    }
}

Args::~Args()
{
    if (InputImage_)
	delete[] InputImage_;
    if (OutputImageBV_)
	delete[] OutputImageBV_;
    if (OutputImageS_)
	delete[] OutputImageS_;
    if (OutputImageComplete_)
	delete[] OutputImageComplete_;
}

bool
Args::checkConsistency() const
{
    if (!WindowMode_ && (!InputImage_ || !OutputImageBV_ || !OutputImageS_))
	return false;
    return true;
}

void
Args::printHelp() const
{
    std::cout << "Available parameters:" << std::endl
	      << "* -BBV/--BetaBV <value>, BetaBV;" << std::endl
	      << "* -BS/--BetaS <value>, BetaS;" << std::endl
	      << "* -i/--InputImage <value>, image to be denoised;" << std::endl
	      << "* -oBV/--OutputImageBV <value>, image to store the Bounded Variations;" << std::endl
	      << "* -oS/--OutputImageS <value>, image to store the scatterers;" << std::endl
	      << "* -oC/--OutputImageComplete <value>, image to store the denoised image;" << std::endl
	      << "* -w/--Window, switch to window mode;" << std::endl
	      << "* -r/--Radar, switch to radar mode (so no need to pu exts at the end of files names);" << std::endl
	      << "* -h/--help, print this message;" << std::endl;
}

bool
Args::getHelp() const
{
    return Help_;
}

double
Args::getBetaS() const
{
    return BetaS_;
}

double
Args::getBetaBV() const
{
    return BetaBV_;
}

const char*
Args::getInputImage() const
{
    return InputImage_;
}

const char*
Args::getOutputImageBV() const
{
    return OutputImageBV_;
}

const char*
Args::getOutputImageS() const
{
    return OutputImageS_;
}

const char*
Args::getOutputImageComplete() const
{
    return OutputImageComplete_;
}

bool
Args::getWindowMode() const
{
    return WindowMode_;
}

bool
Args::getRadarMode() const
{
    return RadarMode_;
}

std::ostream&
operator<< (std::ostream& stream, const Args& args)
{
    stream << "BetaBV = " << args.BetaBV_ << "," << std::endl
	   << "BetaS = " << args.BetaS_ << "," << std::endl;
    if (args.InputImage_)
	stream << "InputImage = " << args.InputImage_ << "," << std::endl;
    if (args.OutputImageBV_)
	stream << "OutputImageBV = " << args.OutputImageBV_ << "," << std::endl;
    if (args.OutputImageS_)
	stream << "OutputImageS = " << args.OutputImageS_ << "," << std::endl;
    if (args.OutputImageComplete_)
	stream << "OutputImageComplete = " << args.OutputImageComplete_ << "," << std::endl;
    stream << "Window Mode = " << args.WindowMode_ << std::endl;
    stream << "Radar Mode = " << args.RadarMode_;
    return stream;
}


