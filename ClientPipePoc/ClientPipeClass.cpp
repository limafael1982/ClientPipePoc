#include "stdafx.h"
#include "ClientPipeClass.h"


ClientPipeClass::ClientPipeClass()
{
	this->fSuccess = FALSE;
	this->lpvMessage = TEXT("2");
	this->lpszPipename = TEXT("\\\\.\\pipe\\namedPipeExample");
}


ClientPipeClass::~ClientPipeClass()
{
	this->closeHandle();
}

int ClientPipeClass::tryToOpenPipe()
{
	int retVal = 0;
	while (true)
	{
		this->hPipe = CreateFile(this->lpszPipename,
			GENERIC_READ |
			GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (this->hPipe != INVALID_HANDLE_VALUE)
		{
			std::cout << "[ClientPipeClass] Invalid Handle value " << std::endl;
			retVal = -1;
			break;
		}
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			auto errorValue = static_cast<int>(GetLastError());
			std::cout << "[ClientPipeClass] Could not open pipe error: " << errorValue << std::endl;
			retVal = -1;
			break;
		}

		// wait for instances to be free for 20 seconds:
		if (!WaitNamedPipe(this->lpszPipename, TIMETOWAIT))
		{
			std::cout << "[ClientPipeClass] I couldn't wait any longer to connect " << std::endl;
			retVal = -1;
			break;
		}

		// TODO: check loop-breaking conditions
	}


	return retVal;
}

int ClientPipeClass::sendMessageToServer(int numberToSend)
{
	int retValue = 0;
	this->dwMode = PIPE_READMODE_MESSAGE;
	TCHAR buf[BUFSIZE];
	swprintf_s(buf, BUFSIZE, TEXT("%d"), numberToSend);
	this->fSuccess = SetNamedPipeHandleState(this->hPipe, &dwMode , NULL, NULL);

	if (this->fSuccess != TRUE)
	{
		auto errorValue = static_cast<int>(GetLastError());
		std::cout << "[ClientPipeClass] SetNamedPipeHandleState error: " << errorValue << std::endl;
		retValue = -1;
	}
	else
	{
		this->cbToWrite = (lstrlen(buf) + 1) * sizeof(TCHAR);
		_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, buf);
		this->fSuccess = WriteFile(this->hPipe, buf, this->cbToWrite, &(this->cbWritten), NULL);

		if (this->fSuccess != TRUE)
		{
			std::cout << "[ClientPipeClass] WriteFile to pipe failed." << std::endl;
			return -1;
		}
		std::cout << "[ClientPipeClass] Message sent to the server" << std::endl;
	}

	return retValue;
}

void ClientPipeClass::closeHandle()
{
	CloseHandle(this->hPipe);
}

int ClientPipeClass::readFromServer()
{
	int retVal = 0;
	do
	{
		this->fSuccess = ReadFile(this->hPipe, this->chBuf, (BUFSIZE * sizeof(TCHAR)), &(this->cbRead), NULL);
		if ((this->fSuccess != TRUE) && (GetLastError() != ERROR_MORE_DATA))
		{
			retVal = -1;
			break;
		}
		std::cout << "[ClientPipeClass] reading message from server... " << std::endl;
		std::cout << "[Message from Server]: " << this->chBuf << std::endl;
		_tprintf(TEXT("\"%s\"\n"), this->chBuf);
	} while (!(this->fSuccess));

	if (!fSuccess)
	{
		std::cout << "Message now failed" << std::endl;
		retVal = -1;
	}

	return retVal;
}
