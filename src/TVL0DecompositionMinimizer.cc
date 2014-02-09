#include "TVL0DecompositionMinimizer.hh"
#include "Args.hh"

#include "maxflow/graph.h"

#include <stdexcept>

#define INFTY 1E20

TVL0DecompositionMinimizer::TVL0DecompositionMinimizer(const std::vector<double>& alpha, const std::vector<double>& gamma)
    : BetaBV_(DEFAULT_BBV), BetaS_(DEFAULT_BS), Alpha_(alpha), Gamma_(gamma), OutputBV_(NULL), OutputS_(NULL)
{
}


TVL0DecompositionMinimizer::TVL0DecompositionMinimizer(const std::vector<double>& alpha, const std::vector<double>& gamma, double BetaBV, double BetaS)
    : BetaBV_(BetaBV), BetaS_(BetaS), Alpha_(alpha), Gamma_(gamma), OutputBV_(NULL), OutputS_(NULL)
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
    //Allocation of the output images.
    if (OutputBV_)
	delete OutputBV_;
    OutputBV_ = new cv::Mat(input.size(), CV_8UC1);

    if (OutputS_)
	delete OutputS_;
    OutputS_ = new cv::Mat(input.size(), CV_8UC1);

    //Allocation of the memory for the graph.
    unsigned nbPix = input.size().width * input.size().height;
    unsigned nbAlpha = Alpha_.size();
    unsigned nbNodes = nbPix * nbAlpha;

    Graph* g = new Graph(nbNodes, 2 * nbNodes);
    Graph::node_id* nodes = new Graph::node_id[nbNodes];

    //Creation of the nodes.
    for (unsigned i = 0; i < nbNodes; ++i)
	nodes[i] = g->add_node();

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
