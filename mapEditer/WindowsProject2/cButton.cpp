#include "stdafx.h"
#include "cButton.h"

cButton::cButton(function<void()> func, texture* imageDisabled, texture* imageNormal, texture* imageOver, texture* imageClick, const RECT& rc, bool deleteTexture, BUTTON_STATE state)
	:clickFunc(func), state(state), rc(rc), deleteTexture(deleteTexture)
{
	image[0] = imageDisabled;
	image[1] = imageNormal;
	image[2] = imageOver;
	image[3] = imageClick;
}

cButton::cButton(function<void()> func, const tstring& folderPath, const D3DXVECTOR2& pos, bool deleteTexture, BUTTON_STATE state)
	:clickFunc(func), state(state), deleteTexture(deleteTexture)
{
	rc.left = pos.x;
	rc.top = pos.y;
	image[0] = new texture(folderPath + T("/disabled.png"));
	image[1] = new texture(folderPath + T("/normal.png"));
	image[2] = new texture(folderPath + T("/mouseOver.png"));
	image[3] = new texture(folderPath + T("/pressed.png"));

	for (int i = 0; i < 4; i++)
	{
		if (!image[i]->isLoaded)
		{
			SAFE_DELETE(image[i]);
		}
		else
		{
			rc.right = pos.x + image[i]->info.Width;
			rc.bottom = pos.y + image[i]->info.Height;
		}
	}
}


cButton::~cButton()
{
	if (deleteTexture)
	{
		for (int i = 0; i < 4; i++)
		{
			SAFE_DELETE(image[i]);
		}
	}
}

void cButton::Update()
{
	bool collision = PtInRect(&rc, ptMouse);

	switch (state)
	{
	case cButton::BTST_DISABLED:
		//È÷È÷
		break;
	case cButton::BTST_NORMAL:
		if (collision)
			state = BTST_OVER;
		break;
	case cButton::BTST_OVER:
		if (!collision)
			state = BTST_NORMAL;
		if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
			state = BTST_CLICK;
		break;
	case cButton::BTST_CLICK:
		if (!collision)
			state = BTST_CLICKOUT;
		if (KEYMANAGER->IsOnceKeyUp(VK_LBUTTON))
		{
			state = BTST_NORMAL;
			clickFunc();
		}
		break;
	case cButton::BTST_CLICKOUT:
		if (KEYMANAGER->IsOnceKeyUp(VK_LBUTTON))
			state = BTST_NORMAL;
		if (collision)
			state = BTST_CLICK;
		break;
	}
}

void cButton::Render()
{
	if (state == BTST_CLICKOUT)
		image[1]->Render(rc.left, rc.top);
	else
		image[state]->Render(rc.left, rc.top);
}
