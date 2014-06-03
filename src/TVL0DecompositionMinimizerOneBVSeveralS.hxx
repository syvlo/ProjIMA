#ifndef TV_L0_DECOMPOSITION_MINIMIZER_ONE_BV_SEVERAL_S_HXX_
# define TV_L0_DECOMPOSITION_MINIMIZER_ONE_BV_SEVERAL_S_HXX_

#include "TVL0DecompositionMinimizerOneBVSeveralS.hh"
#include "Args.hh"

#include "maxflow/graph.h"

#include <stdexcept>
#include <iostream>

#define INFTY 1E20

template <typename DataTerm>
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::TVL0DecompositionMinimizerOneBVSeveralS(const std::vector<unsigned>& alpha)
	: BetaBV_(DEFAULT_BBV),
	  BetaS_(DEFAULT_BS),
	  Alpha_(alpha)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::TVL0DecompositionMinimizerOneBVSeveralS(const std::vector<unsigned>& alpha, double BetaBV, double BetaS)
    : BetaBV_(BetaBV),
	  BetaS_(BetaS),
	  Alpha_(alpha)
{
}

template <typename DataTerm>
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::~TVL0DecompositionMinimizerOneBVSeveralS()
{
}

template <typename DataTerm>
bool
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::compute(const std::vector<cv::Mat> inputs)
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
				unsigned sum = 0;
				for (unsigned i_img = 0; i_img < nbImages; ++ i_img)
				{
					if (type == CV_8U)
						sum += DataTerm::Compute(inputs[i_img].at<unsigned char>(i, j), Alpha_[level], BetaS_, BetaBV_);
					else
						sum += DataTerm::Compute(inputs[i_img].at<unsigned short>(i, j), Alpha_[level],  BetaS_, BetaBV_);
				}
				if (type == CV_8U)
					g->add_edge(nodes[current + (level - 1) * nbPix],
								nodes[current + level * nbPix], INFTY,
								(unsigned char)sum);
					else
						g->add_edge(nodes[current + (level - 1) * nbPix],
									nodes[current + level * nbPix], INFTY,
									(unsigned short)sum);
			}

			//Last one is linked to the source
			unsigned sum = 0;
			for (unsigned i_img = 0; i_img < nbImages; ++ i_img)
			{
				if (type == CV_8U)
					sum += DataTerm::Compute(inputs[i_img].at<unsigned char>(i, j),
											 Alpha_[nbAlpha - 1], BetaS_, BetaBV_);
				else
					sum += DataTerm::Compute(inputs[i_img].at<unsigned short>(i, j),
											 Alpha_[nbAlpha - 1], BetaS_, BetaBV_);
			}
			if (type == CV_8U)
				g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
								(unsigned char)sum, 0);
			else
				g->add_tweights(nodes[current + (nbAlpha - 1) * nbPix],
								(unsigned short)sum, 0);

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
		}
	}


    //Computation of max flow.
    g->maxflow();

	OutputsBV_.push_back(cv::Mat(Height, Width, type));
    //Construction of the output BV image.
	for (unsigned i = 0; i < Height; ++i)
	{
		unsigned offset = i * Width;
		for (unsigned j = 0; j < Width; ++j)
		{
			unsigned current = offset + j;
			if (type == CV_8U)
			{
				OutputsBV_[0].at<unsigned char>(i, j) = Alpha_[0];
			}
			else
			{
				OutputsBV_[0].at<unsigned short>(i, j) = Alpha_[0];
			}

			for (unsigned level = nbAlpha - 1; level >= 1; --level)
			{
				if (g->what_segment(nodes[current + (level) * nbPix])
					!= Graph::SOURCE)
				{
					if (type == CV_8U)
					{
						OutputsBV_[0].at<unsigned char>(i, j) = Alpha_[level];
					}
					else
					{
						OutputsBV_[0].at<unsigned short>(i, j) = Alpha_[level];
					}
					break;
				}
			}
		}
	}

	//Construction of the ouputsS and C images.
	for (unsigned i_img = 0; i_img < nbImages; ++ i_img)
	{
		OutputsS_.push_back(cv::Mat(Height, Width, type));
		OutputsComplete_.push_back(cv::Mat(Height, Width, type));

		for (unsigned i = 0; i < Height; ++i)
		{
			for (unsigned j = 0; j < Width; ++j)
			{
				if (type == CV_8U)
				{
					OutputsS_[i_img].at<unsigned char>(i, j) =
						DataTerm::ComputeUs(inputs[i_img].at<unsigned char>(i, j),
											OutputsBV_[0].at<unsigned char>(i, j),
											BetaS_, BetaBV_);
					OutputsComplete_[i_img].at<unsigned char>(i, j) =
						OutputsBV_[0].at<unsigned char>(i, j)
						+ OutputsS_[i_img].at<unsigned char>(i, j);
				}
				else
				{
					OutputsS_[i_img].at<unsigned short>(i, j) =
						DataTerm::ComputeUs(inputs[i_img].at<unsigned short>(i, j),
											OutputsBV_[0].at<unsigned short>(i, j),
											BetaS_, BetaBV_);
					OutputsComplete_[i_img].at<unsigned short>(i, j) =
						OutputsBV_[0].at<unsigned short>(i, j)
						+ OutputsS_[i_img].at<unsigned short>(i, j);
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
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::setBetaBV(const double BetaBV)
{
    BetaBV_ = BetaBV;
}

template <typename DataTerm>
void
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::setBetaS(const double BetaS)
{
    BetaS_ = BetaS;
}

template <typename DataTerm>
const std::vector<cv::Mat>
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::getOutputsBV() const
{
    return OutputsBV_;
}

template <typename DataTerm>
const std::vector<cv::Mat>
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::getOutputsS() const
{
    return OutputsS_;
}

template <typename DataTerm>
const std::vector<cv::Mat>
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::getOutputsComplete() const
{
    return OutputsComplete_;
}

template <typename DataTerm>
const double
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::getBetaS() const
{
	return BetaS_;
}

template <typename DataTerm>
const double
TVL0DecompositionMinimizerOneBVSeveralS<DataTerm>::getBetaBV() const
{
	return BetaBV_;
}

#endif /* !TV_L0_DECOMPOSITION_MINIMIZER_ONE_BV_SEVERAL_S_HXX_ */
