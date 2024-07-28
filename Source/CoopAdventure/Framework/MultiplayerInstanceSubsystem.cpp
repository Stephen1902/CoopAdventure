// Copyright 2024 DME Games

#include "MultiplayerInstanceSubsystem.h"
#include "OnlineSessionSettings.h"

void PrintString(const FString& StringIn)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, StringIn);
	}
}

UMultiplayerInstanceSubsystem::UMultiplayerInstanceSubsystem()
{
	PrintString(TEXT("UMIS Constructor"));

	OnlineSubsystem = nullptr;
	bCreateServerAfterDestroy = false;
	DestroyServerName = "";
}

void UMultiplayerInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerInstanceSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerInstanceSubsystem::OnDestroySessionComplete);
		}
	}
}

void UMultiplayerInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogTemp, Warning, TEXT("UMIS Deinitialize"));
}

void UMultiplayerInstanceSubsystem::CreateServer(const FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		PrintString(TEXT("Server name cannot be empty"));
		return;
	}

	FName MySessionName = FName("Co-op Adventure Session Name");

	// Check if a session already exists, destroy it if it does
	if (const FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName))
	{
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;  // Whether or not to allow players to join mid-session
	SessionSettings.bIsDedicated = false;  // Whether dedicated or a listen server
	SessionSettings.bShouldAdvertise = true;  // Whether the game should advertise itself
	SessionSettings.NumPublicConnections = 2;  // The number of players that can be in the game in total
	SessionSettings.bUseLobbiesIfAvailable = true;  // Must be here for session to work
	SessionSettings.bUsesPresence = true;  // Steam uses presence.  Must be true
	SessionSettings.bAllowJoinViaPresence = true;
	if (SubsystemName == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
		
}

void UMultiplayerInstanceSubsystem::JoinServer(const FString ServerName)
{
	PrintString(ServerName);
}

void UMultiplayerInstanceSubsystem::OnCreateSessionComplete(FName SessionName, bool bSuccessful)
{
	if (bSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UMultiplayerInstanceSubsystem::OnDestroySessionComplete(FName SessionName, bool bSuccessful)
{
	if (bSuccessful && bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}
