
#include "windows.h"
#include "shellapi.h"
#include <shlobj.h>
#include <Shlwapi.h>

//static bool SettingsAppBelievesConnected() {
//  nsresult rv;
//  nsCOMPtr<nsIWindowsRegKey> regKey =
//      do_CreateInstance("@mozilla.org/windows-registry-key;1", &rv);
//  if (NS_FAILED(rv)) {
//    return false;
//  }
//
//  rv = regKey->Open(
//      nsIWindowsRegKey::ROOT_KEY_CURRENT_USER,
//      L"SOFTWARE\\Microsoft\\Windows\\Shell\\Associations",
//      nsIWindowsRegKey::ACCESS_READ);
//  if (NS_FAILED(rv)) {
//    return false;
//  }
//
//  uint32_t value;
//  rv = regKey->ReadIntValue(NS_LITERAL_STRING("IsConnectedAtLogon"), &value);
//  if (NS_FAILED(rv)) {
//    return false;
//  }
//
//  return !!value;
//}
//
//
//static bool IsWindowsLogonConnected() {
//  WCHAR userName[UNLEN + 1];
//  DWORD size = ArrayLength(userName);
//  if (!GetUserNameW(userName, &size)) {
//    return false;
//  }
//
//  LPUSER_INFO_24 info;
//  if (NetUserGetInfo(nullptr, userName, 24, (LPBYTE*)&info) != NERR_Success) {
//    return false;
//  }
//  bool connected = info->usri24_internet_identity;
//  NetApiBufferFree(info);
//
//  return connected;
//}

bool IsWindowsBuildOrLater(int aBuild)
{
  static int minBuild = 0;
  static int maxBuild = 0x7FFFFFFF16U;

  if (minBuild >= aBuild) {
    return true;
  }

  if (aBuild >= maxBuild) {
    return false;
  }

  OSVERSIONINFOEX info;
  ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
  info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  info.dwBuildNumber = aBuild;

  DWORDLONG conditionMask = 0;
  VER_SET_CONDITION(conditionMask, VER_BUILDNUMBER, VER_GREATER_EQUAL);

  if (VerifyVersionInfo(&info, VER_BUILDNUMBER, conditionMask)) {
    minBuild = aBuild;
    return true;
  }

  maxBuild = aBuild;
  return false;
}

int LaunchControlPanelDefaultPrograms() {
  // Build the path control.exe path safely
  WCHAR controlEXEPath[MAX_PATH + 1] = {'\0'};
  if (!GetSystemDirectoryW(controlEXEPath, MAX_PATH)) {
    return 0;
  }
  LPCWSTR controlEXE = L"control.exe";
  if (wcslen(controlEXEPath) + wcslen(controlEXE) >= MAX_PATH) {
    return 0;
  }
  if (!PathAppendW(controlEXEPath, controlEXE)) {
    return 0;
  }

  const wchar_t *params = L"control.exe /name Microsoft.DefaultPrograms "
      L"/page pageDefaultProgram\\pageAdvancedSettings?pszAppName=ZoomLinkOpener";
  //wchar_t appRegName;
  //GetAppRegName(appRegName);
  //params.Append(appRegName);
  STARTUPINFOW si = {sizeof(si), 0};
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_SHOWDEFAULT;
  PROCESS_INFORMATION pi = {0};
  if (!CreateProcessW(controlEXEPath, (wchar_t*)params,
                      nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
    return 0;
  }
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return 1;
}



int launch_default_apps_dialog()
{
	if (!IsWindowsBuildOrLater(14965)) {
		// Use the classic Control Panel to work around a bug of older
		// builds of Windows 10.
		return LaunchControlPanelDefaultPrograms();
	}

	IApplicationActivationManager* pActivator;
	HRESULT hr = CoCreateInstance(CLSID_ApplicationActivationManager,
				      NULL,
				      CLSCTX_INPROC,
				      IID_IApplicationActivationManager,
				      (void**)&pActivator);

	if (SUCCEEDED(hr)) {
		DWORD pid = 0;
		hr = pActivator->ActivateApplication(
			L"windows.immersivecontrolpanel_cw5n1h2txyewy"
			L"!microsoft.windows.immersivecontrolpanel",
			L"page=SettingsPageAppsDefaults", AO_NONE, &pid);
		pActivator->Release();
		return SUCCEEDED(hr) ? 1 : 0;
	}

	return 0;
}
