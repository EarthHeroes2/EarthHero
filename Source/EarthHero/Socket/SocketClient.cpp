// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketClient.h"
#include "Networking/Public/Networking.h"
#include "Sockets/Public/Sockets.h"
#include "Sockets/Public/SocketSubsystem.h"

void SocketClient::CreateSocket()
{
	FSocket* Socket;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, "TCPSocket", false);
	
	FIPv4Address IP;
	FIPv4Address::Parse("116.121.57.64", IP); //못 감추나

	//FInternetAddr 클래스에 네트워크 정보를 저장 ?
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(7777); //임시

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));
	
	bool bConnected = Socket->Connect(*Addr);
	
	if (bConnected)
	{
		FString Message = TEXT("create_session");
		TCHAR* SerializedChar = Message.GetCharArray().GetData(); // 메시지를 TChar 배열로 변환
		int32 Size = FCString::Strlen(SerializedChar);
		int32 Sent = 0;
		
		Socket->Send((uint8*)TCHAR_TO_UTF8(SerializedChar), Size, Sent);

		
		uint8 Response[10]; //서버로 부터 받을 포트 번호 위한 버퍼
		int32 Read = 0;
		
		Socket->Recv(Response, sizeof(Response), Read);

		if (Read > 0)
		{
			// 받은 응답을 문자열로 변환
			FString ResponseString = UTF8_TO_TCHAR((const char*)Response);
			
			UE_LOG(LogTemp, Error, TEXT("Response : %s"), *ResponseString);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
	}
	
	Socket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
}