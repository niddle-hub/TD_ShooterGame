#include "AnimNotify_EnableRagdoll.h"
#include "TD_ShooterGame/TD_ShooterGameTypes.h"

void UAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	MeshComp->SetCollisionProfileName(CollisionProfile::Ragdoll);
	MeshComp->SetSimulatePhysics(true);
}
