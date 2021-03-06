#ifndef QUADRATIC_DATA_TERM_HH_
# define QUADRATIC_DATA_TERM_HH_

/**
 * \file QuadraticDataTerm.hh
 * \author Sylvain Lobry
 * \date 02/10/2014
 * \brief File that defines class QuadraticDataTerm
 */


/**
 * \class QuadraticDataTerm
 * \brief Class to be used to compute the data term for a gaussian distribution
 *	  noise based.
 */
template <typename Input, typename Output>
class QuadraticDataTerm
{
public:
    /**
     * \brief	Method to compute the data term
     * \param	ObservedValue		Observed value (i.e. y_s).
     * \param	RegularizedValue	Value to be tested (i.e. x_s).
	 * \param	BetaS				Value for the L0 parameter.
     * \return  The value for the data term.
     */
    static double Compute(const Input& ObservedValue,
						  const Input& RegularizedValue,
						  const double BetaS,
						  const double BetaBV);
    /**
     * \brief Method to compute The sparse component.
     * \param	ObservedValue		Observed value (i.e. y_s).
     * \param	RegularizedValue	Value to be tested (i.e. x_s).
     * \param   BetaS			    Value for betaS.
     * \return  The value for Us.
     */
    static Output ComputeUs(const Input& ObservedValue,
							const Input& RegularizedValue,
							const double BetaS,
							const double BetaBV);

	/**
	 * \brief	Given an Us compute the data term.
	 * \param   ObservedValue			The observed value.
	 * \param	RegularizedValue		Regularized value chosen.
	 * \param	Us						The value chosen for Us.
	 * \param	BetaBV					Regularization parameter for TV.
	 */
    static double ComputeDataTermOnly(const Input& ObservedValue,
									  const Input& RegularizedValue,
									  const Input& Us,
									  const double BetaBV);

};

#include "QuadraticDataTerm.hxx"

#endif /* !QUADRATIC_DATA_TERM_HH_ */
