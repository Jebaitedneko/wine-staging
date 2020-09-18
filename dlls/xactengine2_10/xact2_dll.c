/*
 * Copyright (c) 2018 Ethan Lee for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA+ */

#include "config.h"

#include <stdarg.h>
#include <FACT.h>

#define NONAMELESSUNION
#define COBJMACROS

#include "initguid.h"
#include "xact.h"
#include "rpcproxy.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(xact3);

static HINSTANCE instance;

typedef struct _XACTCueImpl {
    IXACTCue IXACTCue_iface;
    FACTCue *fact_cue;
} XACTCueImpl;

static inline XACTCueImpl *impl_from_IXACTCue(IXACTCue *iface)
{
    return CONTAINING_RECORD(iface, XACTCueImpl, IXACTCue_iface);
}

static HRESULT WINAPI IXACTCueImpl_Play(IXACTCue *iface)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)\n", iface);

    return FACTCue_Play(This->fact_cue);
}

static HRESULT WINAPI IXACTCueImpl_Stop(IXACTCue *iface, DWORD dwFlags)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%u)\n", iface, dwFlags);

    return FACTCue_Stop(This->fact_cue, dwFlags);
}

static HRESULT WINAPI IXACTCueImpl_GetState(IXACTCue *iface, DWORD *pdwState)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%p)\n", iface, pdwState);

    return FACTCue_GetState(This->fact_cue, pdwState);
}

static HRESULT WINAPI IXACTCueImpl_Destroy(IXACTCue *iface)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);
    UINT ret;

    TRACE("(%p)\n", iface);

    ret = FACTCue_Destroy(This->fact_cue);
    if (ret != 0)
        WARN("FACTCue_Destroy returned %d\n", ret);
    HeapFree(GetProcessHeap(), 0, This);
    return S_OK;
}

static HRESULT WINAPI IXACTCueImpl_GetChannelMap(IXACTCue *iface,
        LPXACTCHANNELMAP pChannelMap, DWORD BufferSize, LPDWORD pRequiredSize)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);
    FIXME("(%p)->(%p %d %p): stub!\n", This, pChannelMap, BufferSize, pRequiredSize);
    return S_OK;
}

static HRESULT WINAPI IXACTCueImpl_SetChannelMap(IXACTCue *iface,
        LPXACTCHANNELMAP pChannelMap)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);
    FIXME("(%p)->(%p): stub!\n", This, pChannelMap);
    return S_OK;
}

static HRESULT WINAPI IXACTCueImpl_GetChannelVolume(IXACTCue *iface,
        LPXACTCHANNELVOLUME pVolume)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);
    FIXME("(%p)->(%p): stub!\n", This, pVolume);
    return S_OK;
}

static HRESULT WINAPI IXACTCueImpl_SetChannelVolume(IXACTCue *iface,
        LPXACTCHANNELVOLUME pVolume)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);
    FIXME("(%p)->(%p): stub!\n", This, pVolume);
    return S_OK;
}

static HRESULT WINAPI IXACTCueImpl_SetMatrixCoefficients(IXACTCue *iface,
        UINT32 uSrcChannelCount, UINT32 uDstChannelCount,
        float *pMatrixCoefficients)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%u, %u, %p)\n", iface, uSrcChannelCount, uDstChannelCount,
            pMatrixCoefficients);

    return FACTCue_SetMatrixCoefficients(This->fact_cue, uSrcChannelCount,
        uDstChannelCount, pMatrixCoefficients);
}

static XACTVARIABLEINDEX WINAPI IXACTCueImpl_GetVariableIndex(IXACTCue *iface,
        PCSTR szFriendlyName)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%s)\n", iface, szFriendlyName);

    return FACTCue_GetVariableIndex(This->fact_cue, szFriendlyName);
}

static HRESULT WINAPI IXACTCueImpl_SetVariable(IXACTCue *iface,
        XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%u, %f)\n", iface, nIndex, nValue);

    return FACTCue_SetVariable(This->fact_cue, nIndex, nValue);
}

static HRESULT WINAPI IXACTCueImpl_GetVariable(IXACTCue *iface,
        XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE *nValue)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%u, %p)\n", iface, nIndex, nValue);

    return FACTCue_GetVariable(This->fact_cue, nIndex, nValue);
}

