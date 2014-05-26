#ifndef RICE_DATA_TERM_HXX_
# define RICE_DATA_TERM_HXX_

# include "RiceDataTerm.hh"
# include <cmath>
# include <limits>
# include <fstream>

//Set to false after having run once to save a lot of time...
# define FIRSTTIME (false)
# define SAVERICE (false)

template <typename Input, typename Output>
double
RiceDataTerm<Input,Output>::Compute(const Input& V,
									const Input& Ubv,
									const double BetaS,
									const double BetaBV)
{
	if (V <= Ubv)
	{
		Input Us = 0;
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);
	}

    Input Us = RiceDataTerm<Input,Output>::ComputeUs(V, Ubv, BetaS, BetaBV);

    if (Us == 0)
		return ComputeDataTermOnly(V, Ubv, Us, BetaBV);

    return BetaS/BetaBV;
}


template <typename Input, typename Output>
double
RiceDataTerm<Input,Output>::ComputeDataTermOnly(const Input& V,
												const Input& Ubv,
												const Input& Us,
												const double BetaBV)
{
	double dV = (double)V;
	double dUbv = (double)Ubv;
	double dUs = (double)Us;

	return ((dV * dV + dUs * dUs) / (dUbv * dUbv) + 2 * log(dUbv)
			- log(i0(2 * dV * dUs / (dUbv * dUbv)))) / BetaBV;
}

template <typename Input, typename Output>
Output
RiceDataTerm<Input,Output>::ComputeUs(const Input& V,
									  const Input& Ubv,
									  const double BetaS,
									  const double BetaBV)
{
    double tmp = ComputeDataTermOnly(V, Ubv, 0, BetaBV);
    if (tmp < BetaS / BetaBV)
		return 0;

	unsigned NTABUL = 65536;
	double step = 65536/(double)NTABUL;

	return ToptRice[(long)((double)V/(double)Ubv/step)] * (double)Ubv;
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

template <typename Input, typename Output>
double
RiceDataTerm<Input,Output>::i1(const Input& x)
{
	double ax,ans;
	double y;

	if ((ax=fabs(x)) < 3.75) {
		y=x/3.75;
		y*=y;
		ans=ax+y*(0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934+y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3))))));
	}
	else {
		y=3.75/ax;
		ans=0.2282967e-1+y*(-.2895312e-1+y*(0.1787654e-1-y*.420059e-2));
		ans=0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2+y*(0.163801e-2+y*(-0.1031555e-1+y*ans))));
		ans *= (exp(ax)/sqrt(ax));
	}
	return x < 0.0 ? -ans : ans;
}

template <typename Input, typename Output>
double
RiceDataTerm<Input,Output>::optRice(double v)
{
	double precision = 0.1;

	if (v < 1.414213)
		return 0;

	double inf = 0;
	double mid = 0.75 * v;
	double sup = 1.5 * v;

	double niter = sup/precision;
	for (long i = 0; i < niter; ++i)
	{
		if (mid * i0(v * mid) >= v * i1(v * mid))
			sup = mid;
		else
			inf = mid;
		mid = 0.5 * (sup + inf); //Really?
	}
	return mid;
}

template <typename Input, typename Output>
void
RiceDataTerm<Input,Output>::init()
{
	unsigned NTABUL = 65536;
	ToptRice = new double[NTABUL];

#if FIRSTTIME
	double step = 65536/(double)NTABUL;
#if SAVERICE
	std::ofstream ofs ("FastRice.txt", std::ofstream::out | std::ofstream::binary);
#endif
	for (unsigned i = 0; i < NTABUL; ++i)
	{
		ToptRice[i] = optRice(i * step);
#if SAVERICE
		ofs.write(reinterpret_cast<const char*>(&ToptRice[i]), sizeof(double));
#endif
	}
#if SAVERICE
	ofs.close();
#endif
#else
	std::ifstream ifs ("FastRice.txt", std::ifstream::in | std::ifstream::binary);
	for (unsigned i = 0; i < NTABUL; ++i)
	{
		ifs.read(reinterpret_cast<char*>(&ToptRice[i]), sizeof(double));
	}
	ifs.close();
#endif

}

template <typename Input, typename Output>
void
RiceDataTerm<Input,Output>::destroy()
{
	delete[] ToptRice;
}


#endif /* !RICE_DATA_TERM_HXX_ */
