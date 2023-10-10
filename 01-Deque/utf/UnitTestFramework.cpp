#include "utf/UnitTestFramework.hpp"

TCHAR* StringToTCHAR(std::string& s)
{
	tstring tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

const std::string TCHARToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];

	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

BOOL RunProcess(std::string& sExeName, int testNum) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD exit_code;
	bool flag = false;
	int TimeOut_ms = 10000;
	TimeOut_ms = UnitTest::getInstance().getTimeLimit();

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	TCHAR* szCmd;

	std::string sCmd;
	sCmd += sExeName;
	sCmd += " ";
	sCmd += std::to_string(testNum);

	szCmd = StringToTCHAR(sCmd);
	// Start the child process. 

	if (!CreateProcess(NULL,   // No module name (use command line)
		szCmd,        // Command line
		NULL,           // Process handle not inheritable - PKO
		NULL,           // Thread handle not inheritable - TKO
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		std::cout << " ** Failed to Create Process ** " << std::endl;
		return FALSE;
	}

	// Wait until child process exits.
	// MAX 10 Sec
	WaitForSingleObject(pi.hProcess, TimeOut_ms);

	GetExitCodeProcess(pi.hProcess, &exit_code);

	if (exit_code == 1) flag = true;
	else if (exit_code == 259) {
		std::cout << std::endl << std::endl;
		std::cout << "\033[1;31m" << "        TIMEOUT!! " << "\033[0m" << " - Time Limit : " << TimeOut_ms << "(ms)" << std::endl << std::endl;
	}
	else if (exit_code == 2 || exit_code == 0) {

	}
	else {
		std::cout << std::endl << std::endl;
		std::cout << "\033[1;31m" <<"           Exception Thrown !!" << "\033[0m" << std::endl;
		std::cout << "           e.g.) Read Access Violation " << std::endl << std::endl;
		// std::cout << "exit_code : " << exit_code << std::endl;
	}

	// Close process and thread handles.   
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (flag)
		return TRUE;
	else
		return FALSE;
}

bool CheckMemoryState(_CrtMemState* memDiff, _CrtMemState* memOld, _CrtMemState* memNew) {
	if (_CrtMemDifference(memDiff, memOld, memNew)) {
		std::cout << std::endl << std::endl;
		std::cout << "\033[1;31m" << "          MEMORY LEAK DETECTED" << "\033[0m" << std::endl;
		return true;
	}
	else {
		std::cout << std::endl << std::endl;
		std::cout << "             NO MEMORY LEAK     " << std::endl;
		return false;
	}
}

void PintTestInfo(std::string testname) {
	std::cout << std::endl
		<< "************************************************" << std::endl
		<< "        " << testname << " Test Starts" << std::endl
		<< "************************************************" << std::endl;
}

void PrintTestResult(int passed, int failed) {
	if (failed == 0)
		std::cout << "\033[1;32m" << "            TEST PASSED" << "\033[0m";
	else
		std::cout << "\033[1;31m" << "            TEST FAILED" << "\033[0m";

	std::cout << "    (" << passed << " / " << passed + failed << ")" << std::endl;
}

void PrintTestCaseInfo(int TestCaseNum) {
	std::cout << std::endl;
	std::cout << "Test Case " << TestCaseNum << " - [";

}

void PrintTestCaseName(const char* TestName) {
	std::cout << TestName << "]  Start" << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
}

void PrintTestCaseResult(int TestCaseNum, bool TestCaseResult) {
	//std::cout << std::endl;
	std::cout << "                                Test Case " << TestCaseNum;
	if (TestCaseResult) std::cout << "\033[1;32m" << " Pass" << "\033[0m" << std::endl;
	else std::cout << "\033[1;31m" << " Fail" << "\033[0m" << std::endl;
	std::cout << "================================================" << std::endl;
}

void PrintSpecificCase(int STestCaseNum) {
	if (STestCaseNum % 5 == 1 && STestCaseNum != 1) {
		std::cout << std::endl;
	}

	if (STestCaseNum % 5 != 1)
	{
		std::cout << "   ";

		if (STestCaseNum < 10)
			std::cout << " ";
		if (STestCaseNum < 100)
			std::cout << " ";
	}

	std::cout << "(" << STestCaseNum << ") ";
}

void UnitTest::setExeName(std::string s) {
	EXE_NAME = s;
}

void UnitTest::setTestName(std::string s) {
	TEST_NAME = s;
}

UnitTest& UnitTest::getInstance() {
	static UnitTest uTest;
	return uTest;
}

int UnitTest::getTimeLimit()
{
	return TimeLimit;
}

void UnitTest::setTimeLimit(int InTimeLimit)
{
	TimeLimit = InTimeLimit;
}

int UnitTest::getTestListSize() {
	return testList.size();
}

void UnitTest::pushTest(UnitTest* TestCase) {
	testList.push_back(TestCase);
}

void UnitTest::setPrintMode(PrintMode inPrintMode)
{
	if (inPrintMode == PrintMode::EveryCase)
	{
		PrintEveryCase = true;
	}
	else if (inPrintMode == PrintMode::Summary)
	{
		PrintEveryCase = false;
	}
}

bool UnitTest::expectTrue(bool result) {
	if (isFirstCase) {
		PrintTestCaseName(testName);
		isFirstCase = false;
	}

	if (isTrue) isTrue = result;
	if (getInstance().PrintEveryCase)
	{
		PrintSpecificCase(SpecificCase);
		if (!result) {
			std::cerr << "F";
		}
		else {
			std::cerr << "P";
		}
	}

	if (result)
	{
		getInstance().PassedSpecificCase++;
	}
	else
	{
		getInstance().FailedSpecificCase++;
	}

	SpecificCase++;
	return isTrue;
}

int UnitTest::getPassedFunc()
{
	return getInstance().PassedSpecificCase;
}

int UnitTest::getFailedFunc()
{
	return getInstance().FailedSpecificCase;
}

void UnitTest::runTests() {
	static int passed = 0;
	static int failed = 0;

	PintTestInfo(TEST_NAME);

	if (testList.empty()) {
		std::cout << "Test is not Registered" << std::endl;
		return;
	}

	int i = 1;

	for (UnitTest* testCase : testList) {
		PrintTestCaseInfo(i);
		if (!RunProcess(EXE_NAME, i)) {
			PrintTestCaseResult(i, false);
			failed++;
		}
		else {
			PrintTestCaseResult(i, true);
			passed++;
		}

		++i;
	}
	PrintTestResult(passed, failed);
}

bool UnitTest::runTest(TCHAR* t) {
	std::list<UnitTest*>::iterator iter = testList.begin();
	std::string s_number = TCHARToString(t);
	int index = std::stoi(s_number) - 1;
	std::advance(iter, index);

	(*iter)->runFunc();
	if ((*iter)->isTrue) { // pass
		return true;
	}
	else { // fail
		return false;
	}
}