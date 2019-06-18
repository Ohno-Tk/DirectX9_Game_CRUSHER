#ifndef _RESULT_H_
#define _RESULT_H_
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define RESULT_PUSH_POS_X ( 550.0f )	//	ボタンのX
#define RESULT_PUSH_POS_Y ( 480.0f )	//	ボタンのY
#define RESULT_PUSH_POS_WIDTH ( 400.0f )	//	ボタンのWIDTH
#define RESULT_PUSH_POS_HEIGHT ( 100.0f )	//	ボタンのHEIGHT
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitResult( void );	//	リザルトの初期化
void UninitResult( void );	//	リザルトの終了
void UpdateResult( void );	//	リザルトの更新
void DrawResult( void );	//	リザルトの描画
#endif