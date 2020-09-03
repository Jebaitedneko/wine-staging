#include <stdarg.h>

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "winstring.h"
#include "wine/debug.h"

#include "objbase.h"
#include "initguid.h"

#include "activation.h"

WINE_DEFAULT_DEBUG_CHANNEL(network);

static const char *debugstr_hstring(HSTRING hstr)
{
    const WCHAR *str;
    UINT32 len;
    if (hstr && !((ULONG_PTR)hstr >> 16)) return "(invalid)";
    str = WindowsGetStringRawBuffer(hstr, &len);
    return wine_dbgstr_wn(str, len);
}

HRESULT WINAPI DllGetClassObject(REFCLSID clsid, REFIID riid, LPVOID *object)
{
    FIXME("clsid %s, riid %s, object %p stub!\n", debugstr_guid(clsid), debugstr_guid(riid), object);
    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllGetActivationFactory(HSTRING classid, IActivationFactory **factory)
{
    FIXME("classid %s, factory %p stub!\n", debugstr_hstring(classid), factory);
    return E_NOINTERFACE;
}