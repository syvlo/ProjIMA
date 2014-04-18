#ifndef RAYLEIGH_DATA_TERM_2_VARS_HXX_
# define RAYLEIGH_DATA_TERM_2_VARS_HXX_

# include "RayleighDataTerm2Vars.hh"
# include <cmath>
# include <limits>

template <typename Input, typename Output>
double
RayleighDataTerm2Vars<Input,Output>::Compute(const Input& V,
											 const Input& Ubv,
											 const std::vector<Input>& gamma,
											 const double BetaS,
											 const double BetaBV)
{
	if (V >= Ubv)
	{
		Input Us = 0;
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);
	}

    Input Us = RayleighDataTerm2Vars<Input,Output>::ComputeUs(V, Ubv, gamma, BetaS, BetaBV);

    if (Us == 0)
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);

    return BetaS/BetaBV;
}


template <typename Input, typename Output>
double
RayleighDataTerm2Vars<Input,Output>::ComputeDataTermOnly(const Input& V,
						const Input& Ubv,
						const Input& Us,
						const double BetaBV)
{
	return (((double)V * (double)V) / (((double)Us + (double)Ubv) * ((double)Us + (double)Ubv)) + 2 * log((double)Ubv + (double)Us)) / BetaBV;
}

template <typename Input, typename Output>
Output
RayleighDataTerm2Vars<Input,Output>::ComputeUs(const Input& V,
											   const Input& Ubv,
											   const std::vector<Input>&,
											   const double BetaS,
											   const double BetaBV)
{
    double tmp = ComputeDataTermOnly(V, Ubv, 0, BetaBV);
    if (tmp < BetaS / BetaBV)
		return 0;

	return V - Ubv;
}


#endif /* !RAYLEIGH_DATA_TERM_2_VARS_HXX_ */
