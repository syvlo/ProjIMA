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
    OutputBV_ = new cv::Mat(input.size(), CV_64F);

    if (OutputS_)
	delete OutputS_;
    OutputS_ = new cv::Mat(input.size(), CV_64F);

    //Useful information about the image.
    unsigned Height = input.size().height;
    unsigned Width = input.size().width;
    unsigned nbPix = Width * Height;
    unsigned nbAlpha = Alpha_.size();
    unsigned nbNodes = nbPix * nbAlpha;

    //Allocation of the memory for the graph.
    Graph g (nbNodes, 2 * nbNodes);
    Graph::node_id* nodes = new Graph::node_id[nbNodes];

    //Creation of the nodes.
    for (unsigned i = 0; i < nbNodes; ++i)
	nodes[i] = g.add_node();

    //Links definition
    for (unsigned i = 0; i < Height; ++i)
    {
	unsigned offset = i * Width;
	for (unsigned j = 0; j < Width; ++j)
	{
	    /*For each site, we have one node for each possible value it can
	      have.*/
	    unsigned current = offset + j;

	    ///////////////////////////////////
            // Links defintion for data term //
            ///////////////////////////////////

	    //Smallest alpha is linked to the sink with infinite weight.
	    g.add_tweights(nodes[current], 0, INFTY);

	    //Every level (except last one) is linked to the one above it.
	    for (unsigned level = 1; level < nbAlpha; ++level)
		g.add_edge(nodes[current + (level - 1) * nbPix],
			   nodes[current + level * nbPix],
			   INFTY,
			   42);//FIX ME

	    //Last one is linked to the source
	    g.add_tweights(nodes[current + (nbAlpha - 1) * nbPix], 42, 0);//FIX ME

	    //////////////////////////////////////////////
            // Links definition for regularization term //
            //////////////////////////////////////////////
	    //Eastern neighbour
	    if (j < Width - 1)
		for (unsigned level = 1; level <= nbAlpha; ++level)
		{
		    double beta = BetaBV_ * (Alpha_[level] - Alpha_[level - 1]);
		    g.add_edge(nodes[current + (level - 1) * nbPix],
			       nodes[current + (level - 1) * nbPix + 1],
			       beta, beta);
		}

	    //Southern neighbour
	    if (i < Height - 1)
		for (unsigned level = 1; level <= nbAlpha; ++level)
		{
		    double beta = BetaBV_ * (Alpha_[level] - Alpha_[level - 1]);
		    g.add_edge(nodes[current + (level - 1) * nbPix],
			       nodes[current + (level - 1) * nbPix + Height],
			       beta, beta);
		}
	}
    }

    //Computation of max flow.
    g.maxflow();

    //Construction of the ouput image.
    for (unsigned i = 0; i < Height; ++i)
    {
	unsigned offset = i * Width;
	for (unsigned j = 0; j < Width; ++j)
	{
	    unsigned current = offset + j;
	    OutputBV_->at<double>(i, j) = 1;

	    for (unsigned level = nbAlpha; level >= 1; --level)
	    {
		if (g.what_segment(nodes[current + (level - 1) * nbPix])
		    != Graph::SOURCE)
		{
		    OutputBV_->at<double>(i, j) = Alpha_[level];
		    break;
		}
	    }
	}
    }

    delete nodes;

    return true;
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
