// Fill out your copyright notice in the Description page of Project Settings.

#include "miSoMadness/Public/MiSoGameInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "OnlineSessionSettings.h"

UMiSoGameInstance::UMiSoGameInstance()
{

}
void UMiSoGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//Bind Delegates here
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMiSoGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMiSoGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMiSoGameInstance::OnJoinSessionComplete);
		}
	}
}

void UMiSoGameInstance::OnCreateSessionComplete(FName ServerName, bool isSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Success: %d"), isSuccessful);
	if (isSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPersonBP/Maps/RunMap?listen");
	}
}

void UMiSoGameInstance::OnFindSessionComplete(bool isSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Success: %d"), isSuccessful);
	if (isSuccessful)
	{
		TArray<FOnlineSessionSearchResult> SearchResult = SessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("SearchResult, Sever Count: %d"), SearchResult.Num());
		if (SearchResult.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			SessionInterface->JoinSession(0, "MySession", SearchResult[0]);
		}

	}
}

void UMiSoGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete, SessionName: %s"), *SessionName.ToString());
	if (APlayerController* player = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			player->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}

	}
}

void UMiSoGameInstance::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bShouldAdvertise = true;
	//need to change to false for steam stuff
	sessionSettings.bIsLANMatch = true;
	sessionSettings.bUsesPresence = true;
	sessionSettings.NumPublicConnections = 5;
	SessionInterface->CreateSession(0, FName("MySession"), sessionSettings);
}

void UMiSoGameInstance::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

}
