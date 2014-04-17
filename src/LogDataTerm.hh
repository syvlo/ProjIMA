#ifndef LOG_DATA_TERM_HH_
# define LOG_DATA_TERM_HH_

/**
 * \file LogDataTerm.hh
 * \author Sylvain Lobry
 * \date 04/16/2014
 * \brief File that defines the class LogDataTerm to handle log images.
 */

# include <vector>

/**
 * \class LogDataTerm
 * \brief Class to be used to compute the data term for a log image multiplied
 * by 1000
 */
template <typename Input, typename Output>
class LogDataTerm
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

#include "LogDataTerm.hxx"

#endif /* !LOG_DATA_TERM_HH_ */
