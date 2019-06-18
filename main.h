#ifndef _MAIN_H_
#define _MAIN_H_
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/

#include <windows.h>
#include <d3dx9.h>

#pragma comment( lib , "d3d9.lib" )
#define DIRECTINPUT_VERSION ( 0x0800 )
#include <dinput.h>
#include <XAudio2.h>
	
#pragma comment( lib , "d3dx9.lib" )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "winmm.lib" )	//	システム時刻取得に必要
#pragma comment( lib , "dinput8.lib" )	

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define SCREEN_WIDTH   ( 950 )				//	ウインドウの幅
#define SCREEN_HEIGHT  ( 575 )				//	ウインドウの高さ

//	頂点フォーマット
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#define NUM_VERTEX ( 4 )	//	頂点数
#define NUM_POLYGON ( 2 )	//	ポリゴン数
#define FADE_RATE ( 1.0f/80 )	//	どのくらいでフェードさせるのか
/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
//	頂点構造体（上のフォーマットの型に合わせること）
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;	//	座標変換済み頂点フラグ（DirectXにしかない仕様）必ず1.0fを入れる
	D3DCOLOR color;
	D3DXVECTOR2 tex;
}VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_RANKING,
	MODE_MAX
}MODE;

typedef void (*ModeFunc)(void);
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );

HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow );	//	初期化関数
void Uninit( void );	//	終了処理
void Update( void );	//	更新処理
void Draw( void );	//	描画処理

LPDIRECT3DDEVICE9 GetDevice( void );	//	D3DDeviceの取得
MODE *GetMode( void );

void SetMode( MODE mode );
#endif