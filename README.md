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

Example output
---------
I tried the JLautodep on a school game project of mine by calling the following target in the project's
makefile:

			.PHONY: autodep
			autodep:
				./JLautodep -i ${SRCS} -o output.txt -E "<" --empty-targets

This generated the following output:

			main.o: GameState.h Engine.h
			Weapon.o: AssetManager.h FrameAnimation.h TileMap.h TileCharacter.h Utility.h MessageLog.h
			TileOptionManager.o: Player.h AssetManager.h TileMap.h Utility.h
			TileOptionActions.o: TileMap.h MessageLog.h TileOptionManager.h Utility.h
			TileMap.o: Tile.h
			TileCharacter.o: FrameAnimation.h AssetManager.h TileMap.h
			Tile.o: TileCharacter.h
			TextureAsset.o: Asset.h
			StopWatch.o:
			StateManager.o: State.h
			State.o: StopWatch.h Engine.h
			SoundManager.o: AssetManager.h
			SoundBufferAsset.o: Asset.h
			Settings.o:
			RifleWeapon.o: Weapon.h TileMap.h
			Player.o: TileCharacter.h Weapon.h BedControl.h TileMap.h MessageLog.h Utility.h GunWeapon.h RifleWeapon.h
			MusicAsset.o: Asset.h
			MessageLog.o: AssetManager.h
			GunWeapon.o: Weapon.h TileMap.h
			GameState.o: Engine.h TileMap.h State.h CharacterManager.h TileOptionManager.h Player.h MessageLog.h TileOptionActions.h
			FrameAnimation.o: SoundManager.h
			FontAsset.o: Asset.h
			Engine.o: AssetManager.h StateManager.h Settings.h Utility.h SoundManager.h
			CharacterManager.o: TileCharacter.h TileMap.h
			BedControl.o: Player.h TileMap.h
			AssetManager.o: Asset.h TextureAsset.h FontAsset.h SoundBufferAsset.h MusicAsset.h


Made by
---------
Jakob Larsson, aged 17