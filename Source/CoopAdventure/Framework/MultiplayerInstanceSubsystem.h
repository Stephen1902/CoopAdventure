// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MultiplayerInstanceSubsystem.generated.h"

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

private:
	IOnlineSubsystem* OnlineSubsystem;
	FString SubsystemName;

	void OnCreateSessionComplete(FName SessionName, bool bSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bSuccessful);

	bool bCreateServerAfterDestroy;
	FString DestroyServerName;
};
