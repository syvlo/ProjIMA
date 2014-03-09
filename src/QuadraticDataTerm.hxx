#ifndef QUADRATIC_DATA_TERM_HXX_
# define QUADRATIC_DATA_TERM_HXX_

#include "QuadraticDataTerm.hh"

template <typename Input, typename Output>
Output
QuadraticDataTerm<Input,Output>::Compute(const Input& ObservedValue,
					 const Input& RegularizedValue,
					 const std::vector<Input>&,
					 const double)
{
    return static_cast<Output> ((ObservedValue - RegularizedValue)
				* (ObservedValue - RegularizedValue));
}

template <typename Input, typename Output>
Output
QuadraticDataTerm<Input,Output>::ComputeUs(const Input&, const Input&,
					   const std::vector<Input>&, const double)
{
    return 0;
}

#endif /* !QUADRATIC_DATA_TERM_HXX_ */
