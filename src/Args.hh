#ifndef ARGS_HH_
# define ARGS_HH_

/**
 * \file Args.hh
 * \author Sylvain Lobry
 * \date 02/01/2014
 * \brief File where arguments handling class is defined.
 */

# include <iostream>

/**
 * Default value for BetaBV.
 */
# define DEFAULT_BBV (42)

/**
 * Default value for BetaS.
 */
# define DEFAULT_BS (42)

/**
 * Default value for WindowMode.
 */
# define DEFAULT_WINDOW_MODE (false)

/**
 * Default value for RadarMode
 */
# define DEFAULT_RADAR_MODE (false)

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
     * \brief		Check consistency among parsed arguments.
     * \details		Check, if we are not in window mode, that filenames
     *			for images are specified (cannot work otherwise...).
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
     * \brief		Getter for input image name.
     * \return		InputImage
     */
    const char* getInputImage() const;

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
     * \brief		Getter for window mode option.
     * \return		WindowMode.
     */
    bool getWindowMode() const;

    /**
     * \brief		Getter for radar mode option.
     * \return		RadarMode.
     */
    bool getRadarMode() const;


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
     * \brief		Path to the image to be denoised.
     */
    char* InputImage_;

    /**
     * \brief		Path to where the bounded variations image should be
     *			stored.
     */
    char* OutputImageBV_;

    /**
     * \brief		Path to where the scaterrers image should be stored.
     */
    char* OutputImageS_;

    /**
     * \brief		Boolean to defines if a GUI should be used.
     */
    bool WindowMode_;

    /**
     * \brief		Boolean to defines if radar mode or plain grey level.
     */
    bool RadarMode_;

    //We do not want to be able to initialize an empty arguments instance.
    Args();
};

#endif /* !ARGS_HH_ */
