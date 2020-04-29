
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>

void launch_default_apps_dialog()
{
	IApplicationActivationManager* pActivator;
	HRESULT hr = CoCreateInstance(CLSID_ApplicationActivationManager,
				      nullptr,
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
		return SUCCEEDED(hr) ? NS_OK : NS_ERROR_FAILURE;
	}

	return NS_OK;
}
