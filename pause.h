#ifndef _PAUSE_H_
#define _PAUSE_H_
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitPause( void );	//	ポーズの初期化
void UninitPause( void );	//	ポーズの終了
void UpdatePause( void );	//	ポーズの更新
void DrawPause( void );	//	ポーズの描画
bool *GetPause( void );	//	ポーズしているか
bool *GetOpeFlag( void );
#endif