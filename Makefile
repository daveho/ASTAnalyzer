CXX = clang++
LLVM_CXXFLAGS = $(shell llvm-config --cxxflags)
LLVM_LDFLAGS = $(shell llvm-config --ldflags)

SRCS = Main.cpp
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CXX) $(LLVM_CXXFLAGS) -c $< -o $*.o

ASTAnalyzer : $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LLVM_LDFLAGS)

all :
	echo $(LLVM_CXXFLAGS)
