#pragma once

enum LogType
{
    /// LogType used for Errors.
    LogType_Error = 0,
    /// LogType used for Asserts. (These indicate an error inside Unity itself.)
    LogType_Assert = 1,
    /// LogType used for Warnings.
    LogType_Warning = 2,
    /// LogType used for regular log messages.
    LogType_Log = 3,
    /// LogType used for Exceptions.
    LogType_Exception = 4,
    /// LogType used for Debug.
    LogType_Debug = 5,
    ///
    LogType_NumLevels
};

inline const char* LogTypeToString (const LogType type)
{
    switch (type)
    {
    case LogType_Assert:    return "Assert";
    case LogType_Debug:     return "Debug";
    case LogType_Exception: return "Exception";
    case LogType_Error:     return "Error";
    case LogType_Log:       return "Log";
    case LogType_Warning:   return "Warning";
    default:                return "";
    }
}

#define AssertIf(x) 				do { (void)sizeof(x); } while(0)
#define AssertIfObject(x,o)			do { (void)sizeof(x); (void)sizeof(o); } while(0)
#define Assert(x) 					do { (void)sizeof(x); } while(0)
#define AssertMsg(x,...)			do { (void)sizeof(x); } while(0)
#define AssertMsgObject(x,o,...)	do { (void)sizeof(x); } while(0)
