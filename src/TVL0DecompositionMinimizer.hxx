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
	: BetaBV_(DEFAULT_BBV), BetaS_(DEFAULT_BS), Alpha_(alpha), Gamma_(gamma)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha, const std::vector<unsigned>& gamma, double BetaBV, double BetaS)
    : BetaBV_(BetaBV), BetaS_(BetaS), Alpha_(alpha), Gamma_(gamma)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::~TVL0DecompositionMinimizer()
{
}

template <typename DataTerm>
bool
TVL0DecompositionMinimizer<DataTerm>::compute(const cv::Mat& input)
{
    //Useful information about the image.
    unsigned Height = input.size().height;
    unsigned Width = input.size().width;
    unsigned nbPix = Width * Height;
    unsigned nbAlpha = Alpha_.size();
    unsigned nbNodes = nbPix * nbAlpha;

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
			{
				if (input.type() == CV_8U)
					g->add_edge(nodes[current + (level - 1) * nbPix],
								nodes[current + level * nbPix],
								INFTY,
								DataTerm::Compute(input.at<unsigned char>(i, j), Alpha_[level], Gamma_, BetaS_, BetaBV_));
				else
					g->add_edge(nodes[current + (level - 1) * nbPix],
								nodes[current + level * nbPix],
								INFTY,
								DataTerm::Compute(input.at<unsigned short>(i, j), Alpha_[level], Gamma_, BetaS_, BetaBV_));
			}

    	    //Last one is linked to the source
			if (input.type() == CV_8U)
				g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
								DataTerm::Compute(input.at<unsigned char>(i, j),
												  Alpha_[nbAlpha - 1], Gamma_, BetaS_, BetaBV_), 0);
			else
				g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
								DataTerm::Compute(input.at<unsigned short>(i, j),
												  Alpha_[nbAlpha - 1], Gamma_, BetaS_, BetaBV_), 0);

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
								beta,
								beta);
				}

    	    //Southern neighbour
    	    if (i < Height - 1)
				for (unsigned level = 1; level <= nbAlpha; ++level)
				{
					double beta = BetaBV_;
					g->add_edge(nodes[current + (level - 1) * nbPix],
								//Ou + Height comme dans TP ?
								nodes[current + (level - 1) * nbPix + Width],
								beta,
								beta);
				}
    	}
    }

    //Computation of max flow.
    g->maxflow();

    OutputBV_ = cv::Mat(Height, Width, input.type());
    OutputS_ = cv::Mat(Height, Width, input.type());
    OutputComplete_ = cv::Mat(Height, Width, input.type());

    //Construction of the ouput image.
    for (unsigned i = 0; i < Height; ++i)
    {
    	unsigned offset = i * Width;
    	for (unsigned j = 0; j < Width; ++j)
    	{
    	    unsigned current = offset + j;
			if (input.type() == CV_8U)
				OutputBV_.at<unsigned char>(i, j) = Alpha_[0];
			else
				OutputBV_.at<unsigned short>(i, j) = Alpha_[0];

    	    for (unsigned level = nbAlpha - 1; level >= 1; --level)
    	    {
				if (g->what_segment(nodes[current + (level) * nbPix])
					!= Graph::SOURCE)
				{
					if (input.type() == CV_8U)
					{
						OutputBV_.at<unsigned char>(i, j) = Alpha_[level];
						OutputS_.at<unsigned char>(i, j) =
							DataTerm::ComputeUs(input.at<unsigned char>(i, j),
												Alpha_[level], Gamma_, BetaS_, BetaBV_);
						OutputComplete_.at<unsigned char>(i, j) =
							OutputBV_.at<unsigned char>(i, j)
							+ OutputS_.at<unsigned char>(i, j);
					}
					else
					{
						OutputBV_.at<unsigned short>(i, j) = Alpha_[level];
						OutputS_.at<unsigned short>(i, j) =
							DataTerm::ComputeUs(input.at<unsigned short>(i, j),
												Alpha_[level], Gamma_, BetaS_, BetaBV_);
						OutputComplete_.at<unsigned short>(i, j) =
							OutputBV_.at<unsigned short>(i, j)
							+ OutputS_.at<unsigned short>(i, j);
					}
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
    return OutputBV_;
}

template <typename DataTerm>
const cv::Mat&
TVL0DecompositionMinimizer<DataTerm>::getOutputS() const
{
    return OutputS_;
}

template <typename DataTerm>
const cv::Mat&
TVL0DecompositionMinimizer<DataTerm>::getOutputComplete() const
{
    return OutputComplete_;
}


#endif /* !TV_L0_DECOMPOSITION_MINIMIZER_HXX_ */
