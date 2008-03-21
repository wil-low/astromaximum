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
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Release/MinGW-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AspEditUI.o \
	${OBJECTDIR}/LibUI.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/ChronoUI.o \
	${OBJECTDIR}/MainWindow.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`../../../fltk-2.0.x-r5864/fltk2-config --ldflags`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} amax-desktop

amax-desktop: ${OBJECTFILES}
	${LINK.cc} -o amax-desktop -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/AspEditUI.o: AspEditUI.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/AspEditUI.o AspEditUI.cxx

${OBJECTDIR}/LibUI.o: LibUI.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/LibUI.o LibUI.cxx

${OBJECTDIR}/main.o: main.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/main.o main.cxx

${OBJECTDIR}/ChronoUI.o: ChronoUI.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/ChronoUI.o ChronoUI.cxx

${OBJECTDIR}/MainWindow.o: MainWindow.cxx 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O3 -s -I../../../fltk2 -o ${OBJECTDIR}/MainWindow.o MainWindow.cxx

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} amax-desktop

# Subprojects
.clean-subprojects:
