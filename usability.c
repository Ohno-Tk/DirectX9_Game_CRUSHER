/*=============================================================================

		�v�Z����[ usability.h ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/08/02
-------------------------------------------------------------------------------
	���@Update
		2016/08/02
			�E�w�b�_�t�@�C���ǉ�
			#include <stdio.h>
			#include "main.h"
			#include "usability.h"
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "usability.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 �֐���:	bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight )
 ����:		float aPosX
			float aPosY
			float aWidth
			float aHeight
			float bPosX
			float bPosY
			float bWidth
			float bHeight
 �߂�l:	�������Ă���ꍇ	return true;
			�������Ă��Ȃ��ꍇ	return false;
 ����:		��`�̓����蔻��
-----------------------------------------------------------------------------*/
bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight )
{
	float x[] = { aPosX , aPosX +  aWidth , bPosX , bPosX + bWidth};
	float y[] = { aPosY , aPosY +  aHeight , bPosY , bPosY + bHeight};

	//	�����蔻��
	if( x[0] < x[3] && x[2] < x[1] && y[0] < y[3] && y[2] < y[1] )
	{
		return true;
	}
	return false;
}
/*-----------------------------------------------------------------------------
 �֐���:	bool CircleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float aArea , float bPosX , float bPosY , float bWidth , float bHeight , float bArea )
 ����:		float aPosX
			float aPosY
			float aWidth
			float aHeight
			float aArea
			float bPosX
			float bPosY
			float bWidth
			float bHeight
			float bArea
 �߂�l:	�������Ă���ꍇ	return true;
			�������Ă��Ȃ��ꍇ	return false;
 ����:		�~�̓����蔻��
-----------------------------------------------------------------------------*/
bool CircleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float aArea , float bPosX , float bPosY , float bWidth , float bHeight , float bArea )
{
	//	�����蔻��
	if( (((aPosY + aHeight * 0.5f) - (bPosY + bHeight * 0.5f)) * ((aPosY + aHeight * 0.5f) - (bPosY + bWidth * 0.5f))) + (((aPosX + aWidth * 0.5f) - (bPosX + bWidth * 0.5f)) * ((aPosX + aWidth * 0.5f) - (bPosX + bWidth * 0.5f))) < (aArea + bArea) * (aArea + bArea) )
	{
		return true;
	}
	return false;
}