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
ComputeByParts<Minimizer>::compute(const cv::Mat& input)
{
    OutputBV_ = cv::Mat(input.size(), input.type());
    OutputS_ = cv::Mat(input.size(), input.type());
    OutputC_ = cv::Mat(input.size(), input.type());

	double iternum = 0;
	double itermax = (input.size().height / fillSize_) * (input.size().width / fillSize_);

    for (unsigned i = 0; i < input.size().height - fillSize_;
		 i += fillSize_)
    {
		for (unsigned j = 0; j < input.size().width - fillSize_;
			 j += fillSize_)
		{
			int endi = i + computeSize_;
			if (endi >= input.size().height)
				endi = input.size().height - 1;
			int endj = j + computeSize_;
			if (endj >= input.size().width)
				endj = input.size().width - 1;

			cv::Rect ComputeRegion(j, i, endj - j, endi - i);
			cv::Mat inputCropped = input(ComputeRegion);


			int startIFill = i + (computeSize_ - fillSize_) / 2;
			if (i == 0)
				startIFill = 0;
			int endIFill = i + (computeSize_ + fillSize_) / 2;
			if (endi == input.size().height - 1)
				endIFill = input.size().height - 1;

			int startJFill = j + (computeSize_ - fillSize_) / 2;
			if (j == 0)
				startJFill = 0;
			int endJFill = j + (computeSize_ + fillSize_) / 2;
			if (endj == input.size().width - 1)
				endJFill = input.size().width - 1;

			cv::Rect FillRegion(startJFill, startIFill, endJFill - startJFill,
								endIFill - startIFill);

			std::clog << "\r" << iternum++ / itermax * 100 << "%" << std::flush;

			if (minimizer_.compute(inputCropped))
			{
				cv::Mat OBV = minimizer_.getOutputBV();
				cv::Mat OS = minimizer_.getOutputS();
				cv::Mat OC = minimizer_.getOutputComplete();
				for (int tmp_i = startIFill; tmp_i < endIFill; ++tmp_i)
					for (int tmp_j = startJFill; tmp_j < endJFill; ++tmp_j)
					{
						if (input.type() == CV_8U)
						{
							OutputBV_.at<unsigned char>(tmp_i, tmp_j) = OBV.at<unsigned char>(tmp_i - i, tmp_j - j);
							OutputS_.at<unsigned char>(tmp_i, tmp_j) = OS.at<unsigned char>(tmp_i - i, tmp_j - j);
							OutputC_.at<unsigned char>(tmp_i, tmp_j) = OC.at<unsigned char>(tmp_i - i, tmp_j - j);
						}
						else
						{
							OutputBV_.at<unsigned short>(tmp_i, tmp_j) = OBV.at<unsigned short>(tmp_i - i, tmp_j - j);
							OutputS_.at<unsigned short>(tmp_i, tmp_j) = OS.at<unsigned short>(tmp_i - i, tmp_j - j);
							OutputC_.at<unsigned short>(tmp_i, tmp_j) = OutputBV_.at<unsigned short>(tmp_i, tmp_j) + OutputS_.at<unsigned short>(tmp_i, tmp_j);
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
const cv::Mat&
ComputeByParts<Minimizer>::getOutputBV() const
{
    return OutputBV_;
}

template <typename Minimizer>
const cv::Mat&
ComputeByParts<Minimizer>::getOutputS() const
{
    return OutputS_;
}
template <typename Minimizer>
const cv::Mat&
ComputeByParts<Minimizer>::getOutputC() const
{
    return OutputC_;
}

#endif /* !COMPUTE_BY_PARTS_HXX_ */
