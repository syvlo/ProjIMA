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
# include <vector>
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
	/**
	 * \brief	Constructor.
	 * \param	computeSize		Size of one side of the window used for
	 *							computation.
	 * \param	fillSize		Size of one side of the window used for
	 *							filling.
	 * \param	minimizer		The minimizer used to compute one bloc.
	 */
    ComputeByParts(unsigned computeSize, unsigned fillSize, Minimizer& minimizer);

	/**
	 * \brief	Destructor
	 */
    ~ComputeByParts();

	/**
	 * \brief Method that split the image in blocks and find the optimum
	 *			on each one of them.
	 * \param	inputs	The input images.
	 * \return	true if everything went OK, false otherwise.
	 */
    bool compute(const std::vector<cv::Mat> inputs, bool oneBVSeveralS);

	/**
	 * \brief Return the Bounded variations images.
	 * \return	The images.
	 */
    const std::vector<cv::Mat> getOutputsBV() const;

	/**
	 * \brief Return the scaterers images.
	 * \return	The images.
	 */
    const std::vector<cv::Mat> getOutputsS() const;

	/**
	 * \brief Return the complete images.
	 * \return	The images.
	 */
    const std::vector<cv::Mat> getOutputsC() const;


private:
	/**
	 * Size of one side of the window used for computation.
	 */
    unsigned computeSize_;

	/**
	 * Size of one side of the window used for filling.
	 */
    unsigned fillSize_;

	/**
	 * Minimizer used to get the optimum on one block.
	 */
    Minimizer& minimizer_;

	/**
	 * Bounded variations images.
	 */
	std::vector<cv::Mat> OutputsBV_;

    /**
	 * Scaterers images.
	 */
	std::vector<cv::Mat> OutputsS_;

	/**
	 * Complete images.
	 */
	std::vector<cv::Mat> OutputsC_;

	//Shoudl not use default constructor.
    ComputeByParts();
};

#include "ComputeByParts.hxx"

#endif /* !COMPUTE_BY_PARTS_HH_ */
