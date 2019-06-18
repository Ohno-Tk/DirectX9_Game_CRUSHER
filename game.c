/*=============================================================================

		ゲーム内容[ game.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/07/28
-------------------------------------------------------------------------------
	■　Update
		2016/07/28
			・ヘッダファイル追加
				#include <stdio.h>
				#include "game.h"
				#include "input.h"
				#include "fade.h"
				#include "player.h"
				#include "bg.h"
			・InitGame追加
				InitBg();	//	背景の初期化
				InitPlayer();	//	プレイヤーの初期化
			・UninitGame追加
				UninitBg();	//	背景の終了
				UninitPlayer();	//	プレイヤーの終了
			・UpdateGame追加
				UpdateBg();	//	背景の更新
				UpdatePlayer();	//	プレイヤーの更新
			・DrawGame追加
				DrawBg();	//	背景の描画
				DrawPlayer();	//	プレイヤーの描画
		2016/08/02
			・ヘッダファイル追加
				#include "time.h"
				#include "score.h"
			・InitGame追加
				InitScore();	//	スコアの初期化
				InitTime();	//	タイムの初期化
			・UninitGame追加
				UninitScore();	//	スコアの終了
				UninitTime();	//	タイムの終了
			・UpdateGame追加
				UpdateScore();	//	スコアの更新
				UpdateTime();	//	タイムの更新
			・DrawGame追加
				DrawScore();	//	スコアの描画
				DrawTime();	//	タイムの描画
		2016/08/08
			・ヘッダファイル追加
				#include "enemy.h"
			・InitGame追加
				InitEnemy();	//	敵の初期化
			・UninitGame追加
				UninitEnemy();	//	敵の終了
			・UpdateGame追加
				UpdateEnemy();	//	敵の更新
			・DrawGame追加
				DrawEnemy();	//	敵の描画
		2016/08/22
			・ヘッダファイル追加
				##include "base.h"
			・InitGame追加
				InitBase();	//	足場の初期化
			・UninitGame追加
				UninitBase();	//	足場の終了
			・UpdateGame追加
				UpdateBase();	//	足場の更新
			・DrawGame追加
				DrawBase();		//	足場の描画
		2016/08/28
			・ヘッダファイル追加
				#include "sound.h"
		2016/09/07
			・ヘッダファイル追加
				#include "effect.h"
			・InitGame追加
				InitEffect();	//	エフェクトの初期化
			・UninitGame追加
				UninitEffect();	//	エフェクトの終了
			・UpdateGame追加
				UpdateEffect();	//	エフェクトの更新
			・DrawGame追加
				DrawEffect();	//	エフェクトの描画
		2016/09/11
			・ヘッダファイル追加
				#include "particle.h"
			・InitGame追加
				Initparticle();	//	パーティクルの初期化
			・UninitGame追加
				Uninitparticle();	//	パーティクルの終了
			・UpdateGame追加
				Updateparticle();	//	パーティクルの更新
			・DrawGame追加
				Drawparticle();	//	パーティクルの描画
		2016/09/14
			・ヘッダファイル追加
				#include "cloud.h"
			・InitGame追加
				InitCloud();	//	雲の初期化
			・UninitGame追加
				UninitCloud();	//	雲の終了
			・UpdateGame追加
				UpdateCloud();	//	雲の更新
			・DrawGame追加
				DrawCloud();	//	雲の描画
		2016/09/19
			・ヘッダファイル追加
				#include "pause.h"
			・InitGame追加
				InitPause();	//	ポーズの初期化
			・UninitGame追加
				UninitPause();	//	ポーズの終了
			・UpdateGame追加
				UpdatePause();	//	ポーズの更新
			・DrawGame追加
				DrawPause();	//	ポーズの描画
		2016/09/27
			・ヘッダファイル追加
				#include "guid.h"
			・InitGame追加
				InitGuid();	//	案内の初期化
			・UninitGame追加
				UninitGuid();	//	案内の終了
			・UpdateGame追加
				UpdateGuid();	//	案内の更新
			・DrawGame追加
				DrawGuid();	//	案内の描画
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "time.h"
#include "score.h"
#include "base.h"
#include "effect.h"
#include "particle.h"
#include "cloud.h"
#include "pause.h"
#include "operation.h"
#include "guid.h"
/*-----------------------------------------------------------------------------
 関数名:	void InitGame( void )
 引数:		なし
 戻り値:	なし
 説明:		ゲームの初期化
-----------------------------------------------------------------------------*/
void InitGame( void )
{
	//	今のモード
	MODE *Mode = GetMode();

	*Mode = MODE_GAME;

	InitBg();		//	背景の初期化

	InitCloud();	//	雲の初期化

	InitBase();		//	足場の初期化

	InitGuid();	//	案内の初期化

	InitEnemy();	//	敵の初期化

	InitPlayer();	//	プレイヤーの初期化

	Initparticle();	//	パーティクルの初期化

	InitEffect();	//	エフェクトの初期化

	InitScore();	//	スコアの初期化

	InitTime();		//	タイムの初期化

	InitPause();	//	ポーズの初期化

	InitOperation();	//	操作方法の初期化

	//	音楽再生
	PlaySound( SOUND_LABEL_BGM_GAME );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitGame( void )
 引数:		なし
 戻り値:	なし
 説明:		ゲームの終了
-----------------------------------------------------------------------------*/
void UninitGame( void )
{
	UninitBg();		//	背景の終了

	UninitCloud();	//	雲の終了

	UninitBase();	//	足場の終了

	UninitGuid();	//	案内の終了

	UninitEnemy();	//	敵の終了

	UninitPlayer();	//	プレイヤーの終了

	Uninitparticle();	//	パーティクルの終了

	UninitEffect();	//	エフェクトの終了

	UninitTime();	//	タイムの終了

	UninitPause();	//	ポーズの終了

	UninitOperation();	//	操作方法の終了


}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdeteGame( void )
 引数:		なし
 戻り値:	なし
 説明:		ゲームの更新
-----------------------------------------------------------------------------*/
void UpdateGame( void )
{
	//	タイム情報の取得
	int *Time = GetTime();

	//	プレイヤー情報の取得
	PLAYER *Player = GetPlayer();

	//	ポーズ情報の取得
	bool *Pause = GetPause();
	bool *PauseUse = GetOpeFlag();

	static bool BebugPause = false;

	if(GetKeyboardTrigger(DIK_O))
	{
		BebugPause ^= 1;
	}

	//	画面遷移
	if( Player->gool == true || Player -> bUse == false )
	{
		SetFade(FADE_OUT , MODE_RESULT);

		//	音楽停止
		StopSound( SOUND_LABEL_BGM_GAME );
	}

	if( *Pause == false && BebugPause == false )
	{
		UpdateBg();		//	背景の更新

		UpdateCloud();	//	雲の更新

		UpdateBase();	//	足場の更新

		UpdateGuid();	//	案内の更新

		UpdateEnemy();	//	敵の更新

		UpdatePlayer();	//	プレイヤーの更新

		Updateparticle();	//	パーティクルの更新

		UpdateEffect();	//	エフェクトの更新

		UpdateScore();	//	スコアの更新

		UpdateTime();	//	タイムの更新
	}	//	end of if

	if( *PauseUse == false )
	{
		UpdatePause();	//	ポーズの更新
	}
	else if( *PauseUse == true )
	{
		UpdateOperation();	//	操作方法の更新
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawGame( void )
 引数:		なし
 戻り値:	なし
 説明:		ゲームの描画
-----------------------------------------------------------------------------*/
void DrawGame( void )
{
	//	ポーズ情報の取得
	bool *PauseUse = GetOpeFlag();

	DrawBg();		//	背景の描画

	DrawCloud();	//	雲の描画

	DrawBase();		//	足場の描画

	DrawGuid();	//	案内の描画

	DrawEnemy();	//	敵の描画

	DrawPlayer();	//	プレイヤーの描画

	Drawparticle();	//	パーティクルの描画

	DrawEffect();	//	エフェクトの描画

	DrawScore();	//	スコアの描画

	DrawTime();		//	タイムの描画

	if( *PauseUse == false )
	{
		DrawPause();	//	ポーズの描画
	}
	else if( *PauseUse == true )
	{
		DrawOperation();	//	操作方法の描画
	}

}	//	end of func