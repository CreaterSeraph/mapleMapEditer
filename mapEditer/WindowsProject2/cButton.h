#pragma once
class cButton
{
private:
	enum BUTTON_STATE
	{
		BTST_DISABLED, BTST_NORMAL, BTST_OVER, BTST_CLICK, BTST_CLICKOUT
	} state;

	texture* image[4];

	function<void()> clickFunc;
	
	RECT rc;

	bool deleteTexture;
public:
	cButton(function<void()> func, texture* imageDisabled, texture* imageNormal, texture* imageOver, texture* imageClick, const RECT& rc, bool deleteTexture, BUTTON_STATE state = BTST_NORMAL);
	cButton(function<void()> func, const tstring& folderPath, const D3DXVECTOR2& pos, bool deleteTexture = true, BUTTON_STATE state = BTST_NORMAL);
	~cButton();

	void Update();
	void Render();

	BUTTON_STATE GetState() { return state; }
	void SetState(BUTTON_STATE state) { this->state = state; }
};

