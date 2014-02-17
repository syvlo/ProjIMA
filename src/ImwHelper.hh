#ifndef IMW_HELPER_HH_
# define IMW_HELPER_HH_

/**
 * \file ImwHelper.hh
 * \author Sylvain Lobry
 * \date 02/17/2014
 * \brief File taht defines the function to read imw files.
 */

#include <cv.h>

/**
 * \brief	Function that takes an imw file as input and return a cv::Mat
 *		storing it.
 * \param	DimFileName	*.dim file containing the dimension of the image.
 * \param	ImwFileName	*.imw file that contains the values (unsigned int 16).
 * \return	The cv::Mat (of type CV_16U)
 */
cv::Mat
ReadImw (const char* DimFileName,
	 const char* ImwFileName);

#endif /* !IMW_HELPER_HH_ */
