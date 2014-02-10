#ifndef QUADRATIC_DATA_TERM_HXX_
# define QUADRATIC_DATA_TERM_HXX_

#include "QuadraticDataTerm.hh"

template <typename Input, typename Output>
Output
QuadraticDataTerm<Input,Output>::Compute(const Input& ObservedValue,
					  const Input& RegularizedValue)
{
    return static_cast<Output> ((ObservedValue - RegularizedValue)
				* (ObservedValue - RegularizedValue));
}


#endif /* !QUADRATIC_DATA_TERM_HXX_ */
