#include "OnScreenDebugger.h"

OnScreenDebugger::OnScreenDebugger()
{
}

OnScreenDebugger::~OnScreenDebugger()
{
}

void OnScreenDebugger::DrawDebugMessage(char* message, bool boolean, int id)
{
	const auto& colour = boolean ? FColor::Green : FColor::Red;
	const auto& fstring = FString(message);

	DrawDebugMessage(fstring, colour, id);
}

void OnScreenDebugger::DrawDebugMessage(char* message, FColor colour, int id)
{
	const auto& fstring = FString(message);

	DrawDebugMessage(fstring, colour, id);
}

void OnScreenDebugger::DrawDebugMessage(FString message, FColor colour, int id)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(id, 0.5f, colour, message);
	}
}
