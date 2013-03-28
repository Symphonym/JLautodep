JLautodep
=========

A customizeable dependency generator for Makefiles. It takes a number of input files, discards the extensions
and uses the default extensions(.h and .cpp) for each filename, unless you've specified other extensions. It
then reads through all the input files and scans them for "#include <>" and "#include """ in order to create
a dependency list. The dependency list can either be outputted to a separate file or inserted directly to your
makefile by using custom(or default) tags.

It also supports exceptions, good when you don't want external libraries or the std library in your
dependency list.

Usage
-----
The optimal way to use JLautodep to would be to use a wildcard in a makefile getting all the files, then having
a target to run JLautodep with all the files as input. Outputting to an external file or inserting directly to the
makefile.

The command-line arguments supported by JLautodep are shown below, this help log can also be seen by running "JLautolog --help"

			JLautodep, Make Dependency Generator by Jakob Larsson
			
				required arguments:
					-i <files>
							Input files to the generator, file extension
							is discarded. Use -e to set custom extensions
					-o <file>
							Write to output file <file>
					-m <file>
							Alternative to -o, insert output to makefile
							<file> using tags in the makefile
			
				optional arguments:
					-e <extensions>
							File extensions used for each input file
					-E <exceptions>
							Include directive exceptions, a dependency
							matching a part of the exception is discarded
					--makeStart=<tag>
							Sets the START tag when outputting to Makefiles.
							Default: #JLautodep START
					--makeEnd=<tag>
							Sets the END tag when outputting to Makefiles.
							Default: #JLautodep END
					--empty-targets
							Allows empty targets to be generated, meaning
							targets with no dependencies
					--self-dependency
							Allows targets to have their own header files as
							a dependency
					--display-logging
							Enables logging, writing progress reports to the
							console as the generator runs
					--help
							Displays a list of accepted arguments and what
							they do

Made by
---------
Jakob Larsson, aged 17, 28th of March 2013