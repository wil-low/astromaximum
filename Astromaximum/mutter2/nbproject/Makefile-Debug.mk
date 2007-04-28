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
	build/Debug/GNU-Windows/evclass.o \
	build/Debug/GNU-Windows/datafile.o \
	build/Debug/GNU-Windows/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=\
	../swe/src/libswe.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/GNU-Windows/mutter2.exe

dist/Debug/GNU-Windows/mutter2.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Windows
	${LINK.cc} -o dist/Debug/GNU-Windows/mutter2 ${OBJECTFILES} ${LDLIBSOPTIONS} 

build/Debug/GNU-Windows/evclass.o: evclass.cpp 
	${MKDIR} -p build/Debug/GNU-Windows
	$(COMPILE.cc) -g -Wall -I../swe/src -o build/Debug/GNU-Windows/evclass.o evclass.cpp

build/Debug/GNU-Windows/datafile.o: datafile.cpp 
	${MKDIR} -p build/Debug/GNU-Windows
	$(COMPILE.cc) -g -Wall -I../swe/src -o build/Debug/GNU-Windows/datafile.o datafile.cpp

build/Debug/GNU-Windows/main.o: main.cpp 
	${MKDIR} -p build/Debug/GNU-Windows
	$(COMPILE.cc) -g -Wall -I../swe/src -o build/Debug/GNU-Windows/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Windows/mutter2.exe

# Subprojects
.clean-subprojects:
