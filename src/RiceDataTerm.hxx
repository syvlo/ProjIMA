#ifndef RICE_DATA_TERM_HXX_
# define RICE_DATA_TERM_HXX_

# include "RiceDataTerm.hh"
# include <cmath>
# include <limits>

template <typename Input, typename Output>
Output
RiceDataTerm<Input,Output>::Compute(const Input& V,
				    const Input& Ubv,
				    const std::vector<Input>& gamma,
				    const double BetaS,
				    const double BetaBV)
{
    Input Us = RiceDataTerm<Input,Output>::ComputeUs(V, Ubv, gamma, BetaS, BetaBV);

    if (Us == 0)
	return ComputeDataTermOnly(V, Ubv, Us, BetaBV);
    //    std::cout << "Pour V = " << V << ", Ubv = " << Ubv << ", res = " << res << std::endl;
    return BetaS/BetaBV;
}


template <typename Input, typename Output>
Output
RiceDataTerm<Input,Output>::ComputeDataTermOnly(const Input& V,
						const Input& Ubv,
						const Input& Us,
						const double BetaBV)
{
    // if (Us != 0)
	// return static_cast<Output> ((V * V + Us * Us) / (Ubv * Ubv)
	// 			    + 2 * log(Ubv)
	// 			    - log(i0(2 * V * Us / (Ubv * Ubv)))) / BetaBV;
    // else
  return static_cast<Output> ((V * V) / ((Us + Ubv) * (Us + Ubv)) + 2 * log(Ubv + Us)) / BetaBV;
}

template <typename Input, typename Output>
Input
RiceDataTerm<Input,Output>::ComputeUs(const Input& V,
				      const Input& Ubv,
				      const std::vector<Input>& gamma,
				      const double BetaS,
				      const double BetaBV)
{
    Output minValue = std::numeric_limits<Output>::max();

    Output tmp = ComputeDataTermOnly(V, Ubv, 0, BetaBV);
    if (tmp < BetaS / BetaBV)
	return 0;

    Input min;
    for (typename std::vector<Input>::const_iterator it = gamma.begin(); it != gamma.end(); ++it)
    {
	Input val = ComputeDataTermOnly(V, Ubv, *it, BetaBV);
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

//From numerical recipies.
template <typename Input, typename Output>
double
RiceDataTerm<Input,Output>::i0(const Input& x)
{
    double ax,ans;
    double y;

    if ((ax=fabs(x)) < 3.75) {
	y=x/3.75;
	y*=y;
	ans=1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492+y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
    }
    else {
	y=3.75/ax;
	ans=(exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1+y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2+y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1+y*0.392377e-2))))))));
    }
    return ans;
}

#endif /* !RICE_DATA_TERM_HXX_ */
