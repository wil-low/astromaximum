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
CC=
CCC=
CXX=
FC=
AS=

# Macros
CND_PLATFORM=GNU-Windows
CND_CONF=Release
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
CCFLAGS=`fox-config --cflags` 
CXXFLAGS=`fox-config --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`fox-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk relgui_fx.exe

relgui_fx.exe: ${OBJECTFILES}
	${LINK.cc} -o relgui_fx -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/relgui.o: nbproject/Makefile-${CND_CONF}.mk relgui.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/relgui.o relgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} relgui_fx.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
