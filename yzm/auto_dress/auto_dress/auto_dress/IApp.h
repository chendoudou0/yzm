#ifndef _IAPP_H
#define _IAPP_H

#include <string>

class	IApp
{
public:

	IApp() = default;

	virtual ~IApp() = default;

public:

	int	Run(int argc, char** argv, const std::string& titile = "");

protected:

	virtual bool	OnStart() = 0;

	virtual void	OnStop()  = 0;


};

#endif