static HRESULT WINAPI IXACTCueImpl_Pause(IXACTCue *iface, BOOL fPause)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);

    TRACE("(%p)->(%u)\n", iface, fPause);

    return FACTCue_Pause(This->fact_cue, fPause);
}

static HRESULT WINAPI IXACTCueImpl_GetProperties(IXACTCue *iface,
        XACT_CUE_INSTANCE_PROPERTIES **ppProperties)
{
    XACTCueImpl *This = impl_from_IXACTCue(iface);
    FACTCueInstanceProperties *fProps;
    HRESULT hr;

    TRACE("(%p)->(%p)\n", iface, ppProperties);

    hr = FACTCue_GetProperties(This->fact_cue, &fProps);
    if(FAILED(hr))
        return hr;

    *ppProperties = (XACT_CUE_INSTANCE_PROPERTIES*) fProps;
    return hr;
}

static const IXACTCueVtbl XACTCue_Vtbl =
{
    IXACTCueImpl_Play,
    IXACTCueImpl_Stop,
    IXACTCueImpl_GetState,
    IXACTCueImpl_Destroy,
    IXACTCueImpl_GetChannelMap,
    IXACTCueImpl_SetChannelMap,
    IXACTCueImpl_GetChannelVolume,
    IXACTCueImpl_SetChannelVolume,
    IXACTCueImpl_SetMatrixCoefficients,
    IXACTCueImpl_GetVariableIndex,
    IXACTCueImpl_SetVariable,
    IXACTCueImpl_GetVariable,
    IXACTCueImpl_Pause,
    IXACTCueImpl_GetProperties
};

typedef struct _XACTSoundBankImpl {
    IXACTSoundBank IXACTSoundBank_iface;

    FACTSoundBank *fact_soundbank;
} XACTSoundBankImpl;

static inline XACTSoundBankImpl *impl_from_IXACTSoundBank(IXACTSoundBank *iface)
{
    return CONTAINING_RECORD(iface, XACTSoundBankImpl, IXACTSoundBank_iface);
}

static XACTINDEX WINAPI IXACTSoundBankImpl_GetCueIndex(IXACTSoundBank *iface,
        PCSTR szFriendlyName)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);

    TRACE("(%p)->(%s)\n", This, szFriendlyName);

    return FACTSoundBank_GetCueIndex(This->fact_soundbank, szFriendlyName);
}

static HRESULT WINAPI IXACTSoundBankImpl_GetNumCues(IXACTSoundBank *iface,
        XACTINDEX *pnNumCues)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);

    TRACE("(%p)->(%p)\n", This, pnNumCues);

    return FACTSoundBank_GetNumCues(This->fact_soundbank, pnNumCues);
}

static HRESULT WINAPI IXACTSoundBankImpl_GetCueProperties(IXACTSoundBank *iface,
        XACTINDEX nCueIndex, XACT_CUE_PROPERTIES *pProperties)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);

    TRACE("(%p)->(%u, %p)\n", This, nCueIndex, pProperties);

    return FACTSoundBank_GetCueProperties(This->fact_soundbank, nCueIndex,
            (FACTCueProperties*) pProperties);
}

static HRESULT WINAPI IXACTSoundBankImpl_Prepare(IXACTSoundBank *iface,
        XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset,
        IXACTCue** ppCue)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);
    XACTCueImpl *cue;
    FACTCue *fcue;
    UINT ret;

    TRACE("(%p)->(%u, 0x%x, %u, %p)\n", This, nCueIndex, dwFlags, timeOffset,
            ppCue);

    ret = FACTSoundBank_Prepare(This->fact_soundbank, nCueIndex, dwFlags,
            timeOffset, &fcue);
    if(ret != 0)
    {
        ERR("Failed to CreateCue: %d\n", ret);
        return E_FAIL;
    }

    cue = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*cue));
    if (!cue)
    {
        FACTCue_Destroy(fcue);
        ERR("Failed to allocate XACTCueImpl!");
        return E_OUTOFMEMORY;
    }

    cue->IXACTCue_iface.lpVtbl = &XACTCue_Vtbl;
    cue->fact_cue = fcue;
    *ppCue = &cue->IXACTCue_iface;

    TRACE("Created Cue: %p\n", cue);

    return S_OK;
}

