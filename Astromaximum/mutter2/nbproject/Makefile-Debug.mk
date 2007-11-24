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
	${OBJECTDIR}/datafile.o \
	${OBJECTDIR}/evclass.o \
	${OBJECTDIR}/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../swe -lswe

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} mutter2

mutter2: ${OBJECTFILES}
	${LINK.cc} -o mutter2 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/datafile.o: datafile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DANSITZ -I../swe -o ${OBJECTDIR}/datafile.o datafile.cpp

${OBJECTDIR}/evclass.o: evclass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DANSITZ -I../swe -o ${OBJECTDIR}/evclass.o evclass.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DANSITZ -I../swe -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} mutter2

# Subprojects
.clean-subprojects:
