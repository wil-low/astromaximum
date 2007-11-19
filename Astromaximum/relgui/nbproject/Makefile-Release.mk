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
OBJECTDIR=build/Release/MinGW-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/fMain.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`../../../fltk2/fltk2-config --ldflags`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} relgui

relgui: ${OBJECTFILES}
	${LINK.cc} -o relgui -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/main.o main.cxx

${OBJECTDIR}/fMain.o: fMain.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/fMain.o fMain.cxx

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} relgui

# Subprojects
.clean-subprojects:
