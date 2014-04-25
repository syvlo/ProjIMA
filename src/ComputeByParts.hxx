#ifndef COMPUTE_BY_PARTS_HXX_
# define COMPUTE_BY_PARTS_HXX_

# include "ComputeByParts.hh"

template <typename Minimizer>
ComputeByParts<Minimizer>::ComputeByParts(unsigned computeSize,
										  unsigned fillSize,
										  Minimizer& minimizer)
    : computeSize_(computeSize),
      fillSize_(fillSize),
      minimizer_(minimizer)
{}

template <typename Minimizer>
ComputeByParts<Minimizer>::~ComputeByParts()
{}

template <typename Minimizer>
bool
ComputeByParts<Minimizer>::compute(const std::vector<cv::Mat> inputs)
{
	if (inputs.size() == 0)
		return false;

	for (unsigned i = 0; i < inputs.size(); ++i)
	{
		OutputsBV_.push_back(cv::Mat(inputs[0].size(), inputs[0].type()));
		OutputsS_.push_back(cv::Mat(inputs[0].size(), inputs[0].type()));
		OutputsC_.push_back(cv::Mat(inputs[0].size(), inputs[0].type()));
	}

	double iternum = 0;
	double itermax = (inputs[0].size().height / fillSize_) * (inputs[0].size().width / fillSize_);

    for (unsigned i = 0; i < inputs[0].size().height - fillSize_;
		 i += fillSize_)
    {
		for (unsigned j = 0; j < inputs[0].size().width - fillSize_;
			 j += fillSize_)
		{
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

			std::clog << "\r" << iternum++ / itermax * 100 << "%" << std::flush;

			if (minimizer_.compute(inputsCropped))
			{
				std::vector<cv::Mat> OBV = minimizer_.getOutputsBV();
				std::vector<cv::Mat> OS = minimizer_.getOutputsS();
				std::vector<cv::Mat> OC = minimizer_.getOutputsComplete();
				for (unsigned img_i = 0; img_i < inputs.size(); ++img_i)
				{
					for (int tmp_i = startIFill; tmp_i < endIFill; ++tmp_i)
					{
						for (int tmp_j = startJFill; tmp_j < endJFill; ++tmp_j)
						{
							if (inputs[0].type() == CV_8U)
							{
								OutputsBV_[img_i].at<unsigned char>(tmp_i, tmp_j) = OBV[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
								OutputsS_[img_i].at<unsigned char>(tmp_i, tmp_j) = OS[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
								OutputsC_[img_i].at<unsigned char>(tmp_i, tmp_j) = OC[img_i].at<unsigned char>(tmp_i - i, tmp_j - j);
							}
							else
							{
								OutputsBV_[img_i].at<unsigned short>(tmp_i, tmp_j) = OBV[img_i].at<unsigned short>(tmp_i - i, tmp_j - j);
								OutputsS_[img_i].at<unsigned short>(tmp_i, tmp_j) = OS[img_i].at<unsigned short>(tmp_i - i, tmp_j - j);
								OutputsC_[img_i].at<unsigned short>(tmp_i, tmp_j) = OutputsBV_[img_i].at<unsigned short>(tmp_i, tmp_j) + OutputsS_[img_i].at<unsigned short>(tmp_i, tmp_j);
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


    return true;
}

template <typename Minimizer>
const std::vector<cv::Mat>
ComputeByParts<Minimizer>::getOutputsBV() const
{
    return OutputsBV_;
}

template <typename Minimizer>
const std::vector<cv::Mat>
ComputeByParts<Minimizer>::getOutputsS() const
{
    return OutputsS_;
}
template <typename Minimizer>
const std::vector<cv::Mat>
ComputeByParts<Minimizer>::getOutputsC() const
{
    return OutputsC_;
}

#endif /* !COMPUTE_BY_PARTS_HXX_ */
