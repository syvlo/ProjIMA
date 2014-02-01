#include <iostream>
#include "Args.hh"

int main (int argc, char* argv[])
{
    Args args(argc, argv);
    if (!args.checkConsistency())
    {
	std::cerr << "Error: in non-WindowMode, you have to define input and output images." << std::endl;
	args.printHelp();
	return (1);
    }

    std::cout << args << std::endl;
    return (0);
}
