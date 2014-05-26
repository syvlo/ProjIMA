#ifndef RAYLEIGH_DATA_TERM_2_VARS_HH_
# define RAYLEIGH_DATA_TERM_2_VARS_HH_

/**
 * \file RayleighDataTerm2Vars.hh
 * \author Sylvain Lobry
 * \date 04/17/2014
 * \brief File that defines the class RayleighDataTerm2Vars
 */

# include <vector>

/**
 * \class RayleighDataTerm2Vars
 * \brief Class to be used to compute the data term for a noise based on a
 *        Rice distribution
 *		  This one should be used on normal images when RayleighDataTerm
 *		  should be used on classic images.
 */

template <typename Input, typename Output>
class RayleighDataTerm2Vars
{
public:
    /**
     * \brief	Method to compute the data term
     * \param	V		Observed value.
     * \param	Ubv		Value to be tested.
	 * \param	BetaS	Value for the L0 parameter.
	 * \param	BetaBV	Value for the TV parameter.
     * \return  The value for the data term.
     */
    static double Compute(const Input& V,
						  const Input& Ubv,
						  const double BetaS,
						  const double BetaBv);

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

	/**
     * \brief	Method to compute Us?
     * \param	V		Observed value.
     * \param	Ubv		Value to be tested.
	 * \param	BetaS	Value for the L0 parameter.
	 * \param	BetaBV	Value for the TV parameter.
     * \return  The value for Us.
     */
    static Output ComputeUs(const Input& V,
							const Input& Ubv,
							const double BetaS,
							const double BetaBV);
};

#include "RayleighDataTerm2Vars.hxx"

#endif /* !RAYLEIGH_DATA_TERM_2_VARS_HH_ */
