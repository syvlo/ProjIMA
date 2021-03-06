#ifndef ENGINE_HXX_
# define ENGINE_HXX_

# include "Engine.hh"
# include "TVL0DecompositionMinimizer.hh"
# include "TVL0DecompositionMinimizerOneBVSeveralS.hh"
# include "ComputeByParts.hh"
# include "ComputeByPartsLinear.hh"


template <typename Minimizer>
Engine<Minimizer>::Engine(const std::vector<unsigned>& alpha)
	: alpha_(alpha)
{
	this->init();
}

template <typename Minimizer>
Engine<Minimizer>::~Engine()
{
	this->destroy();
}

template <typename Minimizer>
void
Engine<Minimizer>::Compute(const std::vector<cv::Mat>& Inputs,
						   std::vector<cv::Mat>& OutputsBV,
						   std::vector<cv::Mat>& OutputsS,
						   std::vector<cv::Mat>& OutputsC,
						   const Args& args)
{
	if (args.getOneBVSeveralS())
	{
		TVL0DecompositionMinimizerOneBVSeveralS<Minimizer> TVL0 (alpha_,
																 args.getBetaBV(),
																 args.getBetaS());

		if (args.getNonOptimalMode())
		{
			if (args.getShiftWindow() != args.getFillingWindowSize())
			{
				ComputeByPartsLinear<TVL0DecompositionMinimizerOneBVSeveralS<Minimizer > > computer(args.getComputeWindowSize(), args.getFillingWindowSize(), args.getShiftWindow(), TVL0);

				computer.compute(Inputs, 30);

				OutputsBV = computer.getOutputsBV();
				OutputsS = computer.getOutputsS();
				OutputsC = computer.getOutputsC();
			}
			else
			{
				ComputeByParts<TVL0DecompositionMinimizerOneBVSeveralS<Minimizer > > computer(args.getComputeWindowSize(), args.getFillingWindowSize(), TVL0);

				computer.compute(Inputs, args.getOneBVSeveralS(), false);

				OutputsBV = computer.getOutputsBV();
				OutputsS = computer.getOutputsS();
				OutputsC = computer.getOutputsC();
			}
		}
		else
		{
			TVL0.compute(Inputs);

			OutputsBV = TVL0.getOutputsBV();
			OutputsS = TVL0.getOutputsS();
			OutputsC = TVL0.getOutputsComplete();
		}
	}
	else
	{
		TVL0DecompositionMinimizer<Minimizer> TVL0 (alpha_,
													args.getBetaBV(),
													args.getBetaS());
		if (args.getNonOptimalMode())
		{
			if (args.getShiftWindow() != args.getFillingWindowSize())
			{
				ComputeByPartsLinear<TVL0DecompositionMinimizer<Minimizer > > computer(args.getComputeWindowSize(), args.getFillingWindowSize(), args.getShiftWindow(), TVL0);

				computer.compute(Inputs, 30);

				OutputsBV = computer.getOutputsBV();
				OutputsS = computer.getOutputsS();
				OutputsC = computer.getOutputsC();
			}
			else
			{
				ComputeByParts<TVL0DecompositionMinimizer<Minimizer > > computer(args.getComputeWindowSize(), args.getFillingWindowSize(), TVL0);

				computer.compute(Inputs, args.getOneBVSeveralS(), true);

				OutputsBV = computer.getOutputsBV();
				OutputsS = computer.getOutputsS();
				OutputsC = computer.getOutputsC();
			}
		}
		else
		{
			TVL0.compute(Inputs);

			OutputsBV = TVL0.getOutputsBV();
			OutputsS = TVL0.getOutputsS();
			OutputsC = TVL0.getOutputsComplete();
		}
	}
}

template <typename Minimizer>
void
Engine<Minimizer>::init()
{
}

template <>
void
Engine<RiceDataTerm<unsigned, unsigned> >::init()
{
	RiceDataTerm<unsigned, unsigned>::init();
}

template <typename Minimizer>
void
Engine<Minimizer>::destroy()
{
}

template <>
void
Engine<RiceDataTerm<unsigned, unsigned> >::destroy()
{
	RiceDataTerm<unsigned, unsigned>::destroy();
}

#endif /* !ENGINE_HXX_ */
