#include "stdafx.h"
#include ".\testcasesuite.h"
#include "..\..\src\Utility\Utility.h"

#include "TestUtility.h"


TestCaseSuite::TestCaseSuite(void)
{
}

TestCaseSuite::~TestCaseSuite(void)
{
}

void TestCaseSuite::start(void)
{		
	try{

		Utility::sendMsg( _T("Test Start ..."));
		TestUtility tu;
		tu.start();

	}catch( exception& e){
		Utility::sendMsg( _T( e.what() ) );
	}catch( MyException& myE){
		Utility::sendMsg( _T( myE.what() ) );
	}
}
