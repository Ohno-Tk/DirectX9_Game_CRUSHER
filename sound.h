//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		//	タイトルBGM
	SOUND_LABEL_BGM_GAME,			//	ゲームBGM
	SOUND_LABEL_BGM_RESULT,			//	リザルトBGM
	SOUND_LABEL_SE_ENTER,			// 決定音
	SOUND_LABEL_SE_SERECT,			//	選択音
	SOUND_LABEL_SE_HIT,				//	撃破音
	SOUND_LABEL_SE_GOOL,			//	ゴール音
	//SOUND_LABEL_SE_ATTACK,//	攻撃音
	
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);	//	音楽の初期化
void UninitSound(void);	//	音楽の終了
HRESULT PlaySound(SOUND_LABEL label);	//	音楽再生
void StopSound(SOUND_LABEL label);	//	何番目の音を止める
void StopSound(void);

#endif
