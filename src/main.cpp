
/*

Programmed by Jakob Larsson (17 years old), 27th of march 2013
JLautodep reads through the inputted files and looks for 
#includes in order to output a makefile dependency list.

*/

#include "MakeDependencyGenerator.h"

int main(int argc, const char *args[])
{
	MakeDependencyGenerator generator;
	if(generator.parseArgs(argc, args))
	{
		generator.runScanner();
		generator.runGenerator();
	}
	return 0;
}