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
	build/DebugConf/GNU-Linux-x86/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=\
	`../../../fltk2/fltk2-config \
	--ldflags`

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} amax-desktop

amax-desktop: ${OBJECTFILES}
	${LINK.cc} -o amax-desktop ${OBJECTFILES} ${LDLIBSOPTIONS} 

build/DebugConf/GNU-Linux-x86/main.o: main.cxx 
	${MKDIR} -p build/DebugConf/GNU-Linux-x86
	$(COMPILE.cc) -g -I../../../fltk2 -o build/DebugConf/GNU-Linux-x86/main.o main.cxx

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/DebugConf
	${RM} amax-desktop

# Subprojects
.clean-subprojects:
