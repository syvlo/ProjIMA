#ifndef COMPUTE_BY_PARTS_HH_
# define COMPUTE_BY_PARTS_HH_

/**
 * \file ComputeByParts.hh
 * \author Sylvain Lobry
 * \date 04/22/2014
 * \brief File where class handling bloc computation (leading to non-optimal
 *		solutions) is defined.
 */

# include <cv.h>

/**
 * Default value for the window size used for computation.
 */
# define DEFAULT_COMPUTE_SIZE (100)

/**
 * Default value for the window size used for filling.
 */
# define DEFAULT_FILL_SIZE (50)

/**
 * \class computeByParts
 * \brief Handles bloc by bloc computation.
 */
template <typename Minimizer>
class ComputeByParts
{
public:
    ComputeByParts(unsigned computeSize, unsigned fillSize, Minimizer& minimizer);

    ~ComputeByParts();

    bool compute(const cv::Mat& input);

    const cv::Mat& getOutputBV() const;
    const cv::Mat& getOutputS() const;
    const cv::Mat& getOutputC() const;
private:
    unsigned computeSize_;
    unsigned fillSize_;
    Minimizer& minimizer_;
    cv::Mat OutputBV_;
    cv::Mat OutputS_;
    cv::Mat OutputC_;

    ComputeByParts();
};

#include "ComputeByParts.hxx"

#endif /* !COMPUTE_BY_PARTS_HH_ */
