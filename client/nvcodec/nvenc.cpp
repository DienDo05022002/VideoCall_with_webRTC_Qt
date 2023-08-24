#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "nvenc.h"
#include <cstdint>
#include <string>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <dxgi1_2.h>

struct nvenc_data
{
	Microsoft::WRL::ComPtr<ID3D11Device>        device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGIAdapter>        adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory1>       factory;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>     texture;

	HANDLE inputHandle = NULL;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> inputTexture;
	Microsoft::WRL::ComPtr<IDXGIKeyedMutex> keyedMutex;

	std::mutex mutex;
	uint32_t width;
	uint32_t height;
	uint32_t framerate;
	uint32_t bitrate;
	uint32_t gop;
	std::string codec;
	DXGI_FORMAT format;
	NvEncoderD3D11 *nvenc = nullptr;
};

static bool is_supported(void)
{
	static HMODULE hModule = NULL;

	if (hModule == NULL) {
#if defined(_WIN64)
		hModule = LoadLibrary(TEXT("nvEncodeAPI64.dll"));
#elif defined(_WIN32)
		hModule = LoadLibrary(TEXT("nvEncodeAPI.dll"));
#endif
	}

	if (hModule == NULL) {
		printf("[nvenc] Error: NVENC library file is not found. Please ensure NV driver is installed. \n");
		return false;
	}

	typedef NVENCSTATUS(NVENCAPI *NvEncodeAPIGetMaxSupportedVersion_Type)(uint32_t*);
#if defined(_WIN32)
	NvEncodeAPIGetMaxSupportedVersion_Type NvEncodeAPIGetMaxSupportedVersion = (NvEncodeAPIGetMaxSupportedVersion_Type)GetProcAddress(hModule, "NvEncodeAPIGetMaxSupportedVersion");
#else
	NvEncodeAPIGetMaxSupportedVersion_Type NvEncodeAPIGetMaxSupportedVersion = (NvEncodeAPIGetMaxSupportedVersion_Type)dlsym(hModule, "NvEncodeAPIGetMaxSupportedVersion");
#endif

	uint32_t version = 0;
	uint32_t currentVersion = (NVENCAPI_MAJOR_VERSION << 4) | NVENCAPI_MINOR_VERSION;
	NVENC_API_CALL(NvEncodeAPIGetMaxSupportedVersion(&version));
	if (currentVersion > version) {
		printf("[nvenc] Error: Current Driver Version does not support this NvEncodeAPI version, please upgrade driver");
		return false;
	}

	return true;
}

static void* nvenc_create()
{
	if (!is_supported()) {
		return nullptr;
	}

	struct nvenc_data *enc = new nvenc_data;

	HRESULT hr = S_OK;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)enc->factory.GetAddressOf());
	for (int gpuIndex = 0; gpuIndex <= 1; gpuIndex++) {
		hr = enc->factory->EnumAdapters(gpuIndex, enc->adapter.GetAddressOf());
		if (FAILED(hr)) {
			goto failed;
		}
		else {
			char desc[128] = { 0 };
			DXGI_ADAPTER_DESC adapterDesc;
			enc->adapter->GetDesc(&adapterDesc);
			wcstombs(desc, adapterDesc.Description, sizeof(desc));
			if (strstr(desc, "NVIDIA") == NULL) {
				continue;
			}
		}

		hr = D3D11CreateDevice(enc->adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, NULL, 0, D3D11_SDK_VERSION,
							enc->device.GetAddressOf(), NULL, enc->context.GetAddressOf());
		if (SUCCEEDED(hr)) {
			break;
		}
	}

	if (enc->adapter.Get() == nullptr) {
		printf("[nvenc] Error: Failed to create d3d11 device. \n");
		goto failed;
	}

	return enc;

failed:
	if (enc != nullptr)  {
		delete enc;
	}
	return nullptr;
}

static void nvenc_destroy(void **nvenc_data)
{
	struct nvenc_data *enc = (struct nvenc_data *)(*nvenc_data);

	enc->mutex.lock();

	if (enc->nvenc != nullptr) {
		//enc->nvenc->DestroyEncoder();
		delete enc->nvenc;
	}

	enc->mutex.unlock();

	delete *nvenc_data;
	*nvenc_data = nullptr;
}

