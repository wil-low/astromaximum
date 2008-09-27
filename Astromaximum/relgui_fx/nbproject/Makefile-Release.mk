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
CC=
CCC=
CXX=
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Release/MinGW-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o

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
	$(COMPILE.cc) -O3 -s -I../../../../../fltk2 -o ${OBJECTDIR}/main.o main.cxx

${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o: /home/willow/amax/relgui_fx/relgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/home/willow/amax/relgui_fx
	$(COMPILE.cc) -O3 -s -I../../../../../fltk2 -o ${OBJECTDIR}/_ext/home/willow/amax/relgui_fx/relgui.o /home/willow/amax/relgui_fx/relgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} relgui

# Subprojects
.clean-subprojects:

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.${CONF}
