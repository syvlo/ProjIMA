#ifndef COMPUTE_BY_PARTS_LINEAR_HXX_
# define COMPUTE_BY_PARTS_LINEAR_HXX_

# include "ComputeByPartsLinear.hh"

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
ComputeByPartsLinear<Minimizer>::compute(const std::vector<cv::Mat> inputs)
{
	if (inputs.size() == 0)
		return false;

	cv::Mat numberOfOverlapped = cv::Mat::zeros(inputs[0].size(), CV_8U);

	for (unsigned i = 0; i < inputs.size(); ++i)
	{
		OutputsBV_.push_back(cv::Mat(inputs[0].size(), inputs[0].type()));
		OutputsS_.push_back(cv::Mat(inputs[0].size(), inputs[0].type()));
		OutputsC_.push_back(cv::Mat(inputs[0].size(), inputs[0].type()));
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
							numberOfOverlapped.at<unsigned char>(tmp_i, tmp_j) += 1;

							if (inputs[0].type() == CV_8U)
							{
								OutputsBV_[img_i].at<unsigned char>(tmp_i, tmp_j) = OutputsBV_[img_i].at<unsigned char>(tmp_i, tmp_j) + OBV[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
								OutputsS_[img_i].at<unsigned char>(tmp_i, tmp_j) = OutputsS_[img_i].at<unsigned char>(tmp_i, tmp_j) + OS[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
							}
							else
							{
								OutputsBV_[img_i].at<unsigned short>(tmp_i, tmp_j) = OutputsBV_[img_i].at<unsigned short>(tmp_i, tmp_j) + OBV[img_i].at<unsigned short>(tmp_i - i, tmp_j - j);
								OutputsS_[img_i].at<unsigned short>(tmp_i, tmp_j) = OutputsS_[img_i].at<unsigned short>(tmp_i, tmp_j) + OS[img_i].at<unsigned short>(tmp_i - i, tmp_j - j);
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

	for (unsigned i_img = 0; i_img < inputs.size(); ++i_img)
	{
		for (int i = 0; i < inputs[0].size().height; ++i)
		{
			std::cout << std::endl;
			for (int j = 0; j < inputs[0].size().width; ++j)
			{
				std::cout << (unsigned int)numberOfOverlapped.at<unsigned char>(i, j) << " ";
				if (numberOfOverlapped.at<unsigned char>(i, j) == 0)
				{
					std::cerr << "numberOfOverlapped.at(" << i << ", " << j << ") = 0" << std::endl;
					continue;
				}
				if (inputs[0].type() == CV_8U)
				{
					OutputsBV_[i_img].at<unsigned char>(i, j) /= numberOfOverlapped.at<unsigned char>(i, j);
					OutputsS_[i_img].at<unsigned char>(i, j) /= numberOfOverlapped.at<unsigned char>(i, j);
					OutputsC_[i_img].at<unsigned char>(i, j) = OutputsBV_[i_img].at<unsigned char>(i, j) + OutputsS_[i_img].at<unsigned char>(i, j);
				}
				else
				{
					OutputsBV_[i_img].at<unsigned short>(i, j) /= numberOfOverlapped.at<unsigned char>(i, j);
					OutputsS_[i_img].at<unsigned short>(i, j) /= numberOfOverlapped.at<unsigned char>(i, j);
					OutputsC_[i_img].at<unsigned short>(i, j) = OutputsBV_[i_img].at<unsigned short>(i, j) + OutputsS_[i_img].at<unsigned short>(i, j);
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
