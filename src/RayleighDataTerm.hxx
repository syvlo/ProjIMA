#ifndef RAYLEIGH_DATA_TERM_HXX_
# define RAYLEIGH_DATA_TERM_HXX_
#include "RayleighDataTerm.hh"
#include <cmath>

template <typename Input, typename Output>
Output
RayleighDataTerm<Input,Output>::Compute(const Input& ObservedValue,
										const Input& RegularizedValue,
										const double)
{
    return static_cast<Output> ((ObservedValue * ObservedValue)
				/ (RegularizedValue * RegularizedValue)
				+ 2 * log(RegularizedValue));
}

template <typename Input, typename Output>
Output
RayleighDataTerm<Input,Output>::ComputeUs(const Input&,
										  const Input&,
										  const double)
{
    return 0;
}

#endif /* !RAYLEIGH_DATA_TERM_HXX_ */
