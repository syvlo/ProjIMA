#ifndef TV_L0_DECOMPOSITION_MINIMIZER_HXX_
# define TV_L0_DECOMPOSITION_MINIMIZER_HXX_

#include "TVL0DecompositionMinimizer.hh"
#include "Args.hh"

#include "maxflow/graph.h"

#include <stdexcept>
#include <iostream>

#define INFTY 1E20

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha, const std::vector<unsigned>& gamma)
    : BetaBV_(DEFAULT_BBV), BetaS_(DEFAULT_BS), Alpha_(alpha), Gamma_(gamma), OutputBV_(NULL), OutputS_(NULL)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha, const std::vector<unsigned>& gamma, double BetaBV, double BetaS)
    : BetaBV_(BetaBV), BetaS_(BetaS), Alpha_(alpha), Gamma_(gamma), OutputBV_(NULL), OutputS_(NULL)
{   
}

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::~TVL0DecompositionMinimizer()
{
    if (OutputBV_)
	delete OutputBV_;
    if (OutputS_)
	delete OutputS_;
}

template <typename DataTerm>
bool
TVL0DecompositionMinimizer<DataTerm>::compute(const cv::Mat& input)
{
    //Allocation of the output images.
    if (OutputBV_)
	delete OutputBV_;
    OutputBV_ = new cv::Mat(input.size(), CV_8U);

    if (OutputS_)
	delete OutputS_;
    OutputS_ = new cv::Mat(input.size(), CV_64F);

    //Useful information about the image.
    unsigned Height = input.size().height;
    unsigned Width = input.size().width;
    unsigned nbPix = Width * Height;
    unsigned nbAlpha = Alpha_.size();
    unsigned nbNodes = nbPix * nbAlpha;

    std::clog << "Graph construction:" << std::flush;

    //Allocation of the memory for the graph.
    Graph* g = new Graph(nbNodes, 2 * nbNodes);
    Graph::node_id* nodes = new Graph::node_id[nbNodes];

    //Creation of the nodes.
    for (unsigned i = 0; i < nbNodes; ++i)
      nodes[i] = g->add_node();

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
	    g->add_tweights(nodes[current], 0, INFTY);

	    //Every level (except last one) is linked to the one above it.
	    for (unsigned level = 1; level < nbAlpha; ++level)
		g->add_edge(nodes[current + (level - 1) * nbPix],
			   nodes[current + level * nbPix],
			   INFTY,
			   DataTerm::Compute(input.at<unsigned>(i, j), Alpha_[level]));//FIX ME

	    //Last one is linked to the source
	    g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
			   DataTerm::Compute(input.at<unsigned>(i, j), Alpha_[nbAlpha - 1]), 0);//FIX ME

	    //////////////////////////////////////////////
            // Links definition for regularization term //
            //////////////////////////////////////////////
	    //Eastern neighbour
	    if (j < Width - 1)
		for (unsigned level = 1; level <= nbAlpha; ++level)
		{
		    double beta = BetaBV_;
		    g->add_edge(nodes[current + (level - 1) * nbPix],
			       nodes[current + (level - 1) * nbPix + 1],
			       beta, beta);
		}

	    //Southern neighbour
	    if (i < Height - 1)
		for (unsigned level = 1; level <= nbAlpha; ++level)
		{
		  double beta = BetaBV_;
		    g->add_edge(nodes[current + (level - 1) * nbPix],
				nodes[current + (level - 1) * nbPix + Height],
				beta, beta);
		}
	}
    }

    std::clog << " done." << std::endl;
    std::clog << "Max flow computation:" << std::flush;
    //Computation of max flow.
    std::cout << g->maxflow();
    std::clog << " done." << std::endl;

    //Construction of the ouput image.
    for (unsigned i = 0; i < Height; ++i)
    {
    	unsigned offset = i * Width;
    	for (unsigned j = 0; j < Width; ++j)
    	{
    	    unsigned current = offset + j;
    	    OutputBV_->at<unsigned>(i, j) = Alpha_[1];

    	    for (unsigned level = nbAlpha; level >= 1; --level)
    	    {
    		if (g->what_segment(nodes[current + (level - 1) * nbPix])
    		    != Graph::SOURCE)
    		{
    		    OutputBV_->at<unsigned>(i, j) = Alpha_[level];
		    std::cout << i << " " << j << " : " << OutputBV_->at<unsigned>(i, j) << std::endl;
    		    break;
    		}
    	    }
    	}
    }

    delete g;
    delete[] nodes;

    return true;
}

template <typename DataTerm>
void
TVL0DecompositionMinimizer<DataTerm>::setBetaBV(const double BetaBV)
{
    BetaBV_ = BetaBV;
}

template <typename DataTerm>
void
TVL0DecompositionMinimizer<DataTerm>::setBetaS(const double BetaS)
{
    BetaS_ = BetaS;
}

template <typename DataTerm>
const cv::Mat&
TVL0DecompositionMinimizer<DataTerm>::getOutputBV() const
{
    if (!OutputBV_)
	throw std::logic_error("OutputBV has not been computed.");
    return *OutputBV_;
}

template <typename DataTerm>
const cv::Mat&
TVL0DecompositionMinimizer<DataTerm>::getOutputS() const
{
    if (!OutputS_)
	throw std::logic_error("OutputS has not been computed.");
    return *OutputS_;
}


#endif /* !TV_L0_DECOMPOSITION_MINIMIZER_HXX_ */