static HRESULT WINAPI IXACTSoundBankImpl_Play(IXACTSoundBank *iface,
        XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset,
        IXACTCue** ppCue)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);
    FIXME("(%p)->(%u, 0x%x, %u, %p): stub!\n", This, nCueIndex, dwFlags, timeOffset,
            ppCue);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTSoundBankImpl_Stop(IXACTSoundBank *iface,
        XACTINDEX nCueIndex, DWORD dwFlags)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);

    TRACE("(%p)->(%u)\n", This, dwFlags);

    return FACTSoundBank_Stop(This->fact_soundbank, nCueIndex, dwFlags);
}

static HRESULT WINAPI IXACTSoundBankImpl_Destroy(IXACTSoundBank *iface)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);
    HRESULT hr;

    TRACE("(%p)\n", This);

    hr = FACTSoundBank_Destroy(This->fact_soundbank);
    HeapFree(GetProcessHeap(), 0, This);
    return hr;
}

static HRESULT WINAPI IXACTSoundBankImpl_GetState(IXACTSoundBank *iface,
        DWORD *pdwState)
{
    XACTSoundBankImpl *This = impl_from_IXACTSoundBank(iface);

    TRACE("(%p)->(%p)\n", This, pdwState);

    return FACTSoundBank_GetState(This->fact_soundbank, pdwState);
}

static const IXACTSoundBankVtbl XACTSoundBank_Vtbl =
{
    IXACTSoundBankImpl_GetCueIndex,
    IXACTSoundBankImpl_GetNumCues,
    IXACTSoundBankImpl_GetCueProperties,
    IXACTSoundBankImpl_Prepare,
    IXACTSoundBankImpl_Play,
    IXACTSoundBankImpl_Stop,
    IXACTSoundBankImpl_Destroy,
    IXACTSoundBankImpl_GetState
};

typedef struct _XACTWaveBankImpl {
    IXACTWaveBank IXACTWaveBank_iface;

    FACTWaveBank *fact_wavebank;
} XACTWaveBankImpl;

static inline XACTWaveBankImpl *impl_from_IXACTWaveBank(IXACTWaveBank *iface)
{
    return CONTAINING_RECORD(iface, XACTWaveBankImpl, IXACTWaveBank_iface);
}

static HRESULT WINAPI IXACTWaveBankImpl_Destroy(IXACTWaveBank *iface)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);
    HRESULT hr;

    TRACE("(%p)\n", This);

    hr = FACTWaveBank_Destroy(This->fact_wavebank);
    HeapFree(GetProcessHeap(), 0, This);
    return hr;
}

static HRESULT WINAPI IXACTWaveBankImpl_GetNumWaves(IXACTWaveBank *iface,
        XACTINDEX *pnNumWaves)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);

    TRACE("(%p)->(%p)\n", This, pnNumWaves);

    return FACTWaveBank_GetNumWaves(This->fact_wavebank, pnNumWaves);
}

static XACTINDEX WINAPI IXACTWaveBankImpl_GetWaveIndex(IXACTWaveBank *iface,
        PCSTR szFriendlyName)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);

    TRACE("(%p)->(%s)\n", This, szFriendlyName);

    return FACTWaveBank_GetWaveIndex(This->fact_wavebank, szFriendlyName);
}

static HRESULT WINAPI IXACTWaveBankImpl_GetWaveProperties(IXACTWaveBank *iface,
        XACTINDEX nWaveIndex, XACT_WAVE_PROPERTIES *pWaveProperties)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);

    TRACE("(%p)->(%u, %p)\n", This, nWaveIndex, pWaveProperties);

    return FACTWaveBank_GetWaveProperties(This->fact_wavebank, nWaveIndex,
            (FACTWaveProperties*) pWaveProperties);
}

