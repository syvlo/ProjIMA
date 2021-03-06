#ifndef RAYLEIGH_DATA_TERM_HH_
# define RAYLEIGH_DATA_TERM_HH_

/**
 * \file RayleighDataTerm.hh
 * \author Sylvain Lobry
 * \date 02/12/2014
 * \brief File that defines class RayleighDataTerm.hh
 */

/**
 * \class RayleighDataTerm
 * \brief Class to be used to compute the data term for a rayleigh
 *	  distribution noise based.
 */
template <typename Input, typename Output>
class RayleighDataTerm
{
public:
    /**
     * \brief	Method to compute the data term
     * \param	ObservedValue		Observed value (i.e. y_s).
     * \param	RegularizedValue	Value to be tested (i.e. x_s).
     * \param   BetaS			    Value for betaS (not used here).
     * \return  The value for the data term.
     */
    static Output Compute(const Input& ObservedValue,
						  const Input& RegularizedValue,
						  const double BetaS);

    /**
     * \brief Method to compute The sparse component.
     * \param	ObservedValue		Observed value (i.e. y_s).
     * \param	RegularizedValue	Value to be tested (i.e. x_s).
     * \param   BetaS		     	Value for betaS (not used here).
     * \return  The value for Us.
     */
    static Output ComputeUs(const Input& ObservedValue,
							const Input& RegularizedValue,
							const double BetaS);
};

#include "RayleighDataTerm.hxx"

#endif /* !RAYLEIGH_DATA_TERM_HH_ */
