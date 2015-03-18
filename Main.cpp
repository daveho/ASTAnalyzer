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
};

class AnalyzeASTAction : public clang::ASTFrontendAction {
public:
	virtual clang::ASTConsumer* CreateASTConsumer(
		clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
		std::cout << "Hello, clang!\n";
		return new AnalyzeASTConsumer(&Compiler.getASTContext());
	}
};

int main(void) {
	std::cout << "Hello, world\n";
	return 0;
}
