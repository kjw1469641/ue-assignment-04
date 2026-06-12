// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include  "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{
	
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // 로컬 플레이어 정보 가져오기
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =		
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())	// EnhancedInputSystem을 관리하는 SubSystem 획득
		{
			if (InputMappingContext)	// IMC 객체 있는지 확인
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);	// Subsystem에 최우선으로 추가 -> 0이 순위임
			}
		}
	}

}