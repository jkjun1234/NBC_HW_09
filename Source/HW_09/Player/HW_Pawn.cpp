// HW_Pawn.cpp


#include "Player/HW_Pawn.h"

#include "HW_09.h"

AHW_Pawn::AHW_Pawn()
{
	PrimaryActorTick.bCanEverTick = false;

}


void AHW_Pawn::BeginPlay()
{
	Super::BeginPlay();

	// Pawn 의 NetRole 로그 확인
	FString NetRoleString = ChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *ChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void AHW_Pawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = ChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *ChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

