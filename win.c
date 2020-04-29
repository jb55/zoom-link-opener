
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>

int launch_default_apps_dialog()
{
	IApplicationActivationManager* pActivator;
	HRESULT hr = CoCreateInstance(&CLSID_ApplicationActivationManager,
				      NULL,
				      CLSCTX_INPROC,
				      &IID_IApplicationActivationManager,
				      (void**)&pActivator);

	if (SUCCEEDED(hr)) {
		DWORD pid = 0;
		hr = pActivator->lpVtbl->ActivateApplication(pActivator,
			L"windows.immersivecontrolpanel_cw5n1h2txyewy"
			L"!microsoft.windows.immersivecontrolpanel",
			L"page=SettingsPageAppsDefaults", AO_NONE, &pid);
		pActivator->lpVtbl->Release(pActivator);
		return SUCCEEDED(hr) ? 1 : 0;
	}

	return 1;
}
