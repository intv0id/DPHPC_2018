#! /bin/bash

for i in $(seq 1 $#); do
	case $1 in 
	"rmlsb")
		# Remove measures
		rm lsb.*.r*
		;;
	"rmout")
		# Remove standard output and error output files
		rm job*.err job*.out
		;;
	"mvlsb")
		# Move lsb measures to data location
		mv lsb.*.r* ../interpret/data/
		;;
	"cmake")
		# Remove makefiles 
		rm -r CMakeFiles/ Makefile compile_commands.json cmake_install.cmake CMakeCache.txt	
		;;
	esac
	shift
done

