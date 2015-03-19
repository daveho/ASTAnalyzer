#include <iostream>
#include <memory>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

class AnalyzeASTConsumer : public clang::ASTConsumer {
public:
	AnalyzeASTConsumer(clang::ASTContext *Context) { }

	virtual void HandleTranslationUnit(clang::ASTContext &Ctx) {
		std::cout << "HandleTranslationUnit\n";
	}
};

class AnalyzeASTAction : public clang::ASTFrontendAction {
public:
	virtual clang::ASTConsumer* CreateASTConsumer(
			clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
		return new AnalyzeASTConsumer(&Compiler.getASTContext());
	}
};

int main(int argc, char **argv) {
	if (argc > 1) {
		clang::tooling::runToolOnCode(new AnalyzeASTAction(), argv[1]);
	}
	return 0;
}

// vim:ts=4:
