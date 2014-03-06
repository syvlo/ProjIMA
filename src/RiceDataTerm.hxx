#ifndef RICE_DATA_TERM_HXX_
# define RICE_DATA_TERM_HXX_

# include "RiceDataTerm.hh"
# include <cmath>
# include <limits>

template <typename Input, typename Output>
Output
RiceDataTerm<Input,Output>::Compute(const Input& V,
				    const Input& Ubv)
{
    Input Us = RiceDataTerm<Input,Output>::ComputeUs(Ubv);
    return ComputeDataTermOnly(V, Ubv, Us);
}


template <typename Input, typename Output>
Output
RiceDataTerm<Input,Output>::ComputeDataTermOnly(const Input& V,
						const Input& Ubv,
						const Input& Us)
{
    return static_cast<Output> ((V * V + Us * Us) / (2 * Ubv * Ubv)
				+ 2 log(Ubv)
				- log(i0(v * Us / (Ubv * Ubv))));
}

template <typename Input, typename Output>
Input
RiceDataTerm<Input,Output>::ComputeUs(const Input& V,
				      const Input& Ubv)
{
    Input minValue = std::numeric_limits<Input>::max();
    Input min;
    for (std::vector<Input>::const_iterator it = gamma.begin(); it != gamma.end(); ++it)
    {
	Input val = ComputeDataTermOnly(V, Ubv, *it);
	if (val < minValue)
	{
	    minValue = val;
	    min = *it;
	}
    }

    if (minValue + BetaS < ComputeDataTermOnly(V, Ubv, 0))
	return min;
    else
	return 0;
}

//From numerical recipies.
template <typename Input, typename Output>
Input
RiceDataTerm<Input,Output>::i0(const Input& x)
{
    Input ax,ans;
    Input y;

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
