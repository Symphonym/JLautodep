#ifndef JL_MAKEDEPENDENCYGENERATOR_H
#define JL_MAKEDEPENDENCYGENERATOR_H

/*

Programmed by Jakob Larsson (17 years old), 27th of march 2013
GNU Make Dependency Generator reads through the inputted files
and looks for #includes in order to output a makefile dependency
list.


*/

#include <map>
#include <vector>
#include <string>

class MakeDependencyGenerator
{
private:

	// Targets, mapped together with their dependencies   ("Target.o: dependencies.h")
	std::map<std::string, std::vector<std::string>> m_targets;

	// Inputted files to be read
	std::vector<std::string> m_inputFiles;

	// List of exceptions
	std::vector<std::string> m_exceptions;

	// List of file extensions to use for each input file
	std::vector<std::string> m_extensions;

	bool m_displayLogging, m_appendToMake, m_emptyTargets, m_selfDependency;
	std::string m_outputFile, m_makeStart, m_makeEnd;

	void writeDebug(const std::string &logMessage, bool newLine = false);

public:

	explicit MakeDependencyGenerator();

	// Add a file to be read. It will discard the extension and
	// search for the files using the provided file extensions.
	// Default extensions are .h and .cpp.
	void addFile(std::string fileName);

	// Add an include directive not to be added to the outputted file. 
	// For example, an exception looking like "<" will discard all #includes
	// with "<" in it. Useful to discard std library includes and other libs.
	inline void addException(const std::string &includeDirective)
	{
		m_exceptions.push_back(includeDirective);
	};

	// Add an extension to use for each input file, default are .h and .cpp
	inline void addExtension(const std::string &extension)
	{
		m_extensions.push_back(extension);
	};

	// Reads through all the inputted files and scans them for #include's
	void runScanner();

	// Outputs the generated Makefile Dependency list to the specified file.
	// It will create a new file if the specified file was not found.
	void runGenerator();


	// Parse inputFiles, exceptions, extensions, outputFile and loggingDisplay through command line
	// Returns true if everything worked as intented
	bool parseArgs(int argc, const char *args[]);
};

#endif