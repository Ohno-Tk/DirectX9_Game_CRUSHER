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
			・プロトタイプ宣言追加
				bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight );	//	矩形の当たり判定
-----------------------------------------------------------------------------*/
#ifndef _USABILITY_H_
#define _USABILITY_H_
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight );	//	矩形の当たり判定
bool CircleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float aArea , float bPosX , float bPosY , float bWidth , float bHeight , float bArea );	//	円の当たり判定
#endif