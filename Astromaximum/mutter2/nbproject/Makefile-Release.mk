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
	${OBJECTDIR}/evclass.o \
	${OBJECTDIR}/datafile.o \
	${OBJECTDIR}/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-march=pentium3 -fomit-frame-pointer
CXXFLAGS=-march=pentium3 -fomit-frame-pointer

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../swe -lswe

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} mutter2

mutter2: ${OBJECTFILES}
	${LINK.cc} -o mutter2 -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/evclass.o: evclass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -DANSITZ -I../swe -o ${OBJECTDIR}/evclass.o evclass.cpp

${OBJECTDIR}/datafile.o: datafile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -DANSITZ -I../swe -o ${OBJECTDIR}/datafile.o datafile.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -DANSITZ -I../swe -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} mutter2

# Subprojects
.clean-subprojects:
