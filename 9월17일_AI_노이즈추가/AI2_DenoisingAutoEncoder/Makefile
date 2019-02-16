.SUFFIXES = .cpp .cu .o

PROGRAM		= DenoisingAutoEncoder

MAINSRC		= main.cpp
MAINOBJ		= ${MAINSRC:.cpp=.o}

SRCS		=	\
	Console.cpp	\
	HGUDigitImage.cpp	\
	HGULayer.cpp	\
	HGUNeuralNetwork.cpp	\
	HGUAutoEncoderUI.cpp	
#	HGUAutoEncoderUI_solution.cpp


INCLUDDES = \
	Console.h	\
	HGUDigitImage.h	\
	HGUAutoEncoderUI.h	\
	HGULayer.h	\
	HGUNeuralNetwork.h

OBJS		= ${SRCS:.cpp=.o}
LIBS		= -lm

CFLAGS		= -Wno-unused-result -O2 -std=c++11 

ifeq ($(ARCH),LINUX)
    CFLAGS      = -Wno-unused-result -O2 -std=c++11 -D_LINUX
endif   #($ARCH), LINUX)

ifeq ($(ARCH),MAC)
    CFLAGS      = -Wno-unused-result -O2 -std=c++11 -D_MAC
endif   # ($(ARCH),MAC)

#DFLAGS		= -g -D_DEBUG
DFLAGS		= -g 

#-D__cplusplus
CC		= g++
#NVCC	= nvcc

INCLUDE_PATH	= -I../../JpegLib -I/usr/local/cuda/include
LIB_PATH= -L. -L/usr/local/cuda/lib64

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
	rm -f *.o *.*~ $(PROGRAM) $(PROGRAM_CUDA) $(LIB)
clearall:
	rm -f *.o *.*~ $(PROGRAM) $(PROGRAM_CUDA) $(LIB)
