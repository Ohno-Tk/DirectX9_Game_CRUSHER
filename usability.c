/*=============================================================================

		計算処理[ usability.h ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/08/02
-------------------------------------------------------------------------------
	■　Update
		2016/08/02
			・ヘッダファイル追加
			#include <stdio.h>
			#include "main.h"
			#include "usability.h"
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "usability.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 関数名:	bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight )
 引数:		float aPosX
			float aPosY
			float aWidth
			float aHeight
			float bPosX
			float bPosY
			float bWidth
			float bHeight
 戻り値:	当たっている場合	return true;
			当たっていない場合	return false;
 説明:		矩形の当たり判定
-----------------------------------------------------------------------------*/
bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight )
{
	float x[] = { aPosX , aPosX +  aWidth , bPosX , bPosX + bWidth};
	float y[] = { aPosY , aPosY +  aHeight , bPosY , bPosY + bHeight};

	//	当たり判定
	if( x[0] < x[3] && x[2] < x[1] && y[0] < y[3] && y[2] < y[1] )
	{
		return true;
	}
	return false;
}
/*-----------------------------------------------------------------------------
 関数名:	bool CircleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float aArea , float bPosX , float bPosY , float bWidth , float bHeight , float bArea )
 引数:		float aPosX
			float aPosY
			float aWidth
			float aHeight
			float aArea
			float bPosX
			float bPosY
			float bWidth
			float bHeight
			float bArea
 戻り値:	当たっている場合	return true;
			当たっていない場合	return false;
 説明:		円の当たり判定
-----------------------------------------------------------------------------*/
bool CircleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float aArea , float bPosX , float bPosY , float bWidth , float bHeight , float bArea )
{
	//	当たり判定
	if( (((aPosY + aHeight * 0.5f) - (bPosY + bHeight * 0.5f)) * ((aPosY + aHeight * 0.5f) - (bPosY + bWidth * 0.5f))) + (((aPosX + aWidth * 0.5f) - (bPosX + bWidth * 0.5f)) * ((aPosX + aWidth * 0.5f) - (bPosX + bWidth * 0.5f))) < (aArea + bArea) * (aArea + bArea) )
	{
		return true;
	}
	return false;
}