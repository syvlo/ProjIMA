#ifndef ARGS_HH_
# define ARGS_HH_

/**
 * \file Args.hh
 * \author Sylvain Lobry
 * \date 02/01/2014
 * \brief File where arguments handling class is defined.
 */

# include <iostream>
# include <vector>

/**
 * Default value for BetaBV.
 */
# define DEFAULT_BBV (42)

/**
 * Default value for BetaS.
 */
# define DEFAULT_BS (42)

/**
 * Default value for RadarMode
 */
# define DEFAULT_RADAR_MODE (false)

/**
 * Default value for NonOptimalMode
 */
# define DEFAULT_NON_OPTIMAL_MODE (false)

/**
 * Default value for the size of the compute window.
 */
# define DEFAULT_COMPUTE_WINDOW_SIZE (100)

/**
 * Default value for the size of the filling window.
 */
# define DEFAULT_FILLING_WINDOW_SIZE (50)

/**
 * Default value for the shift of the window.
 */
# define DEFAULT_SHIFT_WINDOW (DEFAULT_FILLING_WINDOW_SIZE)

/**
 * Default value for OneBVSeveralS_.
 */
# define DEFAULT_ONE_BV_SEVERAL_S (false)

/**
 * Default value for dataterm_.
 */
# define DEFAULT_DATATERM (RAYLEIGH)

/**
 * Default value for QPNbLevels_.
 */
# define DEFAULT_QP_NB_LEVELS (100)

/**
 * Default value for QPPercentage_.
 */
# define DEFAULT_QP_PERCENTAGE (.95)

/**
 * \brief Enum to represent the dataterm to be used.
 */
typedef enum
{
	RICE, /**< Use Rice dataterm. */
	RAYLEIGH, /**< Use Rayleigh dataterm. */
	LOG, /**< Use Fisher-Tippet dataterm */
	GAUSSIAN /**< Use Gaussian dataterm */
} DataTerm;

/**
 * \class Args
 * \brief Defines program arguments and handle them.
 */
class Args
{
public:
    /**
     * \brief		Constructor.
     * \details		Parse programm arguments and store them.
     * \param		argc	Number of arguments.
     * \param		argv	Arguments.
     */
    Args (int argc, char* argv[]);

    /**
     * \brief		Destructor.
     * \details		Destroy char*s
     */
    ~Args();

	/**
	 * \brief		Copy constructor
	 * \param	    other		The args to copy.
	 */
	Args(const Args& other);

    /**
     * \brief		Check consistency among parsed arguments.
     * \details		that filenames for images are set.
     */
    bool checkConsistency() const;

    /**
     * \brief		Print help message.
     */
    void printHelp() const;

    /////////////
    // Getters //
    /////////////
    /**
     * \brief		Getter for BetaS.
     * \return		BetaS
     */
    double getBetaS() const;

    /**
     * \brief		Getter for BetaBV.
     * \return		BetaBV
     */
    double getBetaBV() const;

    /**
     * \brief		Getter for input images names.
     * \return		InputImages
     */
    const std::vector<char*> getInputImages() const;

    /**
     * \brief		Getter for BV output image.
     * \return		OutputImageBV.
     */
    const char* getOutputImageBV() const;

    /**
     * \brief		Getter for Scatterers output image.
     * \return		OutputImageS.
     */
    const char* getOutputImageS() const;

    /**
     * \brief		Getter for complete output image
     * \return		OutputImageComplete.
     */
    const char* getOutputImageComplete() const;

    /**
     * \brief		Getter for radar mode option.
     * \return		RadarMode.
     */
    bool getRadarMode() const;

	/**
	 * \brief		Getter for the non optimal mode.
	 * \return	    true if non optimal is set.
	 */
	bool getNonOptimalMode() const;

	/**
	 * \brief		Getter for the size of the compute window.
	 * \return		The size of the compute window.
	 */
	unsigned getComputeWindowSize() const;

	/**
	 * \brief		Getter for the size of the filling window.
	 * \return		The size of the filling window.
	 */
	unsigned getFillingWindowSize() const;

	/**
	 * \brief		Getter for the shift for the window.
	 * \return		The shift for the window.
	 */
	unsigned getShiftWindow() const;

	/**
	 * \brief	    Getter for the OneBVSeveralS_ bool.
	 * \return	    OneBVSeveralS_.
	 */
	bool getOneBVSeveralS() const;

	/**
	 * \brief		Getter for the Dataterm.
	 * \return		The dataterm.
	 */
	DataTerm getDataTerm () const;

	/**
	 * \brief	    Getter for the Input filename for quantification profile.
	 * \return		The quantification profile input filename.
	 */
	char* getQPFilenameInput() const;

	/**
	 * \brief	    Getter for the Output filename for quantification profile.
	 * \return		The quantification profile output filename.
	 */
	char* getQPFilenameOutput() const;

	/**
	 * \brief	    Getter for the number of levels in quantification.
	 * \return	    The number of levels in quantification.
	 */
	unsigned getQPNbLevels() const;

	/**
	 * \brief	    Getter for the percentage of histogram to be used in quantification.
	 * \return	    The percentage of histogram to be used in quantification.
	 */
	double getQPPercentage() const;

    /**
     * \brief		Getter for Help_ bool.
     * \return		Help_;
     */
    bool getHelp() const;

    /**
     * \brief		Print the arguments and their values.
     * \param		stream	The stream where arguments should be printed.
     * \param		args	The arguments to print.
     * \return		The stream.
     */
    friend std::ostream& operator<< (std::ostream& stream, const Args& args);

private:
    /**
     * \brief		Defines at which point the scatterers image should be
     *			sparse.
     */
    double BetaS_;

    /**
     * \brief		Defines at which point the bounded variation image
     *			should be bounded
     */
    double BetaBV_;

    /**
     * \brief		Path to the images to be denoised.
     */
	std::vector<char*> InputImages_;

    /**
     * \brief		Path to image where the bounded variations image
     *			should be stored.
     */
    char* OutputImageBV_;

    /**
     * \brief		Path to image where the scaterrers image should be
     *			stored.
     */
    char* OutputImageS_;

    /**
     * \brief		Path to image where the whole denoised image should be
     *			stored.
     */
    char* OutputImageComplete_;

    /**
     * \brief		Boolean to defines if radar mode or plain grey level.
     */
    bool RadarMode_;

	/**
	 * \brief		Boolean to defines if we should do a computation by parts.
	 */
	bool NonOptimalMode_;

	/**
	 * \brief		Size of the compute window.
	 */
	unsigned ComputeWindowSize_;

	/**
	 * \brief		Size of the filling window.
	 */
	unsigned FillingWindowSize_;

	/**
	 * \brief		Shift of the window.
	 */
	unsigned ShiftWindow_;

	/**
	 * \brief		Boolean to defines if we have to apply the method "One
	 *				BV, several S".
	 */
	bool OneBVSeveralS_;

	/**
	 * \brief	    The dataterm to be used.
	 */
	DataTerm dataterm_;

    /**
     * \brief		Boolean to defines if help was called.
     */
    bool Help_;

	/**
	 * \brief		Input filename for quantification profile.
	 */
	char* QPFilenameInput_;

	/**
	 * \brief		Output filename for quantification profile.
	 */
	char* QPFilenameOutput_;

	/**
	 * \brief       Number of levels for quantification.
	 */
	unsigned QPNbLevels_;

	/**
	 * \brief		Percentage of the histogram to quantify.
	 */
	double QPPercentage_;

    //We do not want to be able to initialize an empty arguments instance.
    Args();
};

#endif /* !ARGS_HH_ */
