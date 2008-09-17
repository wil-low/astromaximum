#
# Gererated Makefile - do not edit!
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

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/GNU-Linux-x86

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
.build-conf: ${BUILD_SUBPROJECTS} relgui_fx

relgui_fx: ${OBJECTFILES}
	g++ -o relgui_fx ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -o ${OBJECTDIR}/main.o main.cxx

${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o: /home/willow/amax/relgui_fx/relgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/willow/amax/relgui_fx
	$(COMPILE.cc) -g -o ${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o /home/willow/amax/relgui_fx/relgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} relgui_fx

# Subprojects
.clean-subprojects:

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.${CONF}
