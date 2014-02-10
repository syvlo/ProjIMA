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
     * \param	RegularizedValue	Value to be tested (i.e. x_s)
     * \return  The value for the data term.
     */
    static Output Compute(const Input& ObservedValue, const Input& RegularizedValue);
};

#include "QuadraticDataTerm.hxx"

#endif /* !QUADRATIC_DATA_TERM_HH_ */