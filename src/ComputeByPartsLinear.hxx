#ifndef COMPUTE_BY_PARTS_LINEAR_HXX_
# define COMPUTE_BY_PARTS_LINEAR_HXX_

# include "ComputeByPartsLinear.hh"
# include <cmath>

template <typename Minimizer>
ComputeByPartsLinear<Minimizer>::ComputeByPartsLinear(unsigned computeSize,
													  unsigned fillSize,
													  unsigned shiftSize,
													  Minimizer& minimizer)
    : computeSize_(computeSize),
      fillSize_(fillSize),
	  shiftSize_(shiftSize),
      minimizer_(minimizer)
{}

template <typename Minimizer>
ComputeByPartsLinear<Minimizer>::~ComputeByPartsLinear()
{}

template <typename Minimizer>
bool
ComputeByPartsLinear<Minimizer>::compute(const std::vector<cv::Mat> inputs, const double sigma)
{
	if (inputs.size() == 0)
		return false;

	cv::Mat weights = cv::Mat::zeros(inputs[0].size(), CV_64F);
	unsigned kernelSize = (fillSize_ % 2 == 1 ? fillSize_ : fillSize_ + 1);
	cv::Mat Tmpkernel = cv::getGaussianKernel(kernelSize, sigma);
	cv::Mat kernel = Tmpkernel * Tmpkernel.t();
	kernel *= 100 / kernel.at<double>(kernelSize / 2 - 1, kernelSize / 2 - 1);

	std::vector<cv::Mat> TmpOutputsBV;
	std::vector<cv::Mat> TmpOutputsS;

	for (unsigned i = 0; i < inputs.size(); ++i)
	{
		if (inputs[0].type() == CV_8U)
		{
			OutputsBV_.push_back(cv::Mat(inputs[0].size(), CV_8U));
			OutputsS_.push_back(cv::Mat(inputs[0].size(), CV_8U));
			OutputsC_.push_back(cv::Mat(inputs[0].size(), CV_8U));
		}
		else
		{
			//We will do the mean with intesity values for SAR (rather than
			//amplitude), so we need to have a much bigger type than 16bits.

			TmpOutputsBV.push_back(cv::Mat(inputs[0].size(), CV_64F));
			TmpOutputsS.push_back(cv::Mat(inputs[0].size(), CV_64F));

			OutputsBV_.push_back(cv::Mat(inputs[0].size(), CV_16U));
			OutputsS_.push_back(cv::Mat(inputs[0].size(), CV_16U));
			OutputsC_.push_back(cv::Mat(inputs[0].size(), CV_16U));
		}
	}

    for (unsigned i = 0; i < inputs[0].size().height - fillSize_;
		 i += shiftSize_)
    {
		for (unsigned j = 0; j < inputs[0].size().width - fillSize_;
			 j += shiftSize_)
		{
			std::cout << i << " " << j << std::endl;

			int endi = i + computeSize_;
			if (endi >= inputs[0].size().height)
				endi = inputs[0].size().height - 1;
			int endj = j + computeSize_;
			if (endj >= inputs[0].size().width)
				endj = inputs[0].size().width - 1;

			cv::Rect ComputeRegion(j, i, endj - j, endi - i);
			std::vector<cv::Mat> inputsCropped;
			for (unsigned i_img = 0; i_img < inputs.size(); ++i_img)
			{
				inputsCropped.push_back(inputs[i_img](ComputeRegion));
			}


			int startIFill = i + (computeSize_ - fillSize_) / 2;
			if (i == 0)
				startIFill = 0;
			int endIFill = i + (computeSize_ + fillSize_) / 2;
			if (endi == inputs[0].size().height - 1)
				endIFill = inputs[0].size().height - 1;

			int startJFill = j + (computeSize_ - fillSize_) / 2;
			if (j == 0)
				startJFill = 0;
			int endJFill = j + (computeSize_ + fillSize_) / 2;
			if (endj == inputs[0].size().width - 1)
				endJFill = inputs[0].size().width - 1;

			cv::Rect FillRegion(startJFill, startIFill, endJFill - startJFill,
								endIFill - startIFill);

			if (minimizer_.compute(inputsCropped))
			{
				std::vector<cv::Mat> OBV = minimizer_.getOutputsBV();
				std::vector<cv::Mat> OS = minimizer_.getOutputsS();
				std::vector<cv::Mat> OC = minimizer_.getOutputsComplete();
				for (unsigned img_i = 0; img_i < inputs.size(); ++img_i)
				{
					for (int tmp_i = startIFill; tmp_i <= endIFill; ++tmp_i)
					{
						for (int tmp_j = startJFill; tmp_j <= endJFill; ++tmp_j)
						{
							double weight = kernel.at<double>(tmp_i - startIFill, tmp_j - startJFill);
							weights.at<double>(tmp_i, tmp_j) += weight;

							if (inputs[0].type() == CV_8U)
							{
								OutputsBV_[img_i].at<unsigned char>(tmp_i, tmp_j) = OutputsBV_[img_i].at<unsigned char>(tmp_i, tmp_j) + weight * OBV[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
								OutputsS_[img_i].at<unsigned char>(tmp_i, tmp_j) = OutputsS_[img_i].at<unsigned char>(tmp_i, tmp_j) + weight * OS[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
							}
							else
							{
								TmpOutputsBV[img_i].at<double>(tmp_i, tmp_j) = TmpOutputsBV[img_i].at<double>(tmp_i, tmp_j) + weight * pow(OBV[img_i].at<unsigned short>(tmp_i - i, tmp_j - j), 2);
								if (tmp_i == 80 && tmp_j == 80)
								{
									std::cout << "Poids = " << weight << std::endl;
									std::cout << "Value = " << TmpOutputsBV[img_i].at<double>(tmp_i, tmp_j) << std::endl
											  << "OBV = " << OBV[img_i].at<unsigned short>(tmp_i - i, tmp_j - j) << std::endl
											  << "Poids fois carré = " << weight * pow(OBV[img_i].at<unsigned short>(tmp_i - i, tmp_j - j), 2) << std::endl;
								}

								TmpOutputsS[img_i].at<double>(tmp_i, tmp_j) = TmpOutputsS[img_i].at<double>(tmp_i, tmp_j) + weight * pow(OS[img_i].at<unsigned short>(tmp_i - i, tmp_j - j), 2);
							}
						}
					}
				}
			}
			else
			{
				return false;
			}
		}
    }

	std::cout << "Total = " << weights.at<double>(80, 80) << std::endl;
	std::cout << "Before: " << TmpOutputsBV[0].at<double>(80, 80) << std::endl;

	for (unsigned i_img = 0; i_img < inputs.size(); ++i_img)
	{
		for (int i = 0; i < inputs[0].size().height; ++i)
		{
			for (int j = 0; j < inputs[0].size().width; ++j)
			{
				if (inputs[0].type() == CV_8U)
				{
					OutputsBV_[i_img].at<unsigned char>(i, j) /= weights.at<double>(i, j);
					OutputsS_[i_img].at<unsigned char>(i, j) /= weights.at<double>(i, j);
					OutputsC_[i_img].at<unsigned char>(i, j) = OutputsBV_[i_img].at<unsigned char>(i, j) + OutputsS_[i_img].at<unsigned char>(i, j);
				}
				else
				{
					OutputsBV_[i_img].at<unsigned short>(i, j) = sqrt(TmpOutputsBV[i_img].at<double>(i, j)
																	  / weights.at<double>(i, j));
					OutputsS_[i_img].at<unsigned short>(i, j) = sqrt(TmpOutputsS[i_img].at<double>(i, j)
																	 / weights.at<double>(i, j));
					OutputsC_[i_img].at<unsigned short>(i, j) = OutputsBV_[i_img].at<unsigned short>(i, j)
						+ OutputsS_[i_img].at<unsigned short>(i, j);
				}
			}
		}
	}


    return true;
}

template <typename Minimizer>
const std::vector<cv::Mat>
ComputeByPartsLinear<Minimizer>::getOutputsBV() const
{
    return OutputsBV_;
}

template <typename Minimizer>
const std::vector<cv::Mat>
ComputeByPartsLinear<Minimizer>::getOutputsS() const
{
    return OutputsS_;
}
template <typename Minimizer>
const std::vector<cv::Mat>
ComputeByPartsLinear<Minimizer>::getOutputsC() const
{
    return OutputsC_;
}

#endif /* !COMPUTE_BY_PARTS_LINEAR_HXX_ */
