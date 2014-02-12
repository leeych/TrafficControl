#include "command.h"

std::string Command::GetVerId = "GetVerId";
std::string Command::BeginMonitor = "BeginMonitor";
std::string Command::EndMonitor = "EndMonitor";
std::string Command::GetLampStatus = "GetLampStatus";
std::string Command::GetConfigure = "GetConfigure";
std::string Command::GetEventInfo = "GetEventInfo";
std::string Command::GetTSCtime = "GetTSCTime";
std::string Command::GetNetAddress = "GetNetAddress";
std::string Command::GetDetectInfo = "GetDetectInfo";
std::string Command::GetDriverInfo = "GetDriveBoardInfo";

std::string Command::ClearEventInfo = "ClearEventInfo";

std::string Command::SetConfigure = "SetConfigure";
std::string Command::ClearDetectInfo = "ClearDetectInfo";
std::string Command::HearBeat = "IAMALIVE";

std::string Command::WTLVersion = "WLTCGetVerId";
std::string Command::StepCtrl = "WLTC1STEPEND";
std::string Command::YellowFlashCtrl = "WLTC1YFEND";
std::string Command::AllRedCtrl = "WLTC1REDEND";
std::string Command::AutoCtrl = "WLTC1SOCKEND";
std::string Command::ManualCtrl = "WLTC1SOCKEND";

Command::Command()
{
}
