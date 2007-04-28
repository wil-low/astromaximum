#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add custumized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=g77

# Include project Makefile
include Makefile

# Object Files
OBJECTFILES= \
	build/Release/GNU-Windows/evclass.o \
	build/Release/GNU-Windows/datafile.o \
	build/Release/GNU-Windows/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=\
	-L../swe \
	-lswe

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} ../mutter/mutter2.exe

../mutter/mutter2.exe: ${OBJECTFILES}
	${MKDIR} -p ../mutter
	${LINK.cc} -o ../mutter/mutter2 -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

build/Release/GNU-Windows/evclass.o: evclass.cpp 
	${MKDIR} -p build/Release/GNU-Windows
	$(COMPILE.cc) -O2 -I../swe -o build/Release/GNU-Windows/evclass.o evclass.cpp

build/Release/GNU-Windows/datafile.o: datafile.cpp 
	${MKDIR} -p build/Release/GNU-Windows
	$(COMPILE.cc) -O2 -I../swe -o build/Release/GNU-Windows/datafile.o datafile.cpp

build/Release/GNU-Windows/main.o: main.cpp 
	${MKDIR} -p build/Release/GNU-Windows
	$(COMPILE.cc) -O2 -I../swe -o build/Release/GNU-Windows/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} ../mutter/mutter2.exe

# Subprojects
.clean-subprojects:
