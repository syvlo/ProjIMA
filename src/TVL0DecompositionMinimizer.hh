#ifndef TV_L0_DECOMPOSITION_MINIMIZER_HH_
# define TV_L0_DECOMPOSITION_MINIMIZER_HH_

/**
 * \file TVL0DecompositionMinimizer.hh
 * \author Sylvain Lobry
 * \date 02/01/2014
 * \brief File that defines the minimizer.
 */


# include <cv.h>
# include <vector>

/**
 * \class TVL0DecompositionMinimizer
 * \brief Class doing the computations using the mincut algorithm.
 */
template <typename DataTerm>
class TVL0DecompositionMinimizer
{
public:
    /**
     * \brief		Default constructor.
     * \details		Will use default values for betas.
     * \param		alpha Look-up table for the values of alpha_i.
     * \param		gamma Look-up table for the values of gamma_i.
     */
    TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha, const std::vector<unsigned>& gamma);

    /**
     * \brief		Constructor.
     * \param		alpha Look-up table for the values of alpha_i.
     * \param		gamma Look-up table for the values of gamma_i.
     * \param		BetaBV	Value for BetaBV.
     * \param		BetaS	Value for BetaS.
     */
    TVL0DecompositionMinimizer(const std::vector<unsigned>& alpha, const std::vector<unsigned>& gamma, double BetaBV, double BetaS);

    /**
     * \brief		Destructor.
     * \details		Will destroy the output images.
     */
    ~TVL0DecompositionMinimizer();

    /**
     * \brief		Method that construct the graph, launch the mincut and
     *			output the images.
     * \param		inputs	The images to be denoised (could be just one image
	 *					in the vector).
     */
    bool compute(const std::vector<cv::Mat> inputs);

    /**
     * \brief		Setter for BetaBV.
     * \param		BetaBV	New value for BetaBV.
     */
    void setBetaBV(const double BetaBV);

    /**
     * \brief		Setter for BetaS.
     * \param		BetaS	New value for BetaS.
     */
    void setBetaS(const double BetaS);

    /**
     * \brief		Getter for the Bounded variations images.
     * \return		The images.
     */
    const std::vector<cv::Mat> getOutputsBV() const;

    /**
     * \brief		Getter for the Scatterers images.
     * \return		The images.
     */
    const std::vector<cv::Mat> getOutputsS() const;

    /**
     * \brief		Getter for the complete images.
     * \return		The images.
     */
    const std::vector<cv::Mat> getOutputsComplete() const;

	/**
	 * \brief		Getter for gamma.
	 * \return		Gamma_.
	 */
	const std::vector<unsigned>& getGamma() const;

	/**
	 * \brief		Getter for BetaS.
	 * \return	    BetaS.
	 */
	const double getBetaS() const;

	/**
	 * \brief getter for BetaBV.
	 * \return BetaBV.
	 */
	const double getBetaBV() const;

	/** Typedef to use the dataterm in an other class. */
	typedef DataTerm dataTerm;

private:
    /**
     * Beta parameter for the bounded variation component.
     */
    double BetaBV_;

    /**
     * Beta parameter for the sparse component.
     */
    double BetaS_;

    /**
     * look-up table for alpha_i values, i.e. possible values for BV image.
     */
    const std::vector<unsigned> Alpha_;

    /**
     * look-up table for gamma_i values, i.e. possible values for S image.
     */
    const std::vector<unsigned> Gamma_;

    /**
     * Stores the last bounded variations images computed
     */
	std::vector<cv::Mat> OutputsBV_;

    /**
     * Stores the last scatterers images computed.
     */
	std::vector<cv::Mat> OutputsS_;

    /**
     * Stores the last complete images computed.
     */
	std::vector<cv::Mat> OutputsComplete_;

    //Look-up tables must be provided.
    TVL0DecompositionMinimizer();
};

#include "TVL0DecompositionMinimizer.hxx"

#endif /* !TV_L0_DECOMPOSITION_MINIMIZER_HH_ */

