#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);

__inline DEFINE_LOG_CATEGORY(LogGame);

//Current Class Name + Function Name where this is called!
#define CLASS_FUNC (FString(__FUNCTION__))
  
//Current Line Number in the code where this is called!
#define CODE_LINE (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define CODE_INFO (CLASS_FUNC + " (" + CODE_LINE + ")")

//Log Current Class where this is called + Format message!
#define SG_LOG(CategoryName, Verbosity, Format) UE_LOG(CategoryName, Verbosity, TEXT("%s: %s"), *CLASS_FUNC, *FString(Format))

//Log Current Class and Line Number where this is called + Format message!
#define SG_LOG_LINE(CategoryName, Verbosity, Format) UE_LOG(CategoryName, Verbosity, TEXT("%s: %s"), *CODE_INFO, *FString(Format))