static HRESULT WINAPI IXACTWaveBankImpl_Prepare(IXACTWaveBank *iface,
        XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset,
        XACTLOOPCOUNT nLoopCount, IXACTWave** ppWave)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);
    FIXME("(%p)->(0x%x, %u, 0x%x, %u, %p): stub!\n", This, nWaveIndex, dwFlags,
            dwPlayOffset, nLoopCount, ppWave);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTWaveBankImpl_Play(IXACTWaveBank *iface,
        XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset,
        XACTLOOPCOUNT nLoopCount, IXACTWave** ppWave)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);
    FIXME("(%p)->(0x%x, %u, 0x%x, %u, %p): stub!\n", This, nWaveIndex, dwFlags, dwPlayOffset,
            nLoopCount, ppWave);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTWaveBankImpl_Stop(IXACTWaveBank *iface,
        XACTINDEX nWaveIndex, DWORD dwFlags)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);

    TRACE("(%p)->(%u, %u)\n", This, nWaveIndex, dwFlags);

    return FACTWaveBank_Stop(This->fact_wavebank, nWaveIndex, dwFlags);
}

static HRESULT WINAPI IXACTWaveBankImpl_GetState(IXACTWaveBank *iface,
        DWORD *pdwState)
{
    XACTWaveBankImpl *This = impl_from_IXACTWaveBank(iface);

    TRACE("(%p)->(%p)\n", This, pdwState);

    return FACTWaveBank_GetState(This->fact_wavebank, pdwState);
}

static const IXACTWaveBankVtbl XACTWaveBank_Vtbl =
{
    IXACTWaveBankImpl_Destroy,
    IXACTWaveBankImpl_GetNumWaves,
    IXACTWaveBankImpl_GetWaveIndex,
    IXACTWaveBankImpl_GetWaveProperties,
    IXACTWaveBankImpl_Prepare,
    IXACTWaveBankImpl_Play,
    IXACTWaveBankImpl_Stop,
    IXACTWaveBankImpl_GetState
};

typedef struct _XACTEngineImpl {
    IXACTEngine IXACTEngine_iface;

    FACTAudioEngine *fact_engine;

    XACT_READFILE_CALLBACK pReadFile;
    XACT_GETOVERLAPPEDRESULT_CALLBACK pGetOverlappedResult;
    XACT_NOTIFICATION_CALLBACK notification_callback;
} XACTEngineImpl;

typedef struct wrap_readfile_struct {
    XACTEngineImpl *engine;
    HANDLE file;
} wrap_readfile_struct;

static int32_t FACTCALL wrap_readfile(
    void* hFile,
    void* lpBuffer,
    uint32_t nNumberOfBytesRead,
    uint32_t *lpNumberOfBytesRead,
    FACTOverlapped *lpOverlapped)
{
    wrap_readfile_struct *wrap = (wrap_readfile_struct*) hFile;
    return wrap->engine->pReadFile(wrap->file, lpBuffer, nNumberOfBytesRead,
            lpNumberOfBytesRead, (LPOVERLAPPED)lpOverlapped);
}

static int32_t FACTCALL wrap_getoverlappedresult(
    void* hFile,
    FACTOverlapped *lpOverlapped,
    uint32_t *lpNumberOfBytesTransferred,
    int32_t bWait)
{
    wrap_readfile_struct *wrap = (wrap_readfile_struct*) hFile;
    return wrap->engine->pGetOverlappedResult(wrap->file, (LPOVERLAPPED)lpOverlapped,
            lpNumberOfBytesTransferred, bWait);
}

static inline XACTEngineImpl *impl_from_IXACTEngine(IXACTEngine *iface)
{
    return CONTAINING_RECORD(iface, XACTEngineImpl, IXACTEngine_iface);
}

static HRESULT WINAPI IXACTEngineImpl_QueryInterface(IXACTEngine *iface,
        REFIID riid, void **ppvObject)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%s, %p)\n", This, debugstr_guid(riid), ppvObject);

    if(IsEqualGUID(riid, &IID_IUnknown) ||
             IsEqualGUID(riid, &IID_IXACTEngine)){
        *ppvObject = &This->IXACTEngine_iface;
    }
    else
        *ppvObject = NULL;

    if (*ppvObject){
        IUnknown_AddRef((IUnknown*)*ppvObject);
        return S_OK;
    }

    FIXME("(%p)->(%s,%p), not found\n", This, debugstr_guid(riid), ppvObject);

    return E_NOINTERFACE;
}

