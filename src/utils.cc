#include "utils.hh"
#include <cmath>

std::vector<unsigned>
misc::constructAlpha(bool radarMode,
					 DataTerm dataTerm,
					 const std::vector<Scheme>& schemes)
{
	std::vector<unsigned> alpha;

	for (std::vector<Scheme>::const_iterator it = schemes.begin();
		 it != schemes.end(); ++it)
	{
		//It does not make sens to have a value larger than 255
		//in non radar mode.
		unsigned maxVal = (!radarMode && it->maxVal > 255 ? 255 : it->maxVal);

		//If maxVal of this scheme is the same as minVal in next scheme,
		//do not add it (avoid label duplication).
		if ((it + 1) != schemes.end() && (it + 1)->minVal == it->maxVal)
			maxVal = maxVal - 1;

		//Push the labels.
		for (unsigned i = it->minVal; i <= maxVal; i += it->step)
			alpha.push_back(i);
	}

	//If we are in log, we need to take the log of the labels.
	if (dataTerm == LOG)
	{
		for (unsigned i = 0; i < alpha.size(); ++i)
		{
			double var = alpha[i];

			//Times 100 so we can stay in unsigned.
			var = log(var) * 100;
			alpha[i] = (unsigned)var;
		}
	}

	return alpha;
}
