/*=============================================================================

		�v�Z����[ usability.h ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/08/02
-------------------------------------------------------------------------------
	���@Update
		2016/08/02
			�E�v���g�^�C�v�錾�ǉ�
				bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight );	//	��`�̓����蔻��
-----------------------------------------------------------------------------*/
#ifndef _USABILITY_H_
#define _USABILITY_H_
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
bool RectangleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float bPosX , float bPosY , float bWidth , float bHeight );	//	��`�̓����蔻��
bool CircleDecision( float aPosX , float aPosY , float aWidth , float aHeight , float aArea , float bPosX , float bPosY , float bWidth , float bHeight , float bArea );	//	�~�̓����蔻��
#endif