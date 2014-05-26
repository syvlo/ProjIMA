#ifndef UTILS_HH_
# define UTILS_HH_

#include <vector>
#include "Args.hh"

namespace misc
{

	//Use a vector of Scheme to describe the values expected in alpha
	struct Scheme
	{
		unsigned minVal;
		unsigned maxVal;
		unsigned step;
	};

	/**
	 * \brief Helper to construct alpha.
	 *
	 * Alpha is the quantification for the background image.
	 * \param radarMode			True if we handle SAR images.
	 * \param dataTerm			The data term to be used in the computations.
	 * \param schemes			The schemes to construct alpha.
	 * \return					Alpha.
	 */
	std::vector<unsigned> constructAlpha(bool radarMode,
										 DataTerm dataTerm,
										 const std::vector<Scheme>& schemes);

}


#endif /* !UTILS_HH_ */
