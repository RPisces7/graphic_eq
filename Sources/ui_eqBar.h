/*
 * ui_eqBar.h
 *
 *  Created on: Nov 24, 2014
 *      Author: nzb3684
 */
#include "main.h"

#ifndef UI_EQBAR_H_
#define UI_EQBAR_H_
	
	typedef Byte UI_EQ_BAR_VALUE; 
	typedef struct UI_EQ_BAR_S
	{
		D4D_POINT scrPos;
		D4D_SIZE scrSize;
		UI_EQ_BAR_VALUE value;
		UI_EQ_BAR_VALUE prevValue;
		int update;
		int drawn;
	}UI_EQ_BAR;

	
//	typedef UI_EQ_BAR* UI_EQ_BAR_PTR;
	


	#define DECLARE_UI_EQ_BAR(name, x, y, cx, cy)\
		UI_EQ_BAR name## = \
			{ \
				{x,y}, \
				{cx,cy}, \
				0,\
				0,\
				1,\
				0\
			};\
			
	void UI_EqBar_Draw(UI_EQ_BAR* pThis);
	void UI_EqBar_Set_Value(UI_EQ_BAR* pThis, UI_EQ_BAR_VALUE value);
	
#endif /* UI_EQBAR_H_ */