static ULONG WINAPI IXACTEngineImpl_AddRef(IXACTEngine *iface)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    ULONG ref = FACTAudioEngine_AddRef(This->fact_engine);
    TRACE("(%p)->(): Refcount now %u\n", This, ref);
    return ref;
}

static ULONG WINAPI IXACTEngineImpl_Release(IXACTEngine *iface)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    ULONG ref = FACTAudioEngine_Release(This->fact_engine);

    TRACE("(%p)->(): Refcount now %u\n", This, ref);

    if (!ref)
        HeapFree(GetProcessHeap(), 0, This);
    return ref;
}

static HRESULT WINAPI IXACTEngineImpl_GetRendererCount(IXACTEngine *iface,
        XACTINDEX *pnRendererCount)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%p)\n", This, pnRendererCount);

    return FACTAudioEngine_GetRendererCount(This->fact_engine, pnRendererCount);
}

static HRESULT WINAPI IXACTEngineImpl_GetRendererDetails(IXACTEngine *iface,
        XACTINDEX nRendererIndex, XACT_RENDERER_DETAILS *pRendererDetails)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%d, %p)\n", This, nRendererIndex, pRendererDetails);

    return FACTAudioEngine_GetRendererDetails(This->fact_engine,
            nRendererIndex, (FACTRendererDetails*) pRendererDetails);
}

static HRESULT WINAPI IXACTEngineImpl_GetFinalMixFormat(IXACTEngine *iface,
        WAVEFORMATEXTENSIBLE *pFinalMixFormat)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%p)\n", This, pFinalMixFormat);

    return FACTAudioEngine_GetFinalMixFormat(This->fact_engine,
            (FAudioWaveFormatExtensible*) pFinalMixFormat);
}

static void FACTCALL fact_notification_cb(const FACTNotification *notification)
{
    XACTEngineImpl *engine = (XACTEngineImpl *)notification->pvContext;

    /* Older versions of FAudio don't pass through the context */
    if (!engine)
    {
        WARN("Notification context is NULL\n");
        return;
    }

    if (notification->type == XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED)
    {
        FIXME("Callback XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED\n");
    }
    else
        FIXME("Unsupported callback type %d\n", notification->type);
}

static HRESULT WINAPI IXACTEngineImpl_Initialize(IXACTEngine *iface,
        const XACT_RUNTIME_PARAMETERS *pParams)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FACTRuntimeParameters params;
    UINT ret;

    TRACE("(%p)->(%p)\n", This, pParams);

    params.lookAheadTime = pParams->lookAheadTime;
    params.pGlobalSettingsBuffer = pParams->pGlobalSettingsBuffer;
    params.globalSettingsBufferSize = pParams->globalSettingsBufferSize;
    params.globalSettingsFlags = pParams->globalSettingsFlags;
    params.globalSettingsAllocAttributes = pParams->globalSettingsAllocAttributes;
    params.pRendererID = (INT16 *) pParams->pRendererID;
    params.pXAudio2 = NULL;
    params.pMasteringVoice = NULL;

    /* Force Windows I/O, do NOT use the FACT default! */
    This->pReadFile = (XACT_READFILE_CALLBACK)
            pParams->fileIOCallbacks.readFileCallback;
    This->pGetOverlappedResult = (XACT_GETOVERLAPPEDRESULT_CALLBACK)
            pParams->fileIOCallbacks.getOverlappedResultCallback;
    if (This->pReadFile == NULL)
        This->pReadFile = (XACT_READFILE_CALLBACK) ReadFile;
    if (This->pGetOverlappedResult == NULL)
        This->pGetOverlappedResult = (XACT_GETOVERLAPPEDRESULT_CALLBACK)
                GetOverlappedResult;
    params.fileIOCallbacks.readFileCallback = wrap_readfile;
    params.fileIOCallbacks.getOverlappedResultCallback = wrap_getoverlappedresult;
    params.fnNotificationCallback = fact_notification_cb;

    This->notification_callback = (XACT_NOTIFICATION_CALLBACK)pParams->fnNotificationCallback;

    ret = FACTAudioEngine_Initialize(This->fact_engine, &params);
    if (ret != 0)
        WARN("FACTAudioEngine_Initialize returned %d\n", ret);

    return !ret ? S_OK : E_FAIL;
}

