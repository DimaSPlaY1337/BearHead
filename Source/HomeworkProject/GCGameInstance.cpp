// Fill out your copyright notice in the Description page of Project Settings.


#include "GCGameInstance.h"

bool UGCGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	//если консольная команда не отработалась, то нужно вызвать этот метод у всех подсистем
	if (!bResult)
	{
		TArray<UGameInstanceSubsystem*> Subsystems = GetSubsystemArray<UGameInstanceSubsystem>();
		for (UGameInstanceSubsystem* Subsystem : Subsystems)
		{
			bResult |= Subsystem->ProcessConsoleExec(Cmd, Ar, Executor);//результат обьединения
		}
	}
	return bResult;
}
