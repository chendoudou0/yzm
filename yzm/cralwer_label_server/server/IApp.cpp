#include "IApp.h"
#include"monitor.h"

#include<memory>
#include<signal.h>
#include<unistd.h>

using namespace std; 

shared_ptr<Monitor>  gPtrMonitor(new Monitor);

void SignalCallback(int num) 
{ 
	gPtrMonitor->Notify();
}

void IApp::waitForSignal()
{
	gPtrMonitor->Wait();
}

int IApp::Run(int argc, char** argv, const std::string& titile)
{
	if ( OnStart() )
	{
		signal(SIGTERM,  SignalCallback);            
		waitForSignal();
	}

	OnStop();

	return 0;
}
