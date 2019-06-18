#ifndef _PARTICLE_H_
#define _PARTICLE_H_
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void Initparticle( void );	//	パーティクルの初期化
void Uninitparticle( void );	//	パーティクルの終了
void Updateparticle( void );	//	パーティクルの更新
void Drawparticle( void );	//	パーティクルの描画
void SetParticle( float PosX , float PosY );	//	パーティクルのセット
#endif