static HRESULT WINAPI IXACTEngineImpl_ShutDown(IXACTEngine *iface)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)\n", This);

    return FACTAudioEngine_ShutDown(This->fact_engine);
}

static HRESULT WINAPI IXACTEngineImpl_DoWork(IXACTEngine *iface)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)\n", This);

    return FACTAudioEngine_DoWork(This->fact_engine);
}

static HRESULT WINAPI IXACTEngineImpl_CreateSoundBank(IXACTEngine *iface,
        const void* pvBuffer, DWORD dwSize, DWORD dwFlags,
        DWORD dwAllocAttributes, IXACTSoundBank **ppSoundBank)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    XACTSoundBankImpl *sb;
    FACTSoundBank *fsb;
    UINT ret;

    TRACE("(%p)->(%p, %u, 0x%x, 0x%x, %p)\n", This, pvBuffer, dwSize, dwFlags,
            dwAllocAttributes, ppSoundBank);

    ret = FACTAudioEngine_CreateSoundBank(This->fact_engine, pvBuffer, dwSize,
            dwFlags, dwAllocAttributes, &fsb);
    if(ret != 0)
    {
        ERR("Failed to CreateSoundBank: %d\n", ret);
        return E_FAIL;
    }

    sb = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*sb));
    if (!sb)
    {
        FACTSoundBank_Destroy(fsb);
        ERR("Failed to allocate XACTSoundBankImpl!");
        return E_OUTOFMEMORY;
    }

    sb->IXACTSoundBank_iface.lpVtbl = &XACTSoundBank_Vtbl;
    sb->fact_soundbank = fsb;
    *ppSoundBank = &sb->IXACTSoundBank_iface;

    TRACE("Created SoundBank: %p\n", sb);

    return S_OK;
}

static HRESULT WINAPI IXACTEngineImpl_CreateInMemoryWaveBank(IXACTEngine *iface,
        const void* pvBuffer, DWORD dwSize, DWORD dwFlags,
        DWORD dwAllocAttributes, IXACTWaveBank **ppWaveBank)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    XACTWaveBankImpl *wb;
    FACTWaveBank *fwb;
    UINT ret;

    TRACE("(%p)->(%p, %u, 0x%x, 0x%x, %p)!\n", This, pvBuffer, dwSize, dwFlags,
            dwAllocAttributes, ppWaveBank);

    ret = FACTAudioEngine_CreateInMemoryWaveBank(This->fact_engine, pvBuffer,
            dwSize, dwFlags, dwAllocAttributes, &fwb);
    if(ret != 0)
    {
        ERR("Failed to CreateWaveBank: %d\n", ret);
        return E_FAIL;
    }

    wb = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*wb));
    if (!wb)
    {
        FACTWaveBank_Destroy(fwb);
        ERR("Failed to allocate XACTWaveBankImpl!");
        return E_OUTOFMEMORY;
    }

    wb->IXACTWaveBank_iface.lpVtbl = &XACTWaveBank_Vtbl;
    wb->fact_wavebank = fwb;
    *ppWaveBank = &wb->IXACTWaveBank_iface;

    TRACE("Created in-memory WaveBank: %p\n", wb);

    return S_OK;
}

