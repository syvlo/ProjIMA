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
 * \brief Class to be used to compute the data term for a Rice distribution
 *	  noise based.
 */
template <typename Input, typename Output>
class RayleighDataTerm2Vars
{
public:
    /**
     * \brief	Method to compute the data term
     * \param	ObservedValue		Observed value (i.e. y_s).
     * \param	RegularizedValue	Value to be tested (i.e. x_s)
     * \return  The value for the data term.
     */
    static double Compute(const Input& V, const Input& Ubv, const std::vector<Input>& gamma, const double BetaS, const double BetaBv);

    static double ComputeDataTermOnly(const Input& ObservedValue, const Input& RegularizedValue, const Input& Us, const double BetaBV);

    static Output ComputeUs(const Input& V, const Input& Ubv, const std::vector<Input>& gamma, const double BetaS, const double BetaBV);
};

#include "RayleighDataTerm2Vars.hxx"

#endif /* !RAYLEIGH_DATA_TERM_2_VARS_HH_ */
