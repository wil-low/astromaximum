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
OBJECTDIR=build/Release/${PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`fox-config --cflags` 
CXXFLAGS=`fox-config --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`fox-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk relgui_fx

relgui_fx: ${OBJECTFILES}
	${LINK.cc} -o relgui_fx -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cxx

${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o: /home/willow/amax/relgui_fx/relgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/willow/amax/relgui_fx
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o /home/willow/amax/relgui_fx/relgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} relgui_fx

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
