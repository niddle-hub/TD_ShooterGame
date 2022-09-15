#include "SaveSubsystemTypes.h"
#include "SaveSubsystem.h"
#include "Engine/LevelStreaming.h"
#include "TD_ShooterGame/Utils/SGLogUtils.h"

DEFINE_LOG_CATEGORY(LogSaveSubsystem);

FSaveSubsystemArchive::FSaveSubsystemArchive(FArchive& InInnerArchive, bool bInLoadIfFindFails)
	: FObjectAndNameAsStringProxyArchive(InInnerArchive, bInLoadIfFindFails)
{
	ArIsSaveGame = true;
	ArNoDelta = true;
}

FSaveDirectoryVisitor::FSaveDirectoryVisitor(TArray<int32>& InSaveIds)
	: SaveIds(InSaveIds)
{
}

bool FSaveDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (bIsDirectory)
	{
		return true;
	}

	const FString FullFilePath(FilenameOrDirectory);

	FString DirectoryName;
	FString Filename;
	FString FileExtension;
	FPaths::Split(FullFilePath, DirectoryName, Filename, FileExtension);
	if (FileExtension == FileExtensionSave.ToString())
	{
		const int32 SaveId = FCString::Atoi(*Filename);
		if (SaveId > 0)
		{
			SaveIds.AddUnique(SaveId);
		}
	}

	return true;
}

BoolScopeWrapper::BoolScopeWrapper(bool& bInValue, bool bNewValue)
	: bValue(bInValue)
	, bInitialValue(bInValue)
{
	bValue = bNewValue;
}

BoolScopeWrapper::~BoolScopeWrapper()
{
	bValue = bInitialValue;
}

UStreamingLevelObserver::UStreamingLevelObserver()
{
}

void UStreamingLevelObserver::Initialize(USaveSubsystem* InSaveSubsystem, ULevelStreaming* InStreamingLevel)
{
	SG_LOG(LogSaveSubsystem, Display, FString::Printf(TEXT("StreamingLevel %s"), *GetNameSafe(InStreamingLevel)))
	if (!IsValid(InSaveSubsystem) || !IsValid(InStreamingLevel))
	{
		return;
	}

	SaveSubsystem = InSaveSubsystem;
	StreamingLevel = InStreamingLevel;

	StreamingLevel->OnLevelShown.AddUniqueDynamic(this, &UStreamingLevelObserver::OnLevelShown);
	StreamingLevel->OnLevelHidden.AddUniqueDynamic(this, &UStreamingLevelObserver::OnLevelHidden);
}

void UStreamingLevelObserver::Deinitialize()
{
	SG_LOG(LogSaveSubsystem, Display, FString::Printf(TEXT("StreamingLevel %s"), *GetNameSafe(StreamingLevel.Get())))
	
	if (StreamingLevel.IsValid())
	{
		StreamingLevel->OnLevelShown.RemoveDynamic(this, &UStreamingLevelObserver::OnLevelShown);
		StreamingLevel->OnLevelHidden.RemoveDynamic(this, &UStreamingLevelObserver::OnLevelHidden);
	}

	StreamingLevel.Reset();
	SaveSubsystem.Reset();
}

void UStreamingLevelObserver::Serialize(FArchive& Archive)
{
	LevelSaveData.Serialize(Archive);
}

void UStreamingLevelObserver::OnLevelShown()
{
	SG_LOG(LogSaveSubsystem, Display, FString::Printf(TEXT("StreamingLevel %s"), *GetNameSafe(StreamingLevel.Get())))
	if (!SaveSubsystem.IsValid() || !StreamingLevel.IsValid())
	{
		return;
	}

	SaveSubsystem->DeserializeLevel(StreamingLevel->GetLoadedLevel(), StreamingLevel.Get());
}

void UStreamingLevelObserver::OnLevelHidden()
{
	SG_LOG(LogSaveSubsystem, Display, FString::Printf(TEXT("StreamingLevel %s"), *GetNameSafe(StreamingLevel.Get())))
	if (!SaveSubsystem.IsValid() || !StreamingLevel.IsValid())
	{
		return;
	}

	SaveSubsystem->SerializeLevel(StreamingLevel->GetLoadedLevel(), StreamingLevel.Get());
}
