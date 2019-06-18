#ifndef _EFFECT_H_
#define _EFFECT_H_
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define EFFECT_SPEED ( 5.0f )
#define MAX_EFFECT ( 10 )
/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef enum
{
	EFFECT_TYPE_ATTACK = 0,
	EFFECT_TYPE_MAX
}EFFECT_TYPE;
typedef struct
{
	D3DXVECTOR2 pos;	//	���W
	D3DXVECTOR2 size;	//	�傫��
	D3DXVECTOR2 move;	//	�ړ���
	D3DXVECTOR2 rot;	//	����
	D3DXCOLOR color;	//	�J���[
	float length;	//	����
	float fRadius;	//	���a
	float fRadiusValue;	//���a�̕ω���
	float fColorAlphaValue;	//	�F�̕ω���
	int nLife;	//	����
	bool bUse;	//	�g�p�t���O
	EFFECT_TYPE type;	//	�G�t�F�N�g�̎��
}EFFECT;
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
void InitEffect( void );	//	�G�t�F�N�g�̏�����
void UninitEffect( void );	//	�G�t�F�N�g�̏I��
void UpdateEffect( void );	//	�G�t�F�N�g�̍X�V
void DrawEffect( void );	//	�G�t�F�N�g�̕`��
void SetEffect( float PosX , float PosY , float Speed , EFFECT_TYPE type );	//	�G�t�F�N�g�̐ݒu
EFFECT *GetEffect( void );
#endif