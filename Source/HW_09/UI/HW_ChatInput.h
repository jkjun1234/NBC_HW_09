// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "HW_ChatInput.generated.h"


class UEditableTextBox;
/**
 * 
 */
UCLASS()
class HW_09_API UHW_ChatInput : public UUserWidget
{
	GENERATED_BODY()

public:
	// View 포트에 추가될때 자동호출
	virtual void NativeConstruct() override;

	// View 포트에서 제거될때 자동호출
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
};
