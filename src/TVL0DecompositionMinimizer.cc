#include "TVL0DecompositionMinimizer.hh"
#include "Args.hh"

#include <stdexcept>

TVL0DecompositionMinimizer::TVL0DecompositionMinimizer()
    : BetaBV_(DEFAULT_BBV), BetaS_(DEFAULT_BS), OutputBV_(NULL), OutputS_(NULL)
{
}


TVL0DecompositionMinimizer::TVL0DecompositionMinimizer(double BetaBV, double BetaS)
    : BetaBV_(BetaBV), BetaS_(BetaS), OutputBV_(NULL), OutputS_(NULL)
{   
}

TVL0DecompositionMinimizer::~TVL0DecompositionMinimizer()
{
    if (OutputBV_)
	delete OutputBV_;
    if (OutputS_)
	delete OutputS_;
}

bool
TVL0DecompositionMinimizer::compute(const cv::Mat& input)
{
    if (OutputBV_)
	delete OutputBV_;
    OutputBV_ = new cv::Mat(input.size(), CV_8UC1);

    if (OutputS_)
	delete OutputS_;
    OutputS_ = new cv::Mat(input.size(), CV_8UC1);

    return false;
}

void
TVL0DecompositionMinimizer::setBetaBV(const double BetaBV)
{
    BetaBV_ = BetaBV;
}

void
TVL0DecompositionMinimizer::setBetaS(const double BetaS)
{
    BetaS_ = BetaS;
}

const cv::Mat&
TVL0DecompositionMinimizer::getOutputBV() const
{
    if (!OutputBV_)
	throw std::logic_error("OutputBV has not been computed.");
    return *OutputBV_;
}

const cv::Mat&
TVL0DecompositionMinimizer::getOutputS() const
{
    if (!OutputS_)
	throw std::logic_error("OutputS has not been computed.");
    return *OutputS_;
}
