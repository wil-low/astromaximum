#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
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
FC=

# Macros
PLATFORM=GNU-Linux-x86

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Profiling/${PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/evclass.o \
	${OBJECTDIR}/datafile.o \
	${OBJECTDIR}/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pg
CXXFLAGS=-pg

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../swe -lswe

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Profiling.mk mutter2prof

mutter2prof: ${OBJECTFILES}
	${LINK.cc} -pg -o mutter2prof ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/evclass.o: evclass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DANSITZ -I../swe -MMD -MP -MF $@.d -o ${OBJECTDIR}/evclass.o evclass.cpp

${OBJECTDIR}/datafile.o: datafile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DANSITZ -I../swe -MMD -MP -MF $@.d -o ${OBJECTDIR}/datafile.o datafile.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DANSITZ -I../swe -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Profiling
	${RM} mutter2prof

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
