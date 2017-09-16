#include "IApp.h"
#include<memory>
#include <conio.h>

using namespace std; 


int IApp::Run(int argc, char** argv, const std::string& titile)
{
	if ( OnStart() )
	{
		
	}

	while (_getch() != 'q');

	OnStop();

	return 0;
}
