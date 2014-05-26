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
 * by 100
 */
template <typename Input, typename Output>
class LogDataTerm
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

	/**
     * \brief	Method to compute Us?
     * \param	V		Observed value.
     * \param	Ubv		Value to be tested.
	 * \param	BetaS	Value for the L0 parameter.
	 * \param	BetaBV	Value for the TV parameter.
     * \return  The value for Us.
     */
    static Output ComputeUs(const Input& V, const Input& Ubv, const double BetaS, const double BetaBV);

};

#include "LogDataTerm.hxx"

#endif /* !LOG_DATA_TERM_HH_ */
