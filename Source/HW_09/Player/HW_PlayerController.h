// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW_PlayerController.generated.h"

class UHW_ChatInput;
class UUserWidget;
/**
 * 
 */
UCLASS()
class HW_09_API AHW_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHW_PlayerController();
	
	virtual void BeginPlay() override;

	// 채팅 메세지 설정
	void SetChatMessageString(const FString& InChatMSGString);
	// 채팅 메세지 뷰포트에 출력
	void PrintChatMessageString(const FString& InChatMSGString);

	UFUNCTION(Client, Reliable)	// 클라이언트 RPC (서버 -> 클라이언트 함수 호출)
	void ClientRPCPrintChatMSGString(const FString& InChatMSGString);

	UFUNCTION(Server, Reliable)	// 서버 RPC (클라이언트 -> 서버 함수 호출)
	void ServerRPCPrintChatMSGString(const FString& InChatMSGString);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW_ChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UHW_ChatInput> ChatInputWidgetInstance;

	FString ChatMSGString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
