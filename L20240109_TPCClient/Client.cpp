#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h> // ���� ����� ���Ͽ�
#include <string>

#pragma comment(lib, "ws2_32") // �߰� ���� �ҷ����� ���


using namespace std;

int main()

{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData); //dll�� �ҷ���//����Ʈ������ �о �Ҽ������� ����
	if (Result != 0)
	{
		cout << "Winsock dll error" << GetLastError() << endl; // ������ �ִ� ������ �����ڵ� ǥ��
		exit(-1);

	}

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.3.17"); // ip�ּ� ���� ��, 4byte���·� low�ϰ�
	ServerSockAddr.sin_port = htons(40211); // ��Ʈ��ȣ 

	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP�� 0���� �ᵵ ����� ����

	Result = connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr)); // ������� �����̶� ����
	if (Result == SOCKET_ERROR)
	{
		cout << "can't connect. : " << GetLastError() << endl;
		exit(-1);
	}

	char RecvBuffer[1024] = { 0, }; // �ִ� ���� ũ�� // �ӽ�����
	int RecvByte = recv(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0); //�ִ� ���� ���� //os�� ������

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

	int SentByte = send(ServerSocket, Buffer, (int)(strlen(Buffer) + 1), 0); //�ִ� ���� ���� //os�� ������

	cout << Buffer << endl;

	closesocket(ServerSocket);

	WSACleanup();

	return 0;

}
	
	


	