#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

class ASTAnalyzer : public clang::RecursiveASTVisitor<ASTAnalyzer> {
private:
	std::vector<bool> m_enable;
	std::string m_targetFunctionName;
	
	bool isEnabled() const { return m_enable.back(); }
	
public:
	typedef clang::RecursiveASTVisitor<ASTAnalyzer> Base;
	
	ASTAnalyzer(const std::string &targetFunctionName)
		: m_targetFunctionName(targetFunctionName) {
		m_enable.push_back(false);
	}
	
//	bool VisitCXXRecordDecl(clang::CXXRecordDecl *Declaration) {
//		std::cout << Declaration->getQualifiedNameAsString();
//		return true;
//	}
	
//	bool VisitFunctionDecl(clang::FunctionDecl* D) {
//		if (isEnabled()) {
//			std::string funcName = D->getDeclName().getAsString();
//			std::cout << "Visiting: " << funcName << std::endl;
//		}
//		
//		return true;
//	}
	
	bool VisitCompoundStmt(clang::CompoundStmt *S) {
		//std::cout << "Visiting compound statement, enabled=" << isEnabled() << std::endl;
		return true;
	}
	
	bool TraverseFunctionDecl(clang::FunctionDecl *D) {
		std::string funcName = D->getDeclName().getAsString();

		m_enable.push_back(funcName == m_targetFunctionName);
		bool result = Base::TraverseFunctionDecl(D);
		m_enable.pop_back();
		
		return result;
	}
	
	bool TraverseWhileStmt(clang::WhileStmt *S) {
		if (isEnabled()) {
			// For now, traverse ONLY the body (not the condition)
			std::cout << "while{";
			TraverseStmt(S->getBody());
			std::cout << "}";
		}
		return true;
	}
	
	bool TraverseForStmt(clang::ForStmt *S) {
		if (isEnabled()) {
			// For now, traverse ONLY the body (not the initialization/condition/update)
			std::cout << "for{";
			TraverseStmt(S->getBody());
			std::cout << "}";
		}
		return true;
	}
	
	bool TraverseIfStmt(clang::IfStmt *S) {
		if (isEnabled()) {
			std::cout << "if{";
			TraverseStmt(S->getThen());
			std::cout << "}";
			if (S->getElse()) {
				std::cout << "else{";
				TraverseStmt(S->getElse());
				std::cout << "}";
			}
		}
		return true;
	}

};

class AnalyzeASTConsumer : public clang::ASTConsumer {
public:
	AnalyzeASTConsumer(clang::ASTContext *Context, const std::string &targetFunctionName)
		: m_analyzer(targetFunctionName)
	{ }

	virtual void HandleTranslationUnit(clang::ASTContext &Ctx) {
		//std::cout << "HandleTranslationUnit\n";

		m_analyzer.TraverseDecl(Ctx.getTranslationUnitDecl());
	}

private:
	ASTAnalyzer m_analyzer;
};

class AnalyzeASTAction : public clang::ASTFrontendAction {
private:
	std::string m_targetFunctionName;
	
public:
	AnalyzeASTAction(const std::string &targetFunctionName) : m_targetFunctionName(targetFunctionName) { }
	
	virtual clang::ASTConsumer* CreateASTConsumer(
			clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
		return new AnalyzeASTConsumer(&Compiler.getASTContext(), m_targetFunctionName);
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
		clang::tooling::runToolOnCode(new AnalyzeASTAction("main"), code);
		delete[] code;  // not really necessary, but whatever
	}
	return 0;
}

// vim:ts=4:
