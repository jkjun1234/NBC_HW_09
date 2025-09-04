// HW_09.h
// NetMode 확인하기위한 매크로 

// 사용은 해당 헤더를 인클루드한 후
// ChatXFunctionLibrary::GetNetModeStrng(this); 를 사용하여 this 액터의 NetMode의 문자열을 얻을 수 있음
#pragma once

#include "CoreMinimal.h"

class ChatXFunctionLibrary
{
public:
	// String 출력
	// AActor, FString , float, FColor 의 인자를 가짐
	// 어떤 액터에 어떤 문자열을 몇초(float) 무슨색(FColor)로 출력할지 결정
	static void MyPrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 1.f, FColor InColor = FColor::Cyan)
	{
		if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
		{
			if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *InString);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
			}
		}
	}

	// NetMode 확인
	static FString GetNetModeString(const AActor* InWorldContextActor)
	{
		FString NetModeString = TEXT("None");

		if (IsValid(InWorldContextActor) == true)
		{
			ENetMode NetMode = InWorldContextActor->GetNetMode();
			if (NetMode == NM_Client)
			{
				NetModeString = TEXT("Client");
			}
			else
			{
				if (NetMode == NM_Standalone)
				{
					NetModeString = TEXT("StandAlone");
				}
				else
				{
					NetModeString = TEXT("Server");
				}
			}
		}
		
		return NetModeString;
	}

	// NetRole 확인 
	static FString GetRoleString(const AActor* InActor)
	{
		FString RoleString = TEXT("None");

		if (IsValid(InActor) == true)
		{
			FString LocalRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetLocalRole());
			FString RemoteRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetRemoteRole());
			
			RoleString = FString::Printf(TEXT("%s / %s"), *LocalRoleString, *RemoteRoleString);
		}

		return RoleString;
	}
	
};

