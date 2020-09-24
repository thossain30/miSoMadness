// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MiSoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MISOMADNESS_API UMiSoGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
//Constructor
	UMiSoGameInstance();

protected:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	virtual void Init() override;
	//when create session is complete call this function
	virtual void OnCreateSessionComplete(FName ServerName, bool isSuccessful);
	//when find session is complete call this function
	virtual void OnFindSessionComplete(bool isSuccessful);
	//when join session is complete call this function
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	UFUNCTION(BlueprintCallable)
		void HostServer();
	UFUNCTION(BlueprintCallable)
		void JoinServer();
};
