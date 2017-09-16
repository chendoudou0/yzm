#include "IApp.h"
#include "autoDress.h" 


class	CApp : public IApp
{
	CAutoDressServer	    ADS_;
	bool				Start_{ false };

protected:

	virtual bool OnStart() override
	{
		Start_ = ADS_.Start();
		return Start_;
	}


	virtual void OnStop() override
	{
		if (Start_)
		{
			ADS_.Stop();
		}
	}

};

int main(int argc, char** argv)
{
	CApp app;

	return app.Run(argc, argv, "UpgradeServer");
}
