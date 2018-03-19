#pragma once

namespace MyDevice
{
	HRESULT CreateDevice(int sizeX, int sizeY);
	void ReleaseDevice();
	D3DPRESENT_PARAMETERS GetD3DPP(int sizeX, int sizeY);
	vector<POINT> GetDeviceSize();
}