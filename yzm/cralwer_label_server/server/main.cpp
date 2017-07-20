#include "IApp.h"
#include "upgradeServer.h" 


class	CApp : public IApp
{
	CUpgradeServer	    US_;
	bool				Start_{ false };

protected:

	virtual bool OnStart() override
	{
		Start_ =  US_.Start();
		return Start_;
	}


	virtual void OnStop() override
	{
		if ( Start_ )
		{
			US_.Stop();
		}
	}

};

int main(int argc, char** argv)
{
    CApp app;

	return app.Run(argc, argv, "UpgradeServer");                              
}
