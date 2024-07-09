// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketClient.h"

#include "EarthHero/EHGameInstance.h"
#include "Networking/Public/Networking.h"
#include "Sockets/Public/Sockets.h"
#include "Sockets/Public/SocketSubsystem.h"

FString USocketClient::GetLobbyNameAndPrivateState()
{
	FString LobbyName;
	FString IsPrivate;
	
	UWorld* World = GetWorld();
	
	if (World)
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(World->GetGameInstance());
		if(EHGameInstance)
		{
			LobbyName = EHGameInstance->LobbyName;
			if(EHGameInstance->IsCheckedPrivate) IsPrivate = "true";
			else IsPrivate = "false";
			
			UE_LOG(LogTemp, Log, TEXT("Lobby name : %s     Private : %s"), *LobbyName, *IsPrivate);
		}
	}
	return LobbyName + "|" + IsPrivate;
}

FString USocketClient::GetPortNumber()
{
	FString PortNumber;
	UWorld* World = GetWorld();
	
	if (World)
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(World->GetGameInstance());
		if(EHGameInstance)
		{
			PortNumber = EHGameInstance->ServerPortNumber;
			UE_LOG(LogTemp, Log, TEXT("Server port: %s"), *PortNumber);
		}
	}

	return PortNumber;
}

//필요한 리턴값이 있다면 리턴함
//없다면 FString()리턴
FString USocketClient::CreateSocket(FString RequestMessage)
{
	FSocket* Socket;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, "TCPSocket", false);
	
	FIPv4Address IP;
	if(RequestMessage.Equals("CreateLobby")) FIPv4Address::Parse("116.121.57.64", IP); //못 감추나 //
	else if(RequestMessage.Equals("DestroyServer")) FIPv4Address::Parse("127.0.0.1", IP); //임시

	//FInternetAddr 클래스에 네트워크 정보를 저장?
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(7777);

	
	bool bConnected = Socket->Connect(*Addr);
	if (bConnected)
	{
		
		int32 BytesSent = 0;
		FString SendMessage;
		
		if(RequestMessage.Equals("CreateLobby"))
			SendMessage = RequestMessage + "|" + GetLobbyNameAndPrivateState();
		else if(RequestMessage.Equals("DestroyServer"))
			SendMessage = RequestMessage + "|" + GetPortNumber(); 
		
		UE_LOG(LogTemp, Log, TEXT("Send Message: %s"), *SendMessage);

		TCHAR* SerializedChar = SendMessage.GetCharArray().GetData();
		Socket->Send((uint8*)TCHAR_TO_UTF8(SerializedChar), FCString::Strlen(SerializedChar), BytesSent);
		
		uint8 ReceiveBuf[1000];
		int32 BytesRead = 0;
		Socket->Recv(ReceiveBuf, sizeof(ReceiveBuf), BytesRead);

		if (BytesRead > 0)
		{
			FString ReceiveMessage = UTF8_TO_TCHAR((const char*)ReceiveBuf);

			UE_LOG(LogTemp, Log, TEXT("Receive Message: %s"), *ReceiveMessage);
			
			if(RequestMessage.Equals("CreateLobby")) return ReceiveMessage; //ReceiveMessage = 접속할 서버의 포트번호
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
	
	Socket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);

	return FString();
}