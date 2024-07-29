// Copyright 2024 DME Games

#include "MultiplayerInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

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
	ServerNameToFind = "";
	MySessionName = FName("Co-op Adventure Session Name");
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
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerInstanceSubsystem::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerInstanceSubsystem::OnJoinSessionComplete);
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
	SessionSettings.Set(FName("DME_SERVER01"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
		
}

void UMultiplayerInstanceSubsystem::JoinServer(const FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		PrintString("ServerName cannot be empty");
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SubsystemName == "NULL")
	{
		SessionSearch->bIsLanQuery = true;
	}
	else
	{
		SessionSearch->bIsLanQuery = false;
	}
	SessionSearch->MaxSearchResults = 128;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerInstanceSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	if (WasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UMultiplayerInstanceSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	if (WasSuccessful && bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerInstanceSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	if (WasSuccessful && !ServerNameToFind.IsEmpty())
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		FOnlineSessionSearchResult* CorrectResult = nullptr;
		if (SearchResults.Num() > 0)
		{
			for (FOnlineSessionSearchResult SearchResult : SearchResults)
			{
				if (SearchResult.IsValid())
				{
					FString FoundServerName = "DME_NOSERVER";
					SearchResult.Session.SessionSettings.Get(FName("DME_SERVER01"), FoundServerName);
					if (ServerNameToFind == FoundServerName)
					{
						CorrectResult = &SearchResult;
						const FString Msg = FString::Printf(TEXT("Server found with name %s"), *FoundServerName);
						PrintString(Msg);
						break;
					}
				}
			}

			if (CorrectResult)
            {
				SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
            }
            else
            {
            	OnAttemptServerJoin.Broadcast(false);
            	PrintString("Couldn't find server");
            }
		}
		else
		{
			OnAttemptServerJoin.Broadcast(false);
			PrintString("No servers found");
		}
	}
}

void UMultiplayerInstanceSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (JoinResult == EOnJoinSessionCompleteResult::Success)
	{
		FString ServerIP = "";
		if (SessionInterface->GetResolvedConnectString(SessionName, ServerIP))
		{
			if (APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController())
			{
				PC->ClientTravel(ServerIP, TRAVEL_Absolute);
			}
		}
	}
	else
	{
		OnAttemptServerJoin.Broadcast(false);
		PrintString("On Join Session Complete called but failed");
	}
}
