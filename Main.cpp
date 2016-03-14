#include <iostream>
#include <fstream>
#include <memory>

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

class ASTAnalyzer : public clang::RecursiveASTVisitor<ASTAnalyzer> {
public:
	bool VisitCXXRecordDecl(clang::CXXRecordDecl *Declaration) {
		std::cout << Declaration->getQualifiedNameAsString();
		return true;
	}

};

class AnalyzeASTConsumer : public clang::ASTConsumer {
public:
	AnalyzeASTConsumer(clang::ASTContext *Context) { }

	virtual void HandleTranslationUnit(clang::ASTContext &Ctx) {
		std::cout << "HandleTranslationUnit\n";

		m_analyzer.TraverseDecl(Ctx.getTranslationUnitDecl());
	}

private:
	ASTAnalyzer m_analyzer;
};

class AnalyzeASTAction : public clang::ASTFrontendAction {
public:
	virtual clang::ASTConsumer* CreateASTConsumer(
			clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
		return new AnalyzeASTConsumer(&Compiler.getASTContext());
	}
};

char *loadFileIntoMemory(const char *fileName) {
	// Shamelessly copied from:
	// http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
	std::ifstream t;
	int length;
	t.open(fileName);      // open input file
	t.seekg(0, std::ios::end);    // go to the end
	length = t.tellg();           // report location (this is the length)
	t.seekg(0, std::ios::beg);    // go back to the beginning
	char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
	t.read(buffer, length);       // read the whole file into the buffer
	t.close();
	return buffer;
}

int main(int argc, char **argv) {
	if (argc > 1) {
		char *code = loadFileIntoMemory(argv[1]);
		clang::tooling::runToolOnCode(new AnalyzeASTAction(), code);
		delete[] code;  // not really necessary, but whatever
	}
	return 0;
}

// vim:ts=4:
