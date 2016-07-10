#include "SelectModelServer.h"
#include "MyReceiver.h"
#include "SocketChanel.h"

class MainClass
{
public:
	void start()
	{
		try{
			SocketChanel receiver;
			SelectModelServer server(receiver);
			server.init(8000);
			server.startServer();
		}catch(Exception e)
		{
			e.printException();
		}
	}
};

int main()
{
	MainClass mainClass;
	mainClass.start();
	return 0;
}