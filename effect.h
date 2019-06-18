#ifndef _EFFECT_H_
#define _EFFECT_H_
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define EFFECT_SPEED ( 5.0f )
#define MAX_EFFECT ( 10 )
/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef enum
{
	EFFECT_TYPE_ATTACK = 0,
	EFFECT_TYPE_MAX
}EFFECT_TYPE;
typedef struct
{
	D3DXVECTOR2 pos;	//	座標
	D3DXVECTOR2 size;	//	大きさ
	D3DXVECTOR2 move;	//	移動量
	D3DXVECTOR2 rot;	//	向き
	D3DXCOLOR color;	//	カラー
	float length;	//	長さ
	float fRadius;	//	半径
	float fRadiusValue;	//半径の変化量
	float fColorAlphaValue;	//	色の変化量
	int nLife;	//	寿命
	bool bUse;	//	使用フラグ
	EFFECT_TYPE type;	//	エフェクトの種類
}EFFECT;
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitEffect( void );	//	エフェクトの初期化
void UninitEffect( void );	//	エフェクトの終了
void UpdateEffect( void );	//	エフェクトの更新
void DrawEffect( void );	//	エフェクトの描画
void SetEffect( float PosX , float PosY , float Speed , EFFECT_TYPE type );	//	エフェクトの設置
EFFECT *GetEffect( void );
#endif