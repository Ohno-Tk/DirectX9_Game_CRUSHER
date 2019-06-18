#ifndef _FADE_H_
#define _FADE_H_
/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef enum
{
	FADE_NONE = 0,
	FADE_IN ,
	FADE_OUT ,
	FADE_MAX
}FADE;
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitFade( void );	//	フェードの初期化
void UninitFade( void );	//	フェードの終了
void UpdateFade( void );	//	フェードの更新
void DrawFade( void );	//	フェードの描画
void SetFade( FADE fade , MODE modenext );	//	フェード状態設定
MODE *GetNextMode( void );	//	次のモードの追加
bool *GetFadeUse( void );	//	フェード中かどうか
#endif