// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketClient.h"

#include "Networking/Public/Networking.h"
#include "Sockets/Public/Sockets.h"
#include "Sockets/Public/SocketSubsystem.h"


//필요한 리턴값이 있다면 리턴함
//없다면 FString()리턴ㄴ
FString USocketClient::CreateSocket(const FString& RequestMessage, const FString& ExtraInfo)
{
	FSocket* Socket;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, "TCPSocket", false);
	
	FIPv4Address IP;
	//
	if(RequestMessage.Equals("CreateLobby") ||
		RequestMessage.Equals("ComparePassword") ||
		RequestMessage.Equals("GetPlayerData")) FIPv4Address::Parse("116.121.57.64", IP); //뭘로 감출까
	else if(RequestMessage.Equals("DestroyServer") ||
			RequestMessage.Equals("UpdatePlayerExp") ||
			RequestMessage.Equals("UpdatePassword") ||
			RequestMessage.Equals("GetPlayerLevel")) FIPv4Address::Parse("127.0.0.1", IP);

	
	TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(IP.Value);
	Address->SetPort(7777);

	bool bConnected = Socket->Connect(*Address);
	if (bConnected)
	{
		int32 BytesSent = 0;
		FString SendMessage;
		
		if(!ExtraInfo.IsEmpty()) SendMessage = RequestMessage + "|" + ExtraInfo;
		else SendMessage = RequestMessage;
		
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

			//접속할 서버의 포트번호, 비번 비교 결과 등을 리턴함
			if(RequestMessage.Equals("CreateLobby") || 
				RequestMessage.Equals("ComparePassword") ||
				RequestMessage.Equals("GetPlayerData") ||
				RequestMessage.Equals("GetPlayerLevel")) return ReceiveMessage; 
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
	
	Socket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);

	return FString();
}

TArray<FString> USocketClient::StringTokenizer(FString Str)
{
	TArray<FString> Tokens;
	FString Token;
	
	while (Str.Split(TEXT("|"), &Token, &Str)) Tokens.Add(Token);

	if (!Str.IsEmpty()) Tokens.Add(Str);
	
	return Tokens;
}