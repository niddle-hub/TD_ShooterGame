#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeathInterface.generated.h"

UINTERFACE()
class UDeathInterface : public UInterface
{
	GENERATED_BODY()
};

class TD_SHOOTERGAME_API IDeathInterface
{
	GENERATED_BODY()

public:
	virtual bool IsDead() PURE_VIRTUAL(IDeathInterface::IsDead, return false;);
};
