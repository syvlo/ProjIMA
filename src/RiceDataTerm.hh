#ifndef RICE_DATA_TERM_HH_
# define RICE_DATA_TERM_HH_

/**
 * \file RiceDataTerm.hh
 * \author Sylvain Lobry
 * \date 03/06/2014
 * \brief File that defines the class RiceDataTerm
 */

# include <vector>

/**
 * \class RiceDataTerm
 * \brief Class to be used to compute the data term for a Rice distribution
 *	      noise based.
 */

template <typename Input, typename Output>
class RiceDataTerm
{
public:
    /**
     * \brief	Method to compute the data term
     * \param	V		Observed value.
     * \param	Ubv		Value to be tested.
	 * \param	BetaS	Value for the L0 parameter.
	 * \param	BetaBV	Value for the TV parameter.
     * \return  The value for the data term.
     */
    static double Compute(const Input& V,
						  const Input& Ubv,
						  const double BetaS,
						  const double BetaBv);

	/**
	 * \brief	Given an Us compute the data term.
	 * \param   ObservedValue			The observed value.
	 * \param	RegularizedValue		Regularized value chosen.
	 * \param	Us						The value chosen for Us.
	 * \param	BetaBV					Regularization parameter for TV.
	 */
    static double ComputeDataTermOnly(const Input& ObservedValue,
									  const Input& RegularizedValue,
									  const Input& Us,
									  const double BetaBV);


	/**
     * \brief	Method to compute Us?
     * \param	V		Observed value.
     * \param	Ubv		Value to be tested.
	 * \param	BetaS	Value for the L0 parameter.
	 * \param	BetaBV	Value for the TV parameter.
     * \return  The value for Us.
     */
    static Output ComputeUs(const Input& V,
							const Input& Ubv,
							const double BetaS,
							const double BetaBV);

	/**
	 * \brief	Compute modified bessel function of the first kind - order 0.
	 * \param	x		Input value.
	 * \return  The output value.
	 */
    static double i0(const Input& x);

	/**
	 * \brief	Compute modified bessel function of the first kind - order 1.
	 * \param	x		Input value.
	 * \return  The output value.
	 */
    static double i1(const Input& x);

	/**
	 * \brief	Initialization method.
	 *			Will compute tab values and save or load them (depending on
	 *			the macro FIRSTTIME)
	 */
	static void init();

	/**
	 * \brief   Destroy the memory.
	 *			Free ToptRice.
	 */
	static void destroy();

	/**
	 * Tab values.
	 */
	static double* ToptRice;

private:
	/**
	 * \brief	Compute tab values.
	 * \param	input.
	 * \return	the value.
	 */
	static double optRice(double v);
};

template <class A, class B>
double* RiceDataTerm<A, B>::ToptRice = NULL;

#include "RiceDataTerm.hxx"

#endif /* !RICE_DATA_TERM_HH_ */
