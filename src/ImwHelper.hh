#ifndef IMW_HELPER_HH_
# define IMW_HELPER_HH_

/**
 * \file ImwHelper.hh
 * \author Sylvain Lobry
 * \date 02/17/2014
 * \brief File taht defines the function to work with imw files.
 */

#include <cv.h>

/**
 * \brief	Function that takes an imw file as input and return a cv::Mat
 *		storing it.
 * \param	DimFileName	*.dim file containing the dimension of the
				image.
 * \param	ImwFileName	*.imw file that contains the values (unsigned
				int 16).
 * \return	The cv::Mat (of type CV_16U)
 */
cv::Mat
ReadImw (const char* DimFileName,
	 const char* ImwFileName);

/**
 * \brief	Function that takes an imw file as input and return a cv::Mat
 *		storing it.
 * \param	FileName	File name of the input. So FileName.dim and
				FileNale.imw must exists.
 * \return	The cv::Mat (of type CV_16U)
 */
cv::Mat
ReadImw (const char* FileName);

/**
 * \brief	Write image to an imw file.
 * \param	image		The image to be written (CV_16U).
 * \param	DimFileName	*.dim file name
 * \param	ImwFileName	*.imw file name
 */
void
WriteImw (const cv::Mat image,
	  const char* DimFileName,
	  const char* ImwFileName);

/**
 * \brief	Function that convert a CV_16U cv::Mat to a CV_8U (so it can be
 *		displayed) keeping values between 0 and mu + nsigma * sigma.
 * \param	Input	The input matrix (of type CV_16U).
 * \return	The ouput matrix.
 */
cv::Mat
convertTo8U (const cv::Mat	Input,
	     double		nsigma = 3);

#endif /* !IMW_HELPER_HH_ */
