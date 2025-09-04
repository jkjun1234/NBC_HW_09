// HW_PlayerController.cpp

#include "Player/HW_PlayerController.h"

#include "HW_09.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/HW_ChatInput.h"
#include "EngineUtils.h"
#include "HW_PlayerState.h"
#include "Game/HW_GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AHW_PlayerController::AHW_PlayerController()
{
	bReplicates = true;
}

void AHW_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// OwningClient 판별
	// IsLocalController 가 아니라면 return
	// IsLocalController 가 ture 이면 즉 자기 자신의 플레이어 컨트롤러라면 위젯을 추가함
	// 이렇게 하므로 자기 위젯만 보이고 다른 사람의 위젯은 표시하지 않음
	if (IsLocalController() == false)
	{
		return;
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UHW_ChatInput>(this, ChatInputWidgetClass);
		
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationWidgetClass) == true)
	{
		NotificationWidgetInstance = CreateWidget<UUserWidget>(this, NotificationWidgetClass);
		if (IsValid(NotificationWidgetInstance) == true)
		{
			NotificationWidgetInstance->AddToViewport();	// NotificationWidget 표시
		}
	}
}

void AHW_PlayerController::SetChatMessageString(const FString& InChatMSGString)
{
	ChatMSGString = InChatMSGString;
	// PrintChatMessageString(ChatMSGString);	// 화면에 출력 이상태론 자기 자신의 뷰포트에서만 확인됨(RPC 되지않아서 그럼)

	if (IsLocalController() == true)	// 메세지 설정하는 컨트롤러가 로컬(클라이언트)라면 
	{
		AHW_PlayerState* HWPS = GetPlayerState<AHW_PlayerState>();
		if (IsValid(HWPS) == true)
		{
			FString CombinedMSGStr = HWPS->GetPlayerInfoStr() + TEXT(": ") + InChatMSGString;	// 플레이어 번호 + 입력 숫자 저장

			ServerRPCPrintChatMSGString(CombinedMSGStr);	//ServerRPC 요청 (클라->서버)
		}
	}
}

void AHW_PlayerController::PrintChatMessageString(const FString& InChatMSGString)
{
	// UKismetSystemLibrary::PrintString(this, ChatMSGString, true, true, FLinearColor::Red, 5.f);
	
	// FString NetModeStr = ChatXFunctionLibrary::GetNetModeString(this);	// NetMode 문자열 얻기
	// FString CombinedMSGStr = FString::Printf(TEXT("%s : %s"), *NetModeStr, *InChatMSGString);	// NetMode 에 따른 ChatMsg 저장
	
	ChatXFunctionLibrary::MyPrintString(this, InChatMSGString, 10.f);
}

void AHW_PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);	// 공지사항글은 전체 동일하게 복제되어 보여지도록 설정
}

// Client -> Server 요청
void AHW_PlayerController::ServerRPCPrintChatMSGString_Implementation(const FString& InChatMSGString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AHW_GameModeBase* HWGM = Cast<AHW_GameModeBase>(GM);
		if (IsValid(HWGM) == true)
		{
			HWGM->PrintChatMSGString(this, InChatMSGString);
		}
	}
}

void AHW_PlayerController::ClientRPCPrintChatMSGString_Implementation(const FString& InChatMSGString)
{
	PrintChatMessageString(InChatMSGString);	// 클라이언트 RPC는 서버 -> 클라
	// 서버에서 채팅 값을 Set 한 이후 Client RPC를 하여 각 클라이언트에 해당 함수를 실행하도록함
	// 이로써 클라에서 값수정한것을 서버로 전달 및 적용 -> Print할땐 서버에서 적용된 값을 각 클라에 있는 print 함수 호출
}
