
#include "stdafx.h"
#include "ClientPipeClass.h"


int main()
{
	ClientPipeClass * cpc = new ClientPipeClass();
	cpc->tryToOpenPipe();
	cpc->sendMessageToServer();
	cpc->readFromServer();
	delete cpc;

	_getch();


	return 0;
}
