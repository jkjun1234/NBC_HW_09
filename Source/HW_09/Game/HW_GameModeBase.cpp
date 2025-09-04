// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HW_GameModeBase.h"

#include "Game/HW_GameStateBase.h"
#include "Player/HW_Pawn.h"
#include "Player/HW_PlayerController.h"
#include "EngineUtils.h"
#include "Player/HW_PlayerState.h"

void AHW_GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	AHW_PlayerController* HWPC = Cast<AHW_PlayerController>(NewPlayer);
	if (IsValid(HWPC) == true)
	{
		HWPC->NotificationText = FText::FromString(TEXT("Connected to the Game Sever"));
		AllPC.Add(HWPC);

		AHW_PlayerState* HWPS = HWPC->GetPlayerState<AHW_PlayerState>();
		if (IsValid(HWPS) == true)
		{
			HWPS->PlayerNameStr = TEXT("Player") + FString::FromInt(AllPC.Num());
		}

		AHW_GameStateBase* HWGS = GetGameState<AHW_GameStateBase>();
		if (IsValid(HWGS) == true)
		{
			// 모든 클라에게 로그인한 플레이어 번호 출력
			HWGS->MulticastRPCBroadcastLoginMSG(HWPS->PlayerNameStr);
		}
	}
}

FString AHW_GameModeBase::AnswerNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1 ; i <= 9 ; ++i)
	{
		Numbers.Add(i);		//1~9 숫자 저장
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3 ; ++i)
	{
		int32 Idx = FMath::RandRange(0, Numbers.Num() - 1); // 0 ~ 9 사이 난수 생성
		Result.Append((FString::FromInt(Numbers[Idx])));	// 3자릿 수에 임의의 난수 대입
		Numbers.RemoveAt(Idx);	
	}
	
	return Result;
}

bool AHW_GameModeBase::IsGuessNumberStr(const FString& InNumberStr)
{
	bool bCanPlay = false;

	do
	{
		if (InNumberStr.Len() != 3)	// 추측 번호의 길이가 3이 아니라면 로직 실행 X
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberStr)
		{
			if (FChar::IsDigit(C) == false || C == '0')	// 숫자가 아니고 0이라면  정답 숫자로 인식 X
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	}while (false);

	return bCanPlay;
}

FString AHW_GameModeBase::JudgeResult(const FString& AnswerNumber, const FString& InGuessNumber)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (AnswerNumber[i] == InGuessNumber[i])
		{
			StrikeCount++;	// 입력숫자와 인덱스와 정답 숫자 인덱스와 일치하면 스트라이크!
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumber[i]);
			if (AnswerNumber.Contains(PlayerGuessChar))	// 입력 숫자가 정답에 있다면 볼 카운트 증가!
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT!");
	}

	return FString::Printf(TEXT("%d S /%d B"), StrikeCount, BallCount);
}

void AHW_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	AnswerNumStr = AnswerNumber();	// 정답 난수 저장
	UE_LOG(LogTemp, Error, TEXT("Answer Number : %s"), *AnswerNumStr);	// Answer 출력
}

void AHW_GameModeBase::PrintChatMSGString(AHW_PlayerController* InChattingPC, const FString& InChatMSGString)
{
	FString ChatMsg = InChatMSGString;
	int Index = InChatMSGString.Len() - 3;	// 들어온 문자열의 길이에서 3만큼 뺀후 
	FString GuessNumStr = InChatMSGString.RightChop(Index);	// 들어온 문자열의 끝 3자리를 제외한 Index 만큼 문자 제거

	if (IsGuessNumberStr(GuessNumStr) == true)
	{
		FString JudgeResultString = JudgeResult(AnswerNumStr, GuessNumStr);	// 판결결과 저장

		InCreaseGuessCount(InChattingPC);	// 각 클라이언트마다 시도횟수 증가
		for (TActorIterator<AHW_PlayerController> It(GetWorld()); It; ++It)
		{
			AHW_PlayerController* HW_PC = *It;	// HWPC 순환 참조
			if (IsValid(HW_PC) == true)
			{
				FString CombineMSGStr = InChatMSGString + TEXT(" -> ") + JudgeResultString;
				HW_PC->ClientRPCPrintChatMSGString(CombineMSGStr);	// 답을 맞추기위한 입력값과 결과를 출력

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPC, StrikeCount);	// 게임 승패 판정
			}
		}
	}
	else
	{
		for (TActorIterator<AHW_PlayerController> It(GetWorld()); It; ++It)
		{
			AHW_PlayerController* HW_PC = *It;
			if (IsValid(HW_PC) == true)
			{
				HW_PC->ClientRPCPrintChatMSGString(InChatMSGString);	// 정답을 위한 숫자 입력이 아닌경우 출력
			}
		}
	}
}

void AHW_GameModeBase::InCreaseGuessCount(AHW_PlayerController* InPlayerController)
{
	AHW_PlayerState* HWPS = InPlayerController->GetPlayerState<AHW_PlayerState>();
	if (IsValid(HWPS) == true)
	{
		HWPS->CurrentGuessCount++;
	}
}

void AHW_GameModeBase::ResetGame()
{
	AnswerNumStr = AnswerNumber();	// 새로운 난수 저장
	UE_LOG(LogTemp, Error, TEXT("Answer Number : %s"), *AnswerNumStr);	// Answer 출력
	
	for (const auto& HWPC : AllPC)
	{
		AHW_PlayerState* HW_PS = HWPC->GetPlayerState<AHW_PlayerState>();
		if (IsValid(HW_PS) == true)
		{
			HW_PS->CurrentGuessCount = 0;
		}
	}
}

void AHW_GameModeBase::JudgeGame(AHW_PlayerController* InPlayerController, int InStrkeCount)
{
	if (3 == InStrkeCount)
	{
		AHW_PlayerState* HW_PS = InPlayerController->GetPlayerState<AHW_PlayerState>();

		for (const auto& HW_PC : AllPC)
		{
			if (IsValid(HW_PS) == true)
			{
				FString CombinedMSGStr = HW_PS->PlayerNameStr + TEXT(" has Won the Game!");
				HW_PC->NotificationText = FText::FromString(CombinedMSGStr);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& HW_PC : AllPC)
		{
			AHW_PlayerState* HW_PS = HW_PC->GetPlayerState<AHW_PlayerState>();
			if (IsValid(HW_PS) == true)
			{
				if (HW_PS->CurrentGuessCount < HW_PS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (bIsDraw == true)
		{
			for (const auto& HW_PC : AllPC)
			{
				HW_PC->NotificationText = FText::FromString(TEXT("Draw!.."));	// 무승부 출력

				ResetGame();
			}
		}
	}
	
}
