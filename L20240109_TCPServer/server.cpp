#define _CRT_SECURE_NO_WARINGS

#include <iostream>
#include <WinSock2.h> // 소켓 사용을 위하여
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "ws2_32") // 추가 없이 불러 오는 방법

using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));

	WSAData wsaData; 
	int Result  = WSAStartup(MAKEWORD(2, 2), &wsaData); //dll을 불러옴//바이트단위로 밀어서 소수점으로 만듬
	if (Result != 0)
	{
		cout << "Winsock dll error" << endl; // 가지고 있는 마지막 에러코드를 표시
		exit(-1);

	}

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //UDPTCP쓸꺼야// 연결지향형//소켓에 번호를 줘//
	//파일디스크립터// 컴퓨터 자원//소켓FD//윈도우는 내부구조다르다.
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "INVALID_SOCKET : " << GetLastError()	<< endl;
		exit(-1);

	}

	//밖에서 부루는 주소 ip
	//iptime 네트워크용 리눅스 컴퓨터임 사실//미디어덱//리얼텍//브로드컴//라우터 3com//Cisco//fkdnxlddkfrhflwma//
	//ㅍ6는 2^64//사설Ip/안에서만쓰는거말함//ip도 자원이라 나눠써야함.//모뎀->돌려써야되서 매번바뀜.->게임할때가 문제

	//Whois//icen.org//
	//NIC당 하나//

	
	SOCKADDR_IN ListenSockAddr;
	//port 2bite 숫자로 할당// 1대1연결//Wellknowport//443//RSA2048암호화//숫자키3개2048//푸는게 전재 없는사람은 시간안에 못푸는걸로//443802221//3479,80 플스//
	//porttoscanning//ping 포트는 Icmp//3306MySQL//
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr)); // 0 으로 채운다
	//ZeroMemory(&ListenSocketAddr, sizeof(ListenSockAddr)); //이런게 있다
	ListenSockAddr.sin_family = AF_INET; // AF_INET = address family
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY; // union 매크로로 만들어놈//특정 ip넣어줘야함//주로 config에서작업함.//
	ListenSockAddr.sin_port = htons(40211); //1024 밑으로 많이씀//똑똑할때
	//램카드랑 연결할차레

	Result =  bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));// 바인드를 통해 소켓에 구멍을 뚫고 소켓이 실행될 때마다 바인드된 함수를 실행 
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl; // 가지고 있는 마지막 에러코드 표시
		exit(-1);
	} // 포트당 하나가 베이스	

	Result = listen(ListenSocket, 5); //backlog 대기인원//multithread이후 의미가 없어짐.받는사람이 여러명이라서/
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl; //가지고 있는 마지막 에러코드 표시
		exit(-1);
	}

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSocketAddrLength); //들어온애 정보,tkdlwmsms vhdlsxjfh//실제 통신용 소켓
	//blocking함수라서 들어올때까지 멈춰있음.//실제 일할때는 안씀//
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}


	//문지기에 의해서 연결되는 실제 소켓
	while (true)
	{
		int FirstNumber = rand() % 10000;
		int SecondNumber = rand() % 9999 + 1;
		int OperatorIndex = rand() % 5;
		char Operator[5] = { '+', '-', '*', '/', '%' };

		char Buffer[1024] = { 0, };
		sprintf_s(Buffer, 1024, "%d%c%d", FirstNumber, Operator[OperatorIndex], SecondNumber);
		//		sprintf(Buffer, "%d%c%d", FirstNumber, Operator[OperatorIndex], SecondNumber);
		//buffer overflow 공격;
		//sprintf_버퍼에 저장된 문자열을 넣어준다.

		int SentByte = send(ClientSocket, Buffer, (int)(strlen(Buffer) + 1), 0); // 한번 보내서 못보내면 다시 보낸다
		if (SentByte < 0) // 0 보다 작으면 자료를 못받은것. 에러로 인해서 
		{
			cout << "Error : " << GetLastError() << endl;
			continue;
		}
		else if (SentByte == 0)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
		}
		else
		{
			cout << "Sent byte : " << SentByte << ", " << Buffer << endl;
		}

		char RecvBuffer[1024] = { 0, }; //최대버퍼크기//임시저장//서버에서 0은 끊자는 말
		int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0); //최대버퍼길이//os로 보내줌
		if (RecvByte < 0) // 0보다 작으면 에러로 인해 자료를 못받은것
		{
			cout << "Error : " << GetLastError() << endl;
			continue;
		}
		else if (RecvByte == 0) // 0이면 상대방과 연결이 안된것 (상대방이 나갔다)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
		}
		else
		{
			cout << "Recv byte : " << RecvByte << ", " << RecvBuffer << endl; // 가지고 있다가 보낸다.
		}
	}
	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup(); //End
	// window 전용, 멀티프로세서는 window 지원이 없다

	return 0;
}