static HRESULT WINAPI IXACTEngineImpl_CreateStreamingWaveBank(IXACTEngine *iface,
        const XACT_WAVEBANK_STREAMING_PARAMETERS *pParms,
        IXACTWaveBank **ppWaveBank)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FIXME("(%p)->(%p, %p): stub!\n", This, pParms, ppWaveBank);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTEngineImpl_PrepareInMemoryWave(IXACTEngine *iface,
        DWORD dwFlags, WAVEBANKENTRY entry, DWORD *pdwSeekTable,
        BYTE *pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount,
        IXACTWave **ppWave)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FIXME("(%p): stub!\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTEngineImpl_PrepareStreamingWave(IXACTEngine *iface,
        DWORD dwFlags, WAVEBANKENTRY entry,
        XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment,
        DWORD *pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount,
        IXACTWave **ppWave)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FIXME("(%p): stub!\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTEngineImpl_PrepareWave(IXACTEngine *iface,
        DWORD dwFlags, PCSTR szWavePath, WORD wStreamingPacketSize,
        DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount,
        IXACTWave **ppWave)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FIXME("(%p): stub!\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTEngineImpl_RegisterNotification(IXACTEngine *iface,
        const XACT_NOTIFICATION_DESCRIPTION *pNotificationDesc)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FIXME("(%p)->(%p): stub!\n", This, pNotificationDesc);
    return E_NOTIMPL;
}

static HRESULT WINAPI IXACTEngineImpl_UnRegisterNotification(IXACTEngine *iface,
        const XACT_NOTIFICATION_DESCRIPTION *pNotificationDesc)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);
    FIXME("(%p)->(%p): stub!\n", This, pNotificationDesc);
    return E_NOTIMPL;
}

static XACTCATEGORY WINAPI IXACTEngineImpl_GetCategory(IXACTEngine *iface,
        PCSTR szFriendlyName)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%s)\n", This, szFriendlyName);

    return FACTAudioEngine_GetCategory(This->fact_engine, szFriendlyName);
}

static HRESULT WINAPI IXACTEngineImpl_Stop(IXACTEngine *iface,
        XACTCATEGORY nCategory, DWORD dwFlags)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%u, 0x%x)\n", This, nCategory, dwFlags);

    return FACTAudioEngine_Stop(This->fact_engine, nCategory, dwFlags);
}

static HRESULT WINAPI IXACTEngineImpl_SetVolume(IXACTEngine *iface,
        XACTCATEGORY nCategory, XACTVOLUME nVolume)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%u, %f)\n", This, nCategory, nVolume);

    return FACTAudioEngine_SetVolume(This->fact_engine, nCategory, nVolume);
}

static HRESULT WINAPI IXACTEngineImpl_Pause(IXACTEngine *iface,
        XACTCATEGORY nCategory, BOOL fPause)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%u, %u)\n", This, nCategory, fPause);

    return FACTAudioEngine_Pause(This->fact_engine, nCategory, fPause);
}

static XACTVARIABLEINDEX WINAPI IXACTEngineImpl_GetGlobalVariableIndex(
        IXACTEngine *iface, PCSTR szFriendlyName)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%s)\n", This, szFriendlyName);

    return FACTAudioEngine_GetGlobalVariableIndex(This->fact_engine,
            szFriendlyName);
}

static HRESULT WINAPI IXACTEngineImpl_SetGlobalVariable(IXACTEngine *iface,
        XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%u, %f)\n", This, nIndex, nValue);

    return FACTAudioEngine_SetGlobalVariable(This->fact_engine, nIndex, nValue);
}

static HRESULT WINAPI IXACTEngineImpl_GetGlobalVariable(IXACTEngine *iface,
        XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE *nValue)
{
    XACTEngineImpl *This = impl_from_IXACTEngine(iface);

    TRACE("(%p)->(%u, %p)\n", This, nIndex, nValue);

    return FACTAudioEngine_GetGlobalVariable(This->fact_engine, nIndex, nValue);
}

static const IXACTEngineVtbl XACTEngine_Vtbl =
{
    IXACTEngineImpl_QueryInterface,
    IXACTEngineImpl_AddRef,
    IXACTEngineImpl_Release,
    IXACTEngineImpl_GetRendererCount,
    IXACTEngineImpl_GetRendererDetails,
    IXACTEngineImpl_GetFinalMixFormat,
    IXACTEngineImpl_Initialize,
    IXACTEngineImpl_ShutDown,
    IXACTEngineImpl_DoWork,
    IXACTEngineImpl_CreateSoundBank,
    IXACTEngineImpl_CreateInMemoryWaveBank,
    IXACTEngineImpl_CreateStreamingWaveBank,
    IXACTEngineImpl_PrepareWave,
    IXACTEngineImpl_PrepareInMemoryWave,
    IXACTEngineImpl_PrepareStreamingWave,
    IXACTEngineImpl_RegisterNotification,
    IXACTEngineImpl_UnRegisterNotification,
    IXACTEngineImpl_GetCategory,
    IXACTEngineImpl_Stop,
    IXACTEngineImpl_SetVolume,
    IXACTEngineImpl_Pause,
    IXACTEngineImpl_GetGlobalVariableIndex,
    IXACTEngineImpl_SetGlobalVariable,
    IXACTEngineImpl_GetGlobalVariable
};

