// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PLAYERCONTROLLER_API OnScreenDebugger
{
public:
	OnScreenDebugger();
	~OnScreenDebugger();

	static void DrawDebugMessage(char* message);
	static void DrawDebugMessage(char* message, bool boolean, int id);
	static void DrawDebugMessage(char* message, FColor colour, int id);
	static void DrawDebugMessage(FString message, FColor colour, int id);
};
