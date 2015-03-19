# This makefile is designed for clang-3.4/llvm-3.4
# under Ubuntu 14.04 (I'm actually using Linux Mint 17.1 MATE
# edition, which is based on Ubuntu 14.04).  Packages can be
# installed with
#
#    sudo apt-get install libclang-3.4-dev clang-3.4 llvm-3.4-dev

CXX = clang++
LLVM_CXXFLAGS = $(shell llvm-config --cxxflags)
LLVM_LDFLAGS = $(shell llvm-config --ldflags)

CXXFLAGS = -Wall $(LLVM_CXXFLAGS)

# This is all of the clang libraries.  I did not attempt to
# pare down this list.
CLANG_LIBS = \
	-lclang \
	-lclangAnalysis \
	-lclangApplyReplacements \
	-lclangARCMigrate \
	-lclangAST \
	-lclangASTMatchers \
	-lclangBasic \
	-lclangCodeGen \
	-lclangDriver \
	-lclangDynamicASTMatchers \
	-lclangEdit \
	-lclangFormat \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangIndex \
	-lclangLex \
	-lclangParse \
	-lclangQuery \
	-lclangRewriteCore \
	-lclangRewriteFrontend \
	-lclangSema \
	-lclangSerialization \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangStaticAnalyzerFrontend \
	-lclangTidy \
	-lclangTidyGoogleModule \
	-lclangTidyLLVMModule \
	-lclangTooling

# This is the subset of LLVM libraries that seem to be required.
LLVM_LIBS = \
	-lLLVMSupport \
	-lLLVMOption \
	-lLLVMTransformUtils \
	-lLLVMBitReader \
	-lLLVMMCParser \
	-lLLVMMC \
	-lLLVMCore

LIBS = -Wl,--start-group $(CLANG_LIBS) $(LLVM_LIBS) -Wl,--end-group

SRCS = Main.cpp
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $*.o

ASTAnalyzer : $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LLVM_LDFLAGS) $(LIBS)

clean :
	rm -f *.o ASTAnalyzer
