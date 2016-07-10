#ifndef _Runnable_h_ 
#define _Runnable_h_ 

#include "ThreadParameter.h"

class Runnable
{
public:
	virtual void run(ThreadParameter*)=0;
};

#endif