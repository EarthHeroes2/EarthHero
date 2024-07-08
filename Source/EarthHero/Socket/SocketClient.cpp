// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketClient.h"
#include "Networking/Public/Networking.h"
#include "Sockets/Public/Sockets.h"
#include "Sockets/Public/SocketSubsystem.h"

FString USocketClient::CreateSocket(FString RequestMessage)
{
	FSocket* Socket;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, "TCPSocket", false);
	
	FIPv4Address IP;
	FIPv4Address::Parse("116.121.57.64", IP); //못 감추나

	//FInternetAddr 클래스에 네트워크 정보를 저장?
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(7777);

	bool bConnected = Socket->Connect(*Addr);
	if (bConnected)
	{
		UE_LOG(LogTemp, Log, TEXT("Send Message: %s"), *RequestMessage);
		TCHAR* SerializedChar = RequestMessage.GetCharArray().GetData();
		int32 BytesSent = 0;
		Socket->Send((uint8*)TCHAR_TO_UTF8(SerializedChar), FCString::Strlen(SerializedChar), BytesSent);
		
		uint8 ReceiveBuf[1000];
		int32 BytesRead = 0;
		Socket->Recv(ReceiveBuf, sizeof(ReceiveBuf), BytesRead);

		if (BytesRead > 0)
		{
			FString ReceiveMessage = UTF8_TO_TCHAR((const char*)ReceiveBuf);

			UE_LOG(LogTemp, Log, TEXT("Receive Message: %s"), *ReceiveMessage);
			
			if(RequestMessage.Equals("CreateLobby"))
				return ReceiveMessage; //받은 로비 번호 리턴
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
		return FString();
	}
	
	Socket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);

	return FString();
}