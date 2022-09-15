#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

namespace SGDataTableUtils
{
	template <typename T>
	T* FindData(const FName& ID, const TCHAR* DataTablePath)
	{
		static const FString ContextString(TEXT("Find data with given ID"));
		const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, DataTablePath);
		if (DataTable == nullptr) return nullptr;
		return DataTable->FindRow<T>(ID, ContextString);
	}
};