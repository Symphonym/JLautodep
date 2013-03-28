#include "MakeDependencyGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>

MakeDependencyGenerator::MakeDependencyGenerator() :
	m_displayLogging(false),
	m_appendToMake(false),
	m_emptyTargets(false),
	m_selfDependency(false),
	m_outputFile(""),
	m_makeStart("#JL Make Dependency START"),
	m_makeEnd("#JL Make Dependency END")
{

}

void MakeDependencyGenerator::addFile(std::string fileName)
{
	if(fileName.find_last_of('.') != std::string::npos)
	{
		std::size_t dotIndex = fileName.find_last_of('.');

		// Remove file extension, and check if the dot has a / or \ after it, possibly indicating a directory and not a file extension
		if((dotIndex+1) < fileName.size() && fileName[dotIndex+1] != '\\' && fileName[dotIndex+1] != '/')
			fileName = fileName.substr(0, fileName.find_last_of('.'));

	}
	m_inputFiles.push_back(fileName);
}

void MakeDependencyGenerator::runScanner()
{
	writeDebug("", true);
	writeDebug("Scanner started", true);

	// Using default extensions if none has been specified
	if(m_extensions.empty())
	{
		m_extensions.push_back(".h");
		m_extensions.push_back(".cpp");
		writeDebug("Default extensions used: .h and .cpp", true);
	}

	// Checking if there's any input files
	if(m_inputFiles.empty())
	{
		std::cout << "ERROR: No input files were provided" << std::endl;
		return;
	}

	for(std::size_t i = 0; i < m_inputFiles.size(); i++)
	{
		for(std::size_t e = 0; e < m_extensions.size(); e++)
		{
			writeDebug("Scanning file (" + m_inputFiles[i] + m_extensions[e] + "): ");
			std::ifstream reader(m_inputFiles[i] + m_extensions[e], std::ifstream::in);

			// File could not be read
			if(!reader.good())
			{
				writeDebug(" NOT FOUND", true);
				reader.close();
				continue;
			}

			std::string line = "", dependency = "";
			bool isQuoteInclude = true;
			while(std::getline(reader, line))
			{

				dependency = "";
				isQuoteInclude = true;

				if(line.find("#include") != std::string::npos)
				{

					// It takes the first " or < and connects them to the end of the line, so multiple includes
					// on the same line are not supported, it could be made supportable, but it's not.

					// #include""
					if(line.find('"') != std::string::npos)
						dependency = line.substr(line.find_first_of('"') + 1, (line.length() - 1) - ((line.find_first_of('"') + 1)));

					// #include<>  
					else if(line.find('<') != std::string::npos && line.find('>') != std::string::npos)
					{
						dependency = line.substr(line.find_first_of('<') + 1, (line.length() - 1) - (line.find_first_of('<') + 1));
						isQuoteInclude = false;
					}

					// Check if #include directive contains an exception
					if(!m_exceptions.empty() && !dependency.empty())
					{
						std::string includeDirective = isQuoteInclude ? "\""+dependency+"\"" : "<"+dependency+">";

						for(std::size_t ex = 0; ex < m_exceptions.size(); ex++)
							if(includeDirective.find(m_exceptions[ex]) != std::string::npos)
								dependency = ""; 
					}


					// Create target and add depedencies to it
					if(!dependency.empty())
					{
						// Check if self dependency is disabled
						if(m_selfDependency)
							m_targets[m_inputFiles[i]].push_back(dependency);
						else
						{
							// Remove extension from dependency and compare to target
							std::string dependencyOnly = dependency.substr(0, dependency.length() - (dependency.length() - dependency.find_last_of('.')));
							if(dependencyOnly != m_inputFiles[i])
								m_targets[m_inputFiles[i]].push_back(dependency);
						}
					}
					// Check if target exists, if not, add it with no dependencies, and check if user has emptyTargets enabled
					else if(m_targets.find(m_inputFiles[i]) == m_targets.end() && m_emptyTargets)
						m_targets[m_inputFiles[i]] = std::vector<std::string>();

				}
			}

			reader.close();

			// File was scanned successfully
			writeDebug(" OK", true);
		}
	}

	writeDebug("Scanner done", true);
	writeDebug("", true);
}

