.SUFFIXES = .cpp .o

PROGRAM		= NeuralDigitRecognizer

MAINSRC		= main.cpp
MAINOBJ		= ${MAINSRC:.cpp=.o}

SRCS		=	\
	Console.cpp	\
	HGUDigitImage.cpp	\
	HGULayer.cpp	\
	HGUNeuralNetwork.cpp	\
	HGUDigitUI.cpp
#	HGUDigitUI_solution.cpp


INCLUDDES = \
	Console.h	\
	HGUDigitImage.h	\
	HGUDigitUI.h	\
	HGULayer.h	\
	HGUNeuralNetwork.h

OBJS		= ${SRCS:.cpp=.o}
LIBS		= -lm
CFLAGS		= -Wno-unused-result -O2 -std=c++11 -D_WINDOWS

ifeq ($(ARCH),LINUX)
	CFLAGS		= -Wno-unused-result -O2 -std=c++11 -D_LINUX
endif	#($ARCH), LINUX)

ifeq ($(ARCH),MAC)
	CFLAGS		= -Wno-unused-result -O2 -std=c++11 -D_MAC
endif	# ($(ARCH),MAC)

#DFLAGS		= -g -D_DEBUG
DFLAGS		= -g

#-D__cplusplus
CC		= g++

all: $(PROGRAM)

run:
	$(PROGRAM)

$(MAINOBJ): $(MAINSRC)

$(PROGRAM): $(OBJS) $(MAINOBJ)
	$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) $(MAINOBJ) $(LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDE_PATH) -c $<

depend:
	makedepend $(INCLUDE_PATH) *.cpp

clean:
	rm -f *.o *.*~ $(PROGRAM)

clearall:
	rm -f *.o *.*~ $(PROGRAM)
