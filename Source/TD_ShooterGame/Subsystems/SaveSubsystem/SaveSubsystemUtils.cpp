#include "SaveSubsystemUtils.h"
#include "SaveSubsystemTypes.h"
#include "SaveSubsystemInterface.h"
#include "TD_ShooterGame/Utils/SGLogUtils.h"

void USaveSubsystemUtils::BroadcastOnLevelDeserialized(ULevel* Level)
{
	SG_LOG(LogSaveSubsystem, Display, FString::Printf(TEXT("%s"), *GetNameSafe(Level)));
	if (!IsValid(Level))
	{
		return;
	}

	for (AActor* Actor : Level->Actors)
	{
		if (IsValid(Actor) && Actor->Implements<USaveSubsystemInterface>())
		{
			ISaveSubsystemInterface::Execute_OnLevelDeserialized(Actor);
		}
	}
}
