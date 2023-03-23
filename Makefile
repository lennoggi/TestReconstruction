# Include and parse the option list, but don't do that when running 'make clean'
ifeq (, ${filter clean, ${MAKECMDGOALS}})
	include ${options}

	# Disable optimization if so requested by the user
	ifeq (${OPTIMIZE}, no)
		CXX_OPTIMIZE_FLAGS =
	else ifneq (${OPTIMIZE}, yes)
		$(error Set option OPTIMIZE to either yes or no in the optionlist (current setting: ${OPTIMIZE}))
	endif

	# Disable warnings if so requested by the user
	ifeq (${WARN}, no)
		CXX_WARN_FLAGS =
	else ifneq (${WARN}, yes)
		$(error Set option WARN to either yes or no in the optionlist (current setting: ${WARN}))
	endif

	# Disable debug mode if so requested by the user
	ifeq (${DEBUG}, no)
		CXX_DEBUG_FLAGS =
	else ifneq (${DEBUG}, yes)
		$(error Set option DEBUG to either yes or no in the optionlist (current setting: ${DEBUG}))
	endif
endif


# icpx generates *.o.tmp files which should be removed when cleaning
ifeq (${CXX}, icpx)
	ICPX_OTMP = *.o.tmp
else
	ICPX_OTMP =
endif


# Objects to be built
OBJ = Main.o TestFunctions.o PPM.o
EXE = TestReconstruction_exe


# Build all targets
build: ${OBJ}
	${CXX} -o ${EXE} ${OBJ} ${CXXFLAGS} ${CXX_OPTIMIZE_FLAGS} ${CXX_WARN_FLAGS} ${CXX_DEBUG_FLAGS} ${LDFLAGS}

Main.o: Main.cc Parameters.hh Declarations.hh
	${CXX} -c Main.cc ${CXXFLAGS} ${CXX_OPTIMIZE_FLAGS} ${CXX_WARN_FLAGS} ${CXX_DEBUG_FLAGS} ${LDFLAGS}

TestFunctions.o: TestFunctions.cc Parameters.hh Declarations.hh
	${CXX} -c TestFunctions.cc ${CXXFLAGS} ${CXX_OPTIMIZE_FLAGS} ${CXX_WARN_FLAGS} ${CXX_DEBUG_FLAGS} ${LDFLAGS}

PPM.o: PPM.cc Parameters.hh Declarations.hh Helpers.hh
	${CXX} -c PPM.cc ${CXXFLAGS} ${CXX_OPTIMIZE_FLAGS} ${CXX_WARN_FLAGS} ${CXX_DEBUG_FLAGS} ${LDFLAGS}

.PHONY : clean
clean:
	${RM} ${EXE} ${OBJ} ${ICPX_OTMP}
