
#include "UI/HW_ChatInput.h"

#include "Components//EditableTextBox.h"
#include "Game/HW_GameStateBase.h"
#include "Player/HW_PlayerController.h"
#include "Player/HW_PlayerState.h"

void UHW_ChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChatInput is Null"))
	}
	// bound 처리
	// EditabelTextBox의 OnTextCommitted 이벤트가 OnChatInputTextCommitted함수가 연결되어있는지 확인
	// true일 시 이미 함수가 바인딩되어있다는 뜻
	// false 는 바인딩 되어있지않으므로 바인딩 로직 실행
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		// 입력이벤트 (델리게이트) 바인딩 처리
		// Committed 이벤트가 발생하면 OnChatInputTextCommitted 함수 호출
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHW_ChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
	
}

void UHW_ChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)	
	{
		
		APlayerController* OwningPC = GetOwningPlayer();
		if (IsValid(OwningPC) == true)
		{
			AHW_PlayerController* OwningHW_PC = Cast<AHW_PlayerController>(OwningPC);	// HW 플레이어 컨트롤러 가져오기
			AHW_PlayerState* OwningHW_PS = OwningHW_PC->GetPlayerState<AHW_PlayerState>();
			if (IsValid(OwningHW_PC) == true && IsValid(OwningHW_PS) == true)
			{
				if (OwningHW_PS->CurrentGuessCount >= OwningHW_PS->MaxGuessCount)
				{
					FString OverTryMSG = OwningHW_PS->GetPlayerInfoStr() + TEXT(" Over Count ");
					OwningHW_PC->SetChatMessageString(OverTryMSG);
					UE_LOG(LogTemp, Error, TEXT("EnterOverCount!"));
					return;
				}

				UE_LOG(LogTemp, Error, TEXT("OnEnter!"));
				OwningHW_PC->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());	// 채팅 입력 후 빈 채팅으로 바꾸기
			}
		}
	}
}

