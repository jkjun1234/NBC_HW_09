// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HW_PlayerState.h"

#include "Net/UnrealNetwork.h"

AHW_PlayerState::AHW_PlayerState()
	: PlayerNameStr(TEXT("None")),
	CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;	// 해당 클래스에 대하여 리플리케이트 활성화(모든 클라에 복제됨)
	// 속성(변수들)에 대해선 계속해서 업데이트 되지않음 
}

void AHW_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameStr);	// 프로퍼티 리플리케이션 값이 변할때마다 복제
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
}

FString AHW_PlayerState::GetPlayerInfoStr()
{
	// 플레이어 번호 시도횟수/ 최대 횟수 문자열 저장 
	FString PlayerInfoStr = PlayerNameStr + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoStr;	// 플레이어 정보 반환
}
