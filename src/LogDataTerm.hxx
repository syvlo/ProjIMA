#ifndef LOG_DATA_TERM_HXX_
# define LOG_DATA_TERM_HXX_

# include "LogDataTerm.hh"
# include <cmath>
# include <limits>

template <typename Input, typename Output>
double
LogDataTerm<Input,Output>::Compute(const Input& V,
								   const Input& Ubv,
								   const std::vector<Input>& gamma,
								   const double BetaS,
								   const double BetaBV)
{
    Input Us = LogDataTerm<Input,Output>::ComputeUs(V, Ubv, gamma, BetaS, BetaBV);

    if (Us == 0)
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);

    return BetaS/BetaBV;
}


template <typename Input, typename Output>
double
LogDataTerm<Input,Output>::ComputeDataTermOnly(const Input& V,
						const Input& Ubv,
						const Input& Us,
						const double BetaBV)
{
	double dV = (double)V * 100;
	double dUBV = (double)Ubv * 100;
	double dUS = (double)Us * 100;

	return dUBV + dUS - dV + exp(dV - dUBV - dUS);
}

template <typename Input, typename Output>
Output
LogDataTerm<Input,Output>::ComputeUs(const Input& V,
				      const Input& Ubv,
				      const std::vector<Input>& gamma,
				      const double BetaS,
				      const double BetaBV)
{
    double minValue = std::numeric_limits<double>::max();

    double tmp = ComputeDataTermOnly(V, Ubv, 0, BetaBV);
    if (tmp < BetaS / BetaBV)
	return 0;

    Output min;
    for (typename std::vector<Input>::const_iterator it = gamma.begin(); it != gamma.end(); ++it)
    {
	double val = ComputeDataTermOnly(V, Ubv, *it, BetaBV);
	if (val < minValue)
	{
	    minValue = val;
	    min = *it;
	}
    }

    if (minValue + BetaS < ComputeDataTermOnly(V, Ubv, 0, BetaBV))
	return min;
    else
	return 0;
}


#endif /* !LOG_DATA_TERM_HXX_ */
