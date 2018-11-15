#! /bin/bash

for i in $(seq 1 $#); do
	case $1 in 
	"measures")
		# Remove measures
		rm lsb.tests.r*
		;;
	"cmake")
		# Remove makefiles 
		rm -r CMakeFiles/ Makefile compile_commands.json cmake_install.cmake CMakeCache.txt	
		;;
	esac
	shift
done

