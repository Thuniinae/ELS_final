### Define constants ###
OPTIMIZE_FLAG=-O3
DEBUG_FLAG=-g

CXXFLAGS+=$(OPTIMIZE_FLAG) $(DEBUG_FLAG)
CXXFLAGS+=$(USER_DEFINE_FLAGS)
CXXFLAGS+=-MMD -MP -O3
CXXFLAGS+=-fpermissive

LDLIBS+= 
LDFLAGS+= 

### If you want to keep the compilation/linking errors ###
ifeq ($(ERRLOG), 1)
COMPILER_ERROR = 2> compiler.error
LINKER_ERROR = 2> linker.error
else
COMPILER_ERROR = 
LINKER_ERROR = 
endif

#Included headers 
INCLUDE_DIRS = -I. 
INCLUDE_DIRS += -I./common

#Included libraries
LIB_DIRS = -L. 
LIB_DIRS += -L./include

### Definitions ###
executives=sobel
sources= main.cpp Testbench.cpp
objects= $(subst .cpp,.o,$(sources))
dependencies= $(subst .cpp,.d,$(sources))
	
### what to do? ###
all: sobel

sobel: $(objects)
	$(CXX) $(LDFLAGS) $(LIB_DIRS) $(objects) -o $@ $(LINKER_ERROR) $(LDLIBS) 

$(objects):%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(COMPILER_ERROR)

-include $(sources:.cpp=.d)

### phony ###
run: sobel
	./sobel lena_std_short.bmp out.bmp

clean: 
	rm -f $(executives) $(objects) $(dependencies) $(COMPILER_ERROR) $(LINKER_ERROR)
