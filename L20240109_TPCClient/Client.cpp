#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h> // 소켓 사용을 위하여
#include <string>

#pragma comment(lib, "ws2_32") // 추가 없이 불러오는 방법


using namespace std;

int main()

{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData); //dll을 불러옴//바이트단위로 밀어서 소수점으로 만듬
	if (Result != 0)
	{
		cout << "Winsock dll error" << GetLastError() << endl; // 가지고 있는 마지막 에러코드 표시
		exit(-1);

	}

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.3.17"); // ip주소 적는 곳, 4byte형태로 low하게
	ServerSockAddr.sin_port = htons(40211); // 포트번호 

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP를 0으로 써도 상관은 없다

	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr)); // 랜가드랑 소켓이랑 연결
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}

	char RecvBuffer[1024] = { 0, }; // 최대 버퍼 크기 // 임시저장
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0); //최대 버퍼 길이 //os로 보내줌

	cout << "Sever Sent" << RecvBuffer << endl;


	string Data = RecvBuffer;
	size_t OperatorPosition = Data.find('+');
	if (OperatorPosition == string::npos)
	{
		OperatorPosition = Data.find('-');
		if (OperatorPosition == string::npos)
		{
			OperatorPosition = Data.find('*');
			if (OperatorPosition == string::npos)
			{
				OperatorPosition = Data.find('/');
				if (OperatorPosition == string::npos)
				{
					OperatorPosition = Data.find('%');
				}
			}
		}
	}

	string FirstNumberString = Data.substr(0, OperatorPosition);
	string SecondNumberString = Data.substr(OperatorPosition + 1,
		Data.length() - OperatorPosition - 1);

	cout << FirstNumberString << endl;
	cout << SecondNumberString << endl;

	int FirstNumber = stoi(FirstNumberString);
	int SecondNumber = stoi(SecondNumberString);
	int ResultNumber = 0;
	OperatorPosition = Data.find('+');
	if (OperatorPosition == string::npos)
	{
		OperatorPosition = Data.find('-');
		if (OperatorPosition == string::npos)
		{
			OperatorPosition = Data.find('*');
			if (OperatorPosition == string::npos)
			{
				OperatorPosition = Data.find('/');
				if (OperatorPosition == string::npos)
				{
					OperatorPosition = Data.find('%');
					ResultNumber = FirstNumber % SecondNumber;
				}
				else
				{
					ResultNumber = FirstNumber / +SecondNumber;
				}
			}
			else
			{
				ResultNumber = FirstNumber * SecondNumber;
			}
		}
		else
		{
			ResultNumber = FirstNumber - SecondNumber;
		}
	}
	else
	{
		ResultNumber = FirstNumber + SecondNumber;
	}


	char Buffer[1024] = { 0, };
	sprintf_s(Buffer, 1024, "%d", ResultNumber);

	int SentByte = send(ServerSocket, Buffer, (int)(strlen(Buffer) + 1), 0); //최대 버퍼 길이 //os로 보내줌

	cout << Buffer << endl;

	closesocket(ServerSocket);

	WSACleanup();

	return 0;

}
	
	


	