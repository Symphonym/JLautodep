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