void* XACT_Internal_Malloc(size_t size)
{
    return CoTaskMemAlloc(size);
}

void XACT_Internal_Free(void* ptr)
{
    return CoTaskMemFree(ptr);
}

void* XACT_Internal_Realloc(void* ptr, size_t size)
{
    return CoTaskMemRealloc(ptr, size);
}

static HRESULT WINAPI XACTCF_QueryInterface(IClassFactory *iface, REFIID riid, void **ppobj)
{
    if(IsEqualGUID(riid, &IID_IUnknown) ||
       IsEqualGUID(riid, &IID_IClassFactory))
    {
        *ppobj = iface;
        return S_OK;
    }

    *ppobj = NULL;
    WARN("(%p)->(%s, %p): interface not found\n", iface, debugstr_guid(riid), ppobj);
    return E_NOINTERFACE;
}

static ULONG WINAPI XACTCF_AddRef(IClassFactory *iface)
{
    return 2;
}

static ULONG WINAPI XACTCF_Release(IClassFactory *iface)
{
    return 1;
}

static HRESULT WINAPI XACTCF_CreateInstance(IClassFactory *iface, IUnknown *pOuter,
                                               REFIID riid, void **ppobj)
{
    HRESULT hr;
    XACTEngineImpl *object;

    TRACE("(%p)->(%p,%s,%p)\n", iface, pOuter, debugstr_guid(riid), ppobj);

    *ppobj = NULL;

    if(pOuter)
        return CLASS_E_NOAGGREGATION;

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if(!object)
        return E_OUTOFMEMORY;

    object->IXACTEngine_iface.lpVtbl = &XACTEngine_Vtbl;

    FACTCreateEngineWithCustomAllocatorEXT(
        0,
        &object->fact_engine,
        XACT_Internal_Malloc,
        XACT_Internal_Free,
        XACT_Internal_Realloc
    );

    hr = IXACTEngine_QueryInterface(&object->IXACTEngine_iface, riid, ppobj);
    if(FAILED(hr)){
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    return hr;
}

static HRESULT WINAPI XACTCF_LockServer(IClassFactory *iface, BOOL dolock)
{
    TRACE("(%p)->(%d): stub!\n", iface, dolock);
    return S_OK;
}

static const IClassFactoryVtbl XACTCF_Vtbl =
{
    XACTCF_QueryInterface,
    XACTCF_AddRef,
    XACTCF_Release,
    XACTCF_CreateInstance,
    XACTCF_LockServer
};

static IClassFactory XACTFactory = { &XACTCF_Vtbl };

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD reason, void *pReserved)
{
    TRACE("(%p, %d, %p)\n", hinstDLL, reason, pReserved);

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        instance = hinstDLL;
        DisableThreadLibraryCalls( hinstDLL );

#ifdef HAVE_FAUDIOLINKEDVERSION
        TRACE("Using FAudio version %d\n", FAudioLinkedVersion() );
#endif

        break;
    }
    return TRUE;
}

HRESULT WINAPI DllCanUnloadNow(void)
{
    return S_FALSE;
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    if (IsEqualGUID(rclsid, &CLSID_XACTEngine))
    {
        TRACE("(%s, %s, %p)\n", debugstr_guid(rclsid), debugstr_guid(riid), ppv);
        return IClassFactory_QueryInterface(&XACTFactory, riid, ppv);
    }

    FIXME("Unknown class %s\n", debugstr_guid(rclsid));
    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllRegisterServer(void)
{
    return __wine_register_resources(instance);
}

HRESULT WINAPI DllUnregisterServer(void)
{
    return __wine_unregister_resources(instance);
}
