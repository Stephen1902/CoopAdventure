// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttemptServerJoin, bool, Successful);

/**
 * 
 */
UCLASS()
class COOPADVENTURE_API UMultiplayerInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerInstanceSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	IOnlineSessionPtr SessionInterface;

	UFUNCTION(BlueprintCallable, Category = "Multiplayer Subsystem")
	void CreateServer(const FString ServerName);

	UFUNCTION(BlueprintCallable, Category = "Multiplayer Subsystem")
	void JoinServer(const FString ServerName);

	UPROPERTY(BlueprintAssignable)
	FOnAttemptServerJoin OnAttemptServerJoin;

private:
	class IOnlineSubsystem* OnlineSubsystem;
	FString SubsystemName;
	FName MySessionName;

	void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);
	void OnFindSessionsComplete(bool WasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	bool bCreateServerAfterDestroy;
	FString DestroyServerName;
	FString ServerNameToFind;

	TSharedPtr<FOnlineSessionSearch> SessionSearch; 
};
