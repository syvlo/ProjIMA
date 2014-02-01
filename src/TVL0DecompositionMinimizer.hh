#ifndef TV_L0_DECOMPOSITION_MINIMIZER_HH_
# define TV_L0_DECOMPOSITION_MINIMIZER_HH_

/**
 * \file Args.hh
 * \author Sylvain Lobry
 * \date 02/01/2014
 * \brief File that defines the minimizer.
 */


# include <cv.h>


/**
 * \class TVL0DecompositionMinimizer
 * \brief Class doing the computations using the mincut algorithm.
 */
class TVL0DecompositionMinimizer
{
public:
    /**
     * \brief		Default constructor.
     * \details		Will use default values for betas.
     */
    TVL0DecompositionMinimizer();

    /**
     * \brief		Constructor.
     * \param		BetaBV	Value for BetaBV.
     * \param		BetaS	Value for BetaS.
     */
    TVL0DecompositionMinimizer(double BetaBV, double BetaS);

    /**
     * \brief		Destructor.
     * \details		Will destroy the output images.
     */
    ~TVL0DecompositionMinimizer();

    /**
     * \brief		Method that construct the graph, launch the mincut and
     *			output the images.
     * \param		input	The image to be denoised.
     */
    bool compute(const cv::Mat& input);

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
     * \brief		Getter for the Bounded variations image.
     * \return		The image.
     */
    const cv::Mat& getOutputBV() const;

    /**
     * \brief		Getter for the Scatterers image.
     * \return		The image.
     */
    const cv::Mat& getOutputS() const;

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
     * Stores the last bounded variations image computed
     */
    cv::Mat* OutputBV_;

    /**
     * Stores the last scatterers image computed.
     */
    cv::Mat* OutputS_;
};

#endif /* !TV_L0_DECOMPOSITION_MINIMIZER_HH_ */

