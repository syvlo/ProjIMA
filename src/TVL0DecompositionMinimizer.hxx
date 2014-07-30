#ifndef TV_L0_DECOMPOSITION_MINIMIZER_HXX_
# define TV_L0_DECOMPOSITION_MINIMIZER_HXX_

#include "TVL0DecompositionMinimizer.hh"
#include "Args.hh"

#include "maxflow/graph.h"

#include <stdexcept>
#include <iostream>

#define INFTY 1E20

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha)
	: BetaBV_(DEFAULT_BBV),
	  BetaS_(DEFAULT_BS),
	  Alpha_(alpha)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha, double BetaBV, double BetaS)
    : BetaBV_(BetaBV),
	  BetaS_(BetaS),
	  Alpha_(alpha)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizer<DataTerm>::~TVL0DecompositionMinimizer()
{
}

template <typename DataTerm>
bool
TVL0DecompositionMinimizer<DataTerm>::compute(const std::vector<cv::Mat> inputs)
{

    //Useful information about the image.
	unsigned nbImages = inputs.size();

	//Check if at least one image is given as input.
	if (nbImages == 0)
		return false;

    unsigned Height = inputs[0].size().height;
    unsigned Width = inputs[0].size().width;
    unsigned nbPix = Width * Height;
    unsigned nbAlpha = Alpha_.size();
    unsigned nbNodes = nbPix * nbAlpha;
	unsigned type = inputs[0].type();

    //Allocation of the memory for the graph.
    Graph* g = new Graph(nbNodes * nbImages, 2 * nbNodes * nbImages);
    Graph::node_id* nodes = new Graph::node_id[nbNodes * nbImages];

    //Creation of the nodes.
	for (unsigned i_img = 0; i_img < nbImages; ++ i_img)
		for (unsigned i = 0; i < nbNodes; ++i)
			nodes[i_img * nbNodes + i] = g->add_node();

    //Links definition
	for (unsigned i_img = 0; i_img < nbImages; ++ i_img)
	{
		unsigned offset_img = i_img * nbNodes;
		for (unsigned i = 0; i < Height; ++i)
		{
			unsigned offset = i * Width;
			for (unsigned j = 0; j < Width; ++j)
			{
				/*For each site, we have one node for each possible value it can
				  have.*/
				unsigned current = offset_img + offset + j;

				///////////////////////////////////
				// Links defintion for data term //
				///////////////////////////////////

				//Smallest alpha is linked to the sink with infinite weight.
				g->add_tweights(nodes[current], 0, INFTY);

				//Every level (except last one) is linked to the one above it.
				for (unsigned level = 1; level < nbAlpha; ++level)
				{
					if (type == CV_8U)
						g->add_edge(nodes[current + (level - 1) * nbPix],
									nodes[current + level * nbPix],
									INFTY,
									DataTerm::Compute(inputs[i_img].at<unsigned char>(i, j), Alpha_[level], BetaS_, BetaBV_));
					else
						g->add_edge(nodes[current + (level - 1) * nbPix],
									nodes[current + level * nbPix],
									INFTY,
									DataTerm::Compute(inputs[i_img].at<unsigned short>(i, j), Alpha_[level],  BetaS_, BetaBV_));
				}

				//Last one is linked to the source
				if (type == CV_8U)
					g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
									DataTerm::Compute(inputs[i_img].at<unsigned char>(i, j),
													  Alpha_[nbAlpha - 1], BetaS_, BetaBV_), 0);
				else
					g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
									DataTerm::Compute(inputs[i_img].at<unsigned short>(i, j),
													  Alpha_[nbAlpha - 1], BetaS_, BetaBV_), 0);

				//////////////////////////////////////////////
				// Links definition for regularization term //
				//////////////////////////////////////////////
				double beta = BetaBV_;
				//Eastern neighbour
				if (j < Width - 1)
					for (unsigned level = 1; level <= nbAlpha; ++level)
					{
						g->add_edge(nodes[current + (level - 1) * nbPix],
									nodes[current + (level - 1) * nbPix + 1],
									beta,
									beta);
					}

				//Southern neighbour
				if (i < Height - 1)
					for (unsigned level = 1; level <= nbAlpha; ++level)
					{
						g->add_edge(nodes[current + (level - 1) * nbPix],
									//Ou + Height comme dans TP ?
									nodes[current + (level - 1) * nbPix + Width],
									beta,
									beta);
					}

				//Backward (previous image) neighbour.
				if (i_img > 0)
					for (unsigned level = 1; level <= nbAlpha; ++level)
					{
						g->add_edge(nodes[current + (level -1) * nbPix],
									nodes[current + (level - 1) * nbPix  - offset_img],
									beta,
									beta);
					}
			}
		}
	}

	std::clog << "Max flow computation:" << std::flush;
    //Computation of max flow.
    double MaxFlowValue = g->maxflow();
	std::clog << " done." << std::endl
		      << "Value = " << MaxFlowValue << std::endl;


    //Construction of the output images.
	for (unsigned i_img = 0; i_img < nbImages; ++ i_img)
	{
		OutputsBV_.push_back(cv::Mat(Height, Width, type));
		OutputsS_.push_back(cv::Mat(Height, Width, type));
		OutputsComplete_.push_back(cv::Mat(Height, Width, type));

		unsigned offset_img = i_img * nbNodes;

		for (unsigned i = 0; i < Height; ++i)
		{
			unsigned offset = i * Width;
			for (unsigned j = 0; j < Width; ++j)
			{
				unsigned current = offset_img + offset + j;
				if (type == CV_8U)
				{
					OutputsBV_[i_img].at<unsigned char>(i, j) = Alpha_[0];
					OutputsS_[i_img].at<unsigned char>(i, j) =
						DataTerm::ComputeUs(inputs[i_img].at<unsigned char>(i, j),
											Alpha_[0], BetaS_, BetaBV_);
					OutputsComplete_[i_img].at<unsigned char>(i, j) =
						OutputsBV_[i_img].at<unsigned char>(i, j)
						+ OutputsS_[i_img].at<unsigned char>(i, j);
				}
				else
				{
					OutputsBV_[i_img].at<unsigned short>(i, j) = Alpha_[0];
					OutputsS_[i_img].at<unsigned short>(i, j) =
						DataTerm::ComputeUs(inputs[i_img].at<unsigned short>(i, j),
											Alpha_[0], BetaS_, BetaBV_);
					OutputsComplete_[i_img].at<unsigned short>(i, j) =
						OutputsBV_[i_img].at<unsigned short>(i, j)
						+ OutputsS_[i_img].at<unsigned short>(i, j);
				}

				for (unsigned level = nbAlpha - 1; level >= 1; --level)
				{
					if (g->what_segment(nodes[current + (level) * nbPix])
						!= Graph::SOURCE)
					{
						if (type == CV_8U)
						{
							OutputsBV_[i_img].at<unsigned char>(i, j) = Alpha_[level + 1];
							OutputsS_[i_img].at<unsigned char>(i, j) =
								DataTerm::ComputeUs(inputs[i_img].at<unsigned char>(i, j),
													Alpha_[level + 1], BetaS_, BetaBV_);
							OutputsComplete_[i_img].at<unsigned char>(i, j) =
								OutputsBV_[i_img].at<unsigned char>(i, j)
								+ OutputsS_[i_img].at<unsigned char>(i, j);
						}
						else
						{
							OutputsBV_[i_img].at<unsigned short>(i, j) = Alpha_[level];
							OutputsS_[i_img].at<unsigned short>(i, j) =
								DataTerm::ComputeUs(inputs[i_img].at<unsigned short>(i, j),
													Alpha_[level], BetaS_, BetaBV_);
							OutputsComplete_[i_img].at<unsigned short>(i, j) =
								OutputsBV_[i_img].at<unsigned short>(i, j)
								+ OutputsS_[i_img].at<unsigned short>(i, j);
						}
						break;
					}
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
const std::vector<cv::Mat>
TVL0DecompositionMinimizer<DataTerm>::getOutputsBV() const
{
    return OutputsBV_;
}

template <typename DataTerm>
const std::vector<cv::Mat>
TVL0DecompositionMinimizer<DataTerm>::getOutputsS() const
{
    return OutputsS_;
}

template <typename DataTerm>
const std::vector<cv::Mat>
TVL0DecompositionMinimizer<DataTerm>::getOutputsComplete() const
{
    return OutputsComplete_;
}

template <typename DataTerm>
const double
TVL0DecompositionMinimizer<DataTerm>::getBetaS() const
{
	return BetaS_;
}

template <typename DataTerm>
const double
TVL0DecompositionMinimizer<DataTerm>::getBetaBV() const
{
	return BetaBV_;
}

#endif /* !TV_L0_DECOMPOSITION_MINIMIZER_HXX_ */
