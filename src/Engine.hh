#ifndef ENGINE_HH_
# define ENGINE_HH_

/**
 * \file Engine.hh
 * \author Sylvain Lobry
 * \date 05/19/2014
 * \brief File where class handling computation is defined
 */

# include "Args.hh"

# include <cv.h>
# include <vector>

template <typename Minimizer>
class Engine
{
public:

	/**
	 * \brief  Constructor
	 * \param  Args		The arguments for the engine.
	 * \param  alpha	Vector of quantified values for BV.
	 */
	Engine(const std::vector<unsigned>& alpha);

	/**
	 * \brief  Destructor
	 */
	~Engine();

	/**
	 * \brief      Do the actual computations.
	 * \param[in]  Inputs		The inputs.
	 * \param[out] OutputsBV	Place where outputsBV are stored.
	 * \param[out] OutputsS 	Place where outputsS are stored.
	 * \param[out] OutputsC		Place where outputsC are stored.
	 */
	void Compute(const std::vector<cv::Mat>& Inputs,
				 std::vector<cv::Mat>& OutputsBV,
				 std::vector<cv::Mat>& OutputsS,
				 std::vector<cv::Mat>& OutputsC,
				 const Args& args);

private:
	const std::vector<unsigned> alpha_;

	/**
	 * \brief  Initialisation method (called by constructor).
	 */
	void init();

	/**
	 * \brief  Destroy method (called by destructor).
	 */
	void destroy();

};

# include "Engine.hxx"

#endif /* !ENGINE_HH_ */
