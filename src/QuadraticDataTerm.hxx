#ifndef QUADRATIC_DATA_TERM_HXX_
# define QUADRATIC_DATA_TERM_HXX_

#include "QuadraticDataTerm.hh"

template <typename Input, typename Output>
double
QuadraticDataTerm<Input,Output>::Compute(const Input& V,
										 const Input& Ubv,
										 const double BetaS,
										 const double BetaBV)
{
	if (V <= Ubv)
	{
		Input Us = 0;
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);
	}

    Input Us = ComputeUs(V, Ubv, BetaS, BetaBV);

    if (Us == 0)
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);

    return BetaS/BetaBV;
}

template <typename Input, typename Output>
double
QuadraticDataTerm<Input,Output>::ComputeDataTermOnly(const Input& ObservedValue,
													 const Input& RegularizedValue,
													 const Input&, const double)
{
	double v = (double) ObservedValue;
	double u = (double) RegularizedValue;
	if (u > .5)
	{
		u = 1;
	}
	else
	{
		u = 0;
	}
//	std::clog << (v-u)*(v-u) << std::endl;
	return (v-u)*(v-u);
    return static_cast<double> ((ObservedValue - RegularizedValue)
				* (ObservedValue - RegularizedValue));
}

template <typename Input, typename Output>
Output
QuadraticDataTerm<Input,Output>::ComputeUs(const Input& V,
										   const Input& Ubv,
										   const double BetaS,
										   const double BetaBV)
{
    double tmp = ComputeDataTermOnly(V, Ubv, 0, BetaBV);
    if (tmp < BetaS / BetaBV)
		return 0;

	return V - Ubv;
}

#endif /* !QUADRATIC_DATA_TERM_HXX_ */
