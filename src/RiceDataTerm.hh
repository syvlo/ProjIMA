#ifndef RICE_DATA_TERM_HH_
# define RICE_DATA_TERM_HH_

/**
 * \file RiceDataTerm.hh
 * \author Sylvain Lobry
 * \date 03/06/2014
 * \brief File that defines the class RiceDataTerm
 */

# include <vector>

/**
 * \class RiceDataTerm
 * \brief Class to be used to compute the data term for a Rice distribution
 *	  noise based.
 */
template <typename Input, typename Output>
class RiceDataTerm
{
public:
    /**
     * \brief	Method to compute the data term
     * \param	ObservedValue		Observed value (i.e. y_s).
     * \param	RegularizedValue	Value to be tested (i.e. x_s)
     * \return  The value for the data term.
     */
    static Output Compute(const Input& V, const Input& Ubv, const std::vector<Input>& gamma, const double BetaS, const double BetaBv);

    static Output ComputeDataTermOnly(const Input& ObservedValue, const Input& RegularizedValue, const Input& Us, const double BetaBV);

    static Input ComputeUs(const Input& V, const Input& Ubv, const std::vector<Input>& gamma, const double BetaS, const double BetaBV);

    static double i0(const Input& x);
};

#include "RiceDataTerm.hxx"

#endif /* !RICE_DATA_TERM_HH_ */