void MakeDependencyGenerator::runGenerator()
{
	// Makefile appending
	std::vector<std::string> fileLineData;
	std::size_t startIndex = 0, endIndex = 0;
	std::string line = "";
	if(m_appendToMake)
	{
		writeDebug("Scanning Makefile", true);

		// Scan makefile for specified START and END tags and store
		// these line numbers as the dependency list will be outputted
		// between the tags. To avoid discarding existing data in the Makefile
		std::ifstream reader(m_outputFile, std::ifstream::in);
		std::size_t indexCount = 0;
		while(std::getline(reader, line))
		{
			if(line == m_makeStart)
			{
				writeDebug("Makefile START tag found", true);
				startIndex = indexCount + 1;
			}
			else if(line == m_makeEnd)
			{
				writeDebug("Makefile END tag found", true);
				endIndex = indexCount;
			}

			fileLineData.push_back(line);

			++indexCount;
		}

		writeDebug("Scanning done", true);

		reader.close();
	}
	writeDebug("", true);

	// Remove data between START and END
	fileLineData.erase(fileLineData.begin() + startIndex , fileLineData.begin() + endIndex);

	writeDebug("Generator started (Output: " + m_outputFile + ")", true);


	// Writing dependencies to vector
	for(auto itr = m_targets.begin(); itr != m_targets.end(); itr++)
	{
		writeDebug(itr->first + ".o:");
		std::string line = "";

		// Add target
		line += itr->first + ".o:";

		// Add dependencies
		for(std::size_t i = 0; i < itr->second.size(); i++)
		{
			writeDebug(" " + itr->second[i]);
			line += " " + itr->second[i];
		}
		writeDebug("", true);

		fileLineData.insert(fileLineData.begin() + startIndex, line);
	}

	// Output vector
	writeDebug("Generating to: " + m_outputFile, true);
	std::ofstream writer(m_outputFile, std::ofstream::trunc);
	for(std::size_t i = 0; i < fileLineData.size(); i++)
		writer << fileLineData[i] << std::endl;

	writer.close();
	writeDebug("Generator done", true);
}

void MakeDependencyGenerator::writeDebug(const std::string &logMessage, bool newLine)
{
	if(m_displayLogging)
		{
		if(!newLine)
			std::cout << logMessage;
		else
			std::cout << logMessage << std::endl;
	}
}

bool MakeDependencyGenerator::parseArgs(int argc, const char *args[])
{
	if(argc <= 1)
	{
		std::cout << "JLautodep, Make Dependency Generator by Jakob Larsson" << std::endl;
		return false;
	}

	int inputType = -1;
	for(int i = 0; i < argc; i++)
	{
		const std::string inputArg = args[i];

		if(inputArg == "-i") // Input files
			inputType = 0;
		else if(inputArg == "-E") // Exceptions
			inputType = 1;
		else if(inputArg == "-e") // Extensions
			inputType = 2;
		else if(inputArg == "-o") // Output file
			inputType = 3;
		else if(inputArg == "-m")
			inputType = 4;
		else if(inputArg.find("--makeStart=") != std::string::npos)
		{
			std::stringstream ss;
			ss << inputArg.substr(inputArg.find('=') + 1, inputArg.length() - inputArg.find('='));
			m_makeStart = ss.str();
		}
		else if(inputArg.find("--makeEnd=") != std::string::npos)
		{
			std::stringstream ss;
			ss << inputArg.substr(inputArg.find('=') + 1, inputArg.length() - inputArg.find('='));
			m_makeEnd = ss.str();
		}
		else if(inputArg == "--empty-targets")
		{
			m_emptyTargets = true;
			inputType = -1;
		}
		else if(inputArg == "--self-dependency")
		{
			m_selfDependency = true;
			inputType = -1;
		}
		else if(inputArg == "--display-logging")
		{
			m_displayLogging = true;
			inputType = -1;
		}
		else if(inputArg == "--help")
		{
			std::vector<std::string> helpLog = { 
			"JLautodep, Make Dependency Generator by Jakob Larsson",
			"",
			"	required arguments:",
			"		-i <files>",
			"				Input files to the generator, file extension",
			"				is discarded. Use -e to set custom extensions",
			"		-o <file>",
			"				Write to output file <file>",
			"		-m <file>",
			"				Alternative to -o, insert output to makefile",
			"				<file> using tags in the makefile",
			"",
			"	optional arguments:",
			"		-e <extensions>",
			"				File extensions used for each input file",
			"		-E <exceptions>",
			"				Include directive exceptions, a dependency",
			"				matching a part of the exception is discarded",
			"		--makeStart=<tag>",
			"				Sets the START tag when outputting to Makefiles.",
			"				Default: #JL Make Depedency Generator START",
			"		--makeEnd=<tag>",
			"				Sets the END tag when outputting to Makefiles.",
			"				Default: #JL Make Depedency Generator END",
			"		--empty-targets",
			"				Allows empty targets to be generated, meaning",
			"				targets with no dependencies",
			"		--self-dependency",
			"				Allows targets to have their own header files as",
			"				a dependency",
			"		--display-logging",
			"				Enables logging, writing progress reports to the",
			"				console as the generator runs",
			"		--help",
			"				Displays a list of accepted arguments and what",
			"				they do"};

			for(std::size_t i = 0; i < helpLog.size(); i++)
				std::cout << helpLog[i] << std::endl;
			
			// Don't allow the generator to be run when help log is requested
			// because it wouldn't make sense to run the help command alongside
			// with generator commands.
			return false;
		}
		else
		{
			if(inputType == 0) // Input files
				addFile(inputArg);
			else if(inputType == 1) // Exceptions
				addException(inputArg);
			else if(inputType == 2) // Extensions
				addExtension(inputArg);
			else if(inputType == 3) // Output file
			{
				// Invalidate as afterwards as there can only be 1 output file
				m_outputFile = inputArg;
				inputType = -1;
			}
			else if(inputType == 4) // Makefile output file
			{
				// Invalidate as afterwards as there can only be 1 output file
				m_outputFile = inputArg;
				m_appendToMake = true;
				inputType = -1;
			}
		}


	}

	return true;
}