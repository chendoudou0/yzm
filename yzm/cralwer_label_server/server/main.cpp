#include "IApp.h"
#include "poseLabelServer.h" 


class	CApp : public IApp
{
	CPoseLabelServer	    PS_;
	bool				Start_{ false };

protected:

	virtual bool OnStart() override
	{
		Start_ =  PS_.Start();
		return Start_;
	}


	virtual void OnStop() override
	{
		if ( Start_ )
		{
			PS_.Stop();
		}
	}

};

int main(int argc, char** argv)
{
    CApp app;

	return app.Run(argc, argv, "poseLabelServer");                              
}
