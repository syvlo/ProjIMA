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
	 * \param	input	The input image.
	 * \return	true if everything went OK, false otherwise.
	 */
    bool compute(const cv::Mat& input);

	/**
	 * \brief Return the Bounded variations image.
	 * \return	The image.
	 */
    const cv::Mat& getOutputBV() const;

	/**
	 * \brief Return the scaterers image.
	 * \return	The image.
	 */
    const cv::Mat& getOutputS() const;

	/**
	 * \brief Return the complete image.
	 * \return	The image.
	 */
    const cv::Mat& getOutputC() const;


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
	 * Bounded variations image
	 */
    cv::Mat OutputBV_;

    /**
	 * Scaterers image.
	 */
    cv::Mat OutputS_;

	/**
	 * Complete image.
	 */
    cv::Mat OutputC_;

	//Shoudl not use default constructor.
    ComputeByParts();
};

#include "ComputeByParts.hxx"

#endif /* !COMPUTE_BY_PARTS_HH_ */
