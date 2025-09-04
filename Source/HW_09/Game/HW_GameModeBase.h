// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HW_GameModeBase.generated.h"

class AHW_PlayerController;
/**
 * 
 */
UCLASS()
class HW_09_API AHW_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString AnswerNumber();

	bool IsGuessNumberStr(const FString& InNumberStr);	// 번호 추측 함수 (맞을 시 true)

	FString JudgeResult(const FString& AnswerNumber, const FString& InGuessNumber);

	virtual void BeginPlay() override;

	void PrintChatMSGString(AHW_PlayerController* InChattingPC, const FString& InChatMSGString);

	void InCreaseGuessCount(AHW_PlayerController* InPlayerController);

	void ResetGame();

	void JudgeGame(AHW_PlayerController* InPlayerController, int InStrkeCount);

protected:
	FString AnswerNumStr;

	TArray<TObjectPtr<AHW_PlayerController>> AllPC;	// 모든 플레이어 컨트롤 저장(접속한 클라들 저장)
};
