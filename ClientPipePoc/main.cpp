
#include "stdafx.h"
#include "ClientPipeClass.h"


int main()
{
	std::unique_ptr<ClientPipeClass> cpc(new ClientPipeClass());
	cpc->tryToOpenPipe();
	cpc->sendMessageToServer(10);
	cpc->readFromServer();

	_getch();


	return 0;
}
