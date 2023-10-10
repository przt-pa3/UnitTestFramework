#include "include/RBTreeTest.hpp"

void SetTestMode(enum TestMode& inTestMode, int& inTestCase, enum PrintMode& inPrintMode, int& inTestTimeLimit)
{
	/*
		TestMode::FullTest		=>	Test full Cases (No Matter TestCase value)
		TestMode::SpecificTest	=>	Test Specific Case (Select Case by Modifying TestCase value)
									inTestCase = 1	=> Do First Test Case
									inTestCase = 2	=> Do Second Test Case
		TestMode::OnlyTestFunc	=>	Don't use Test Case, just call TestFunction()

		PrintMode::EveryCase	=>	Print Pass/Fail of Every Requirement of Specific Unit Test Case
		PrintMode::TotalCase	=>	Print Pass/Fail of Unit Test Case Only

		inTestTimeLimit			=>	Time Limit per Process (Test Case) [ms]
	*/

	inTestMode = TestMode::FullTest;
	inTestCase = 1;
	inPrintMode = PrintMode::TotalCase;
	inTestTimeLimit = 10000;				// [ms]
}

void TestFunction()
{
	/*
		  Do Whatever you want
	*/

	std::cout << "Hello, World" << std::endl;

}

/*
	================================================================================
	********** Do Not Modify Below **********
	================================================================================
*/

void SetFileAndTestName(std::string& ExeFileName, std::string& TestName)
{
	/*
		1) Set EXE_FILE_NAME as your project name
		2) Choose Test Mode by Modifying TestMode Variable
	*/

	ExeFileName = "04-RBTree.exe";
	TestName = "RBTree";
}

void RegisterTests()
{
	// Framework Check
	RegisterTest(module, FrameworkCheck);

	// RBTree Check
	RegisterTest(module, BlackBalance);
	RegisterTest(module, InorderTraversal);
	RegisterTest(module, LeftLean);
	RegisterTest(module, RemoveMax);
	RegisterTest(module, RemoveMin);
	RegisterTest(module, BlackBalanceAfterDeletion);
	RegisterTest(module, InorderTraversalAfterDeletion);

}

int _tmain(int argc, TCHAR* argv[])
{
	std::string EXE_FILE_NAME;
	std::string THIS_TEST_NAME;
	SetFileAndTestName(EXE_FILE_NAME, THIS_TEST_NAME);

	UnitTest::getInstance().setExeName(EXE_FILE_NAME);
	UnitTest::getInstance().setTestName(THIS_TEST_NAME);

	enum TestMode TEST_MODE = TestMode::FullTest;
	int TEST_CASE = 0;
	enum PrintMode PRINT_MODE = PrintMode::TotalCase;
	int TEST_TIMELIMIT = 10000;


	SetTestMode(TEST_MODE, TEST_CASE, PRINT_MODE, TEST_TIMELIMIT);
	UnitTest::getInstance().setPrintMode(PRINT_MODE);
	UnitTest::getInstance().setTimeLimit(TEST_TIMELIMIT);

	if (TEST_MODE == 2)
	{
		_CrtMemState sOld;
		_CrtMemState sNew;
		_CrtMemState sDiff;
		_CrtMemCheckpoint(&sOld);

		std::cout << std::endl;
		std::cout << "TestFunction() Start" << std::endl;
		std::cout << "------------------------------------------------" << std::endl;

		TestFunction();

		_CrtMemCheckpoint(&sNew);
		CheckMemoryState(&sDiff, &sOld, &sNew);
		std::cout << "================================================" << std::endl;
		return 0;
	}

	RegisterTests();

	if (TEST_MODE == 1)
	{
		if (TEST_CASE > 0 && TEST_CASE <= UnitTest::getInstance().getTestListSize())
		{
			std::string s = std::to_string(TEST_CASE);
			TCHAR* cz = StringToTCHAR(s);
			PrintTestCaseInfo(TEST_CASE);
			_CrtMemState sOld;
			_CrtMemState sNew;
			_CrtMemState sDiff;
			_CrtMemCheckpoint(&sOld);

			if (UnitTest::getInstance().runTest(cz)) {
				_CrtMemCheckpoint(&sNew);
				if (CheckMemoryState(&sDiff, &sOld, &sNew)) {
					PrintTestCaseResult(TEST_CASE, false);
				}
				else {
					PrintTestCaseResult(TEST_CASE, true);
				}
			}
			else {
				_CrtMemCheckpoint(&sNew);
				CheckMemoryState(&sDiff, &sOld, &sNew);
				PrintTestCaseResult(TEST_CASE, false);
			}
		}
		else
		{
			std::cout << "Wrong TestCase Number, Check TestCase Variable in main.cpp" << std::endl;
			std::cout << std::endl;
			return 0;
		}
	}
	else if (TEST_MODE == 0)
	{
		if (argc < 2) {
			RunTests();
		}
		else if (argc == 2) {
			_CrtMemState sOld;
			_CrtMemState sNew;
			_CrtMemState sDiff;
			_CrtMemCheckpoint(&sOld);

			if (UnitTest::getInstance().runTest(argv[1])) {
				_CrtMemCheckpoint(&sNew);
				if (CheckMemoryState(&sDiff, &sOld, &sNew))
					return 0;
				else
					return 1;
			}
			else {
				_CrtMemCheckpoint(&sNew);
				CheckMemoryState(&sDiff, &sOld, &sNew);
				return 0;
			}
		}
		else {
			std::cout << "Arg is wrong" << std::endl;
			return 0;
		}
	}
	else
	{

	}

	return 0;
}