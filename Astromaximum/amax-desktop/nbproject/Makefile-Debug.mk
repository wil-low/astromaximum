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
	build/Debug/GNU-Linux-x86/main.o \
	build/Debug/GNU-Linux-x86/MainWindow.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=\
	`../../../fltk-2.0.x-r5864/fltk2-config \
	--ldflags` \
	-lsqlite3

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} amax-desktop

amax-desktop: ${OBJECTFILES}
	${LINK.cc} -o amax-desktop ${OBJECTFILES} ${LDLIBSOPTIONS} 

build/Debug/GNU-Linux-x86/main.o: main.cxx 
	${MKDIR} -p build/Debug/GNU-Linux-x86
	$(COMPILE.cc) -g -I../../../fltk-2.0.x-r5864 -o build/Debug/GNU-Linux-x86/main.o main.cxx

build/Debug/GNU-Linux-x86/MainWindow.o: MainWindow.cxx 
	${MKDIR} -p build/Debug/GNU-Linux-x86
	$(COMPILE.cc) -g -I../../../fltk-2.0.x-r5864 -o build/Debug/GNU-Linux-x86/MainWindow.o MainWindow.cxx

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} amax-desktop

# Subprojects
.clean-subprojects:
