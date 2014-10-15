#ifndef ALPHA_BUILDER_HH_
# define ALPHA_BUILDER_HH_

/**
 * \file AlphaBuilder.hh
 * \author Sylvain Lobry
 * \date 08/11/2014
 * \brief File where alpha quantification vector is built.
 */

# include <vector>
# include <cv.h>
# include "Args.hh"

/**
 * \class AlphaBuilder
 * \brief Defines the quantification vector used in the program.
 */
class AlphaBuilder
{
public:
	/**
	 * \brief			Constructor.
	 * \details			Analyze args and construct vector accordingly.
	 * \param			args	The arguments.
	 */
	AlphaBuilder (const Args& args);

	/**
	 * \brief			Constructor from histogram.
	 * \details			Construct quantification vector from image histogram.
	 * \param			nbLevels		Number of desired quantification levels.
	 * \param			percentage		Part of the histogram to quantify
	 *									(between 0 and 1).
	 * \param			input			Input image.
	 * \param			isSAR			true if image is SAR image.
	 */
	AlphaBuilder (unsigned nbLevels, double percentage,
				  cv::Mat input, bool isSAR);

	/**
	 * \brief			Constructor from file.
	 * \details			Analyze file and construct quantification vector.
	 * \param			file	The file describing the quantification vector.
	 * \param			isSAR			true if image is SAR image.
	 */
	AlphaBuilder (const char* file, bool isSAR);

	/**
	 * \brief			Destructor.
	 * \details			Does nothing.
	 */
	~AlphaBuilder();

	/**
	 * \brief			Getter for the quantification vector.
	 * \return			The quantification vector.
	 */
	std::vector<unsigned>
	getQuantificationVector() const;

	/**
	 * \brief			Save quantification profile.
	 * \param			filename		The filename.
	 * \param			imagefilename	The image filename (useful for hist).
	 */
	void
	saveQuantificationProfile(const char* filename,
							  const char* imagefilename = NULL) const;

	/**
	 * \brief			Print quantification vector (debug purposes).
	 */
	void
	printQuantificationVector() const;

private:
	void initFromHisto(unsigned nbLevels, double percentage,
					   cv::Mat input, bool isSAR);

	void initFromFile(const char* file, bool isSAR);

	void postProcess(bool isSAR);

	std::vector<unsigned> QuantificationVector_;
};

#endif /* !ALPHA_BUILDER_HH_ */
