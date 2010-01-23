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
AS=as.exe

# Macros
CND_PLATFORM=MinGW-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/relgui.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`sh ../../../fox-1.6.35/fox-config --cflags` 
CXXFLAGS=`sh ../../../fox-1.6.35/fox-config --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../fox-1.6.35/src/.libs `sh ../../../fox-1.6.35/fox-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk relgui_fx.exe

relgui_fx.exe: ${OBJECTFILES}
	g++ -o relgui_fx ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/relgui.o: nbproject/Makefile-${CND_CONF}.mk relgui.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -I../../../fox-1.6.35/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/relgui.o relgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} relgui_fx.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