static bool nvenc_init(void *nvenc_data, void *encoder_config)
{
	if (nvenc_data == nullptr) {
		return false;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;
	struct encoder_config* config = (struct encoder_config*)encoder_config;

	std::lock_guard<std::mutex> locker(enc->mutex);
	if (enc->nvenc != nullptr) {
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = config->width;
	desc.Height = config->height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = config->format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = enc->device.Get()->CreateTexture2D(&desc, NULL, enc->texture.GetAddressOf());
	if (FAILED(hr)) {
		printf("[nvenc] Error: Failed to create texture. \n");
		return false;
	}

	enc->width = config->width;
	enc->height = config->height;
	enc->framerate = config->framerate;
	enc->format = config->format;
	enc->codec = config->codec;
	enc->gop = config->gop;
	enc->bitrate = config->bitrate;

	NV_ENC_BUFFER_FORMAT eBufferFormat = NV_ENC_BUFFER_FORMAT_NV12;
	if (enc->format == DXGI_FORMAT_NV12) {
		eBufferFormat = NV_ENC_BUFFER_FORMAT_NV12;
	}
	else if (enc->format == DXGI_FORMAT_B8G8R8A8_UNORM) {
		eBufferFormat = NV_ENC_BUFFER_FORMAT_ARGB;
	}
	else {
		printf("[nvenc] Error: Unsupported dxgi format. \n");
		return false;
	}

	GUID codecId = NV_ENC_CODEC_H264_GUID;
	if (enc->codec == "h264") {
		codecId = NV_ENC_CODEC_H264_GUID;
	}
	else if (enc->codec == "hevc") {
		codecId = NV_ENC_CODEC_HEVC_GUID;
	}
	else {
		printf("[nvenc] Error: Unsupported codec. \n");
		return false;
	}

	enc->nvenc = new NvEncoderD3D11(enc->device.Get(), enc->width, enc->height, eBufferFormat);

	NV_ENC_INITIALIZE_PARAMS initializeParams = { NV_ENC_INITIALIZE_PARAMS_VER };
	NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
	initializeParams.encodeConfig = &encodeConfig;	
	enc->nvenc->CreateDefaultEncoderParams(&initializeParams, codecId, NV_ENC_PRESET_LOW_LATENCY_DEFAULT_GUID);

	initializeParams.maxEncodeWidth = enc->width;
	initializeParams.maxEncodeHeight = enc->height;
	initializeParams.frameRateNum = enc->framerate;
	initializeParams.encodeConfig->gopLength = enc->gop;
	initializeParams.encodeConfig->rcParams.averageBitRate = enc->bitrate ;
	initializeParams.encodeConfig->rcParams.maxBitRate = enc->bitrate;
	//initializeParams.encodeConfig->rcParams.vbvBufferSize = enc->bitrate; // / (initializeParams.frameRateNum / initializeParams.frameRateDen);
	//initializeParams.encodeConfig->rcParams.vbvInitialDelay = initializeParams.encodeConfig->rcParams.vbvInitialDelay;
	initializeParams.encodeConfig->rcParams.rateControlMode = NV_ENC_PARAMS_RC_CBR;
	
	enc->nvenc->CreateEncoder(&initializeParams);

	return true;
}

int nvenc_encode_texture(void *nvenc_data, ID3D11Texture2D *texture, std::vector<std::vector<uint8_t>>& out_packets)
{
	if (nvenc_data == nullptr)
	{
		return -1;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;	

	std::lock_guard<std::mutex> locker(enc->mutex);
	if (enc->nvenc == nullptr)
	{
		return -1;
	}

	const NvEncInputFrame* encoderInputFrame = enc->nvenc->GetNextInputFrame();
	ID3D11Texture2D *pTexBgra = reinterpret_cast<ID3D11Texture2D*>(encoderInputFrame->inputPtr);
	enc->context->CopyResource(pTexBgra, texture);
	enc->nvenc->EncodeFrame(out_packets);

	int frameSize = 0;
	for (std::vector<uint8_t> &packet : out_packets)
	{
		frameSize += (int)packet.size();
	}

	return frameSize;
}

int nvenc_encode_handle(void *nvenc_data, HANDLE handle, int lock_key, int unlock_key, 
	std::vector<std::vector<uint8_t>>& out_packet)
{
	if (nvenc_data == nullptr || handle == nullptr)
	{
		return 0;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;
	ID3D11Texture2D* inputTexture = enc->inputTexture.Get();
	IDXGIKeyedMutex* keyedMutex = enc->keyedMutex.Get();
	int frameSize = 0;

	if (enc->inputHandle != handle)
	{		
		if (enc->inputTexture.Get())
		{
			enc->inputTexture->Release();
		}

		if (enc->keyedMutex.Get())
		{
			enc->keyedMutex->Release();
		}

		HRESULT hr = enc->device->OpenSharedResource((HANDLE)(uintptr_t)handle, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void **>(enc->inputTexture.GetAddressOf()));
		if (FAILED(hr))
		{
			return -1;
		}
				
		inputTexture = enc->inputTexture.Get();
		
		hr = inputTexture->QueryInterface(_uuidof(IDXGIKeyedMutex), reinterpret_cast<void **>(enc->keyedMutex.GetAddressOf()));
		if (FAILED(hr)) 
		{
			enc->inputTexture->Release();
			return -1;
		}

		keyedMutex = enc->keyedMutex.Get();
		enc->inputHandle = handle;
	}

	if (inputTexture != nullptr && keyedMutex != nullptr)
	{
		HRESULT hr = keyedMutex->AcquireSync(lock_key, 1000);
		if (hr != S_OK)
		{
			return -1;
		}
		frameSize = nvenc_encode_texture(enc, inputTexture, out_packet);
		keyedMutex->ReleaseSync(unlock_key);
	}

	return frameSize;
}

int nvenc_set_bitrate(void *nvenc_data, uint32_t bitrate_bps)
{
	if (nvenc_data == nullptr)
	{
		return 0;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;
	std::lock_guard<std::mutex> locker(enc->mutex);
	if (enc->nvenc != nullptr)
	{
		NV_ENC_RECONFIGURE_PARAMS reconfigureParams;
		NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
		reconfigureParams.version = NV_ENC_RECONFIGURE_PARAMS_VER;
		reconfigureParams.forceIDR = true;
		reconfigureParams.reInitEncodeParams = { NV_ENC_INITIALIZE_PARAMS_VER };
		reconfigureParams.reInitEncodeParams.encodeConfig = &encodeConfig;
		enc->nvenc->GetInitializeParams(&reconfigureParams.reInitEncodeParams);
		reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.averageBitRate = bitrate_bps;
		reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.maxBitRate = bitrate_bps;
		enc->nvenc->Reconfigure(&reconfigureParams);
	}

	return 0;
}

int nvenc_set_framerate(void *nvenc_data, uint32_t framerate) 
{
	if (nvenc_data == nullptr)
	{
		return 0;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;
	std::lock_guard<std::mutex> locker(enc->mutex);
	if (enc->nvenc != nullptr)
	{
		NV_ENC_RECONFIGURE_PARAMS reconfigureParams;
		NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
		reconfigureParams.version = NV_ENC_RECONFIGURE_PARAMS_VER;
		reconfigureParams.forceIDR = true;
		reconfigureParams.reInitEncodeParams = { NV_ENC_INITIALIZE_PARAMS_VER };
		reconfigureParams.reInitEncodeParams.encodeConfig = &encodeConfig;
		enc->nvenc->GetInitializeParams(&reconfigureParams.reInitEncodeParams);
		reconfigureParams.reInitEncodeParams.frameRateNum = framerate;
		enc->nvenc->Reconfigure(&reconfigureParams);
	}

	return 0;
}

int nvenc_request_idr(void *nvenc_data)
{
	if (nvenc_data == nullptr)
	{
		return 0;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;
	std::lock_guard<std::mutex> locker(enc->mutex);
	if (enc->nvenc != nullptr)
	{
		enc->nvenc->ForceIDR();
	}

	return 0;
}

int nvenc_get_sequence_params(void *nvenc_data, uint8_t* buf, uint32_t maxBufSize)
{
	if (nvenc_data == nullptr)
	{
		return 0;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;

	std::lock_guard<std::mutex> locker(enc->mutex);
	if (enc->nvenc != nullptr)
	{
		std::vector<uint8_t> seq_params;
		enc->nvenc->GetSequenceParams(seq_params);
		if (seq_params.size() > 0 && seq_params.size() < maxBufSize)
		{
			memcpy(buf, seq_params.data(), seq_params.size());
			return (int)seq_params.size();
		}		
	}

	return 0;
}

static ID3D11Device* get_device(void *nvenc_data)
{	
	if (nvenc_data == nullptr)
	{
		return nullptr;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;

	std::lock_guard<std::mutex> locker(enc->mutex);
	return enc->device.Get();
}

static ID3D11Texture2D* get_texture(void *nvenc_data)
{
	if (nvenc_data == nullptr)
	{
		return nullptr;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;

	std::lock_guard<std::mutex> locker(enc->mutex);
	return enc->texture.Get();
}


static ID3D11DeviceContext* get_context(void *nvenc_data)
{
	if (nvenc_data == nullptr)
	{
		return nullptr;
	}

	struct nvenc_data *enc = (struct nvenc_data *)nvenc_data;

	std::lock_guard<std::mutex> locker(enc->mutex);
	return enc->context.Get();
}

struct encoder_info nvenc_info = {
	is_supported,
	nvenc_create,
	nvenc_destroy,
	nvenc_init,
	nvenc_encode_texture,
	nvenc_encode_handle,
	nvenc_set_bitrate,
	nvenc_set_framerate,
	nvenc_request_idr,
	nvenc_get_sequence_params,
	get_device,
	get_texture,
	get_context
};
