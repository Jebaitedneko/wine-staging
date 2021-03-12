#include <stdarg.h>

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "winstring.h"
#include "wine/debug.h"
#include "objbase.h"

#include "initguid.h"
#include "activation.h"

#include "windows.foundation.h"
#include "windows.gaming.input.h"

WINE_DEFAULT_DEBUG_CHANNEL(input);

static const char *debugstr_hstring(HSTRING hstr)
{
    const WCHAR *str;
    UINT32 len;
    if (hstr && !((ULONG_PTR)hstr >> 16)) return "(invalid)";
    str = WindowsGetStringRawBuffer(hstr, &len);
    return wine_dbgstr_wn(str, len);
}

struct windows_gaming_input
{
    IActivationFactory IActivationFactory_iface;
    LONG ref;
};

static inline struct windows_gaming_input *impl_from_IActivationFactory(IActivationFactory *iface)
{
    return CONTAINING_RECORD(iface, struct windows_gaming_input, IActivationFactory_iface);
}

static HRESULT STDMETHODCALLTYPE windows_gaming_input_QueryInterface(
        IActivationFactory *iface, REFIID iid, void **out)
{
    struct windows_gaming_input *impl = impl_from_IActivationFactory(iface);
    TRACE("iface %p, iid %s, out %p stub!\n", iface, debugstr_guid(iid), out);

    if (IsEqualGUID(iid, &IID_IUnknown) ||
        IsEqualGUID(iid, &IID_IInspectable) ||
        IsEqualGUID(iid, &IID_IActivationFactory))
    {
        IUnknown_AddRef(iface);
        *out = &impl->IActivationFactory_iface;
        return S_OK;
    }

    FIXME("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(iid));
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE windows_gaming_input_AddRef(
        IActivationFactory *iface)
{
    struct windows_gaming_input *impl = impl_from_IActivationFactory(iface);
    ULONG ref = InterlockedIncrement(&impl->ref);
    TRACE("iface %p, ref %u.\n", iface, ref);
    return ref;
}

static ULONG STDMETHODCALLTYPE windows_gaming_input_Release(
        IActivationFactory *iface)
{
    struct windows_gaming_input *impl = impl_from_IActivationFactory(iface);
    ULONG ref = InterlockedDecrement(&impl->ref);
    TRACE("iface %p, ref %u.\n", iface, ref);
    return ref;
}

static HRESULT STDMETHODCALLTYPE windows_gaming_input_GetIids(
        IActivationFactory *iface, ULONG *iid_count, IID **iids)
{
    FIXME("iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids);
    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE windows_gaming_input_GetRuntimeClassName(
        IActivationFactory *iface, HSTRING *class_name)
{
    FIXME("iface %p, class_name %p stub!\n", iface, class_name);
    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE windows_gaming_input_GetTrustLevel(
        IActivationFactory *iface, TrustLevel *trust_level)
{
    FIXME("iface %p, trust_level %p stub!\n", iface, trust_level);
    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE windows_gaming_input_ActivateInstance(
        IActivationFactory *iface, IInspectable **instance)
{
    FIXME("iface %p, instance %p stub!\n", iface, instance);
    return E_NOTIMPL;
}

static const struct IActivationFactoryVtbl activation_factory_vtbl =
{
    windows_gaming_input_QueryInterface,
    windows_gaming_input_AddRef,
    windows_gaming_input_Release,
    /* IInspectable methods */
    windows_gaming_input_GetIids,
    windows_gaming_input_GetRuntimeClassName,
    windows_gaming_input_GetTrustLevel,
    /* IActivationFactory methods */
    windows_gaming_input_ActivateInstance,
};

static struct windows_gaming_input windows_gaming_input =
{
    {&activation_factory_vtbl},
    0
};

HRESULT WINAPI DllCanUnloadNow(void)
{
    return S_FALSE;
}

HRESULT WINAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **out)
{
    FIXME("clsid %s, riid %s, out %p stub!\n", debugstr_guid(clsid), debugstr_guid(riid), out);
    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllGetActivationFactory(HSTRING classid, IActivationFactory **factory)
{
    TRACE("classid %s, factory %p.\n", debugstr_hstring(classid), factory);
    *factory = &windows_gaming_input.IActivationFactory_iface;
    IUnknown_AddRef(*factory);
    return S_OK;
}
