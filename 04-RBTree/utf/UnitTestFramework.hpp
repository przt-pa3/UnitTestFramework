#ifndef __UNITTESTFRAMEWORK_H_
#define __UNITTESTFRAMEWORK_H_

#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <list>
#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
typedef std::basic_string<TCHAR> tstring;
TCHAR* StringToTCHAR(std::string& s);
const std::string TCHARToString(const TCHAR* ptsz);
BOOL RunProcess(std::string& sExeName, int testNum);
bool CheckMemoryState(_CrtMemState* memDiff, _CrtMemState* memOld, _CrtMemState* memNew);
void PrintTestCaseInfo(int TestCaseNum);
void PrintTestCaseResult(int TestCaseNum, bool TestCaseResult);

enum TestMode
{
	Full = 0,
	Specific,
	RunTestFunction
};

enum PrintMode
{
	Summary = 0,
	EveryCase
};

class UnitTest {
	UnitTest() {}
protected:
	UnitTest(const char* module, const char* test) : moduleName(module), testName(test) {}

private:
	const char* moduleName;
	const char* testName;

	bool isTrue{ true };
	bool isFirstCase = true;
	int SpecificCase = 1;
	bool PrintEveryCase = false;

	int PassedSpecificCase = 0;
	int FailedSpecificCase = 0;

	std::string TEST_NAME;
	std::string EXE_NAME;
	int TimeLimit = 10000;

	std::list<UnitTest*> testList;

public:
	void setExeName(std::string s);
	void setTestName(std::string s);
	static UnitTest& getInstance();

	int getTimeLimit();
	void setTimeLimit(int InTimeLimit);

	int getTestListSize();
	void pushTest(UnitTest* TestCase);

	void setPrintMode(PrintMode inPrintMode);

	bool expectTrue(bool result);
	int getPassedFunc();
	int getFailedFunc();

	void runTests();
	bool runTest(TCHAR* t);
	virtual void runFunc() {}

	virtual ~UnitTest() {}
};



#define DeclareTest(Module, TestName)								\
	class Test_##Module##_##TestName : public UnitTest				\
{																	\
	Test_##Module##_##TestName() : UnitTest(#Module, #TestName) {}	\
	public:															\
		static Test_##Module##_##TestName* getInstance() {			\
			static Test_##Module##_##TestName testClass;			\
			return &testClass;										\
		}															\
		void runFunc() override;									\
};																

#define DefineTest(Module, TestName)							\
	void Test_##Module##_##TestName::runFunc()					\

//#define RegisterTest(Module, TestName)						\
//	UnitTest::getInstance().testList.push_back(					\
//		Test_##Module##_##TestName::getInstance());				\

#define RegisterTest(Module, TestName)							\
	UnitTest::getInstance().pushTest(							\
		Test_##Module##_##TestName::getInstance());				\

#define RunTests()												\
	UnitTest::getInstance().runTests();							


#endif // __UNITTESTFRAMEWORK_H_