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
INCLUDE_DIRS += -I./include

#Included libraries
LIB_DIRS = -L. 
LIB_DIRS += -L./include

### Definitions ###
executives=canny
sources= canny.cpp
objects= $(subst .cpp,.o,$(sources))
dependencies= $(subst .cpp,.d,$(sources))
	
### what to do? ###
$(executives): %: $(objects)
	$(CXX) $(LDFLAGS) $(LIB_DIRS) $^ -o $@ $(LINKER_ERROR) $(LDLIBS) 

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(COMPILER_ERROR)

%.o:%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(COMPILER_ERROR)

-include $(sources:.cpp=.d)

### phony ###
clean: 
	rm -f $(executives) $(objects) $(dependencies) $(COMPILER_ERROR) $(LINKER_ERROR)
