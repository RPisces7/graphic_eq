/*
 * eqBar.c
 *
 *  Created on: Nov 24, 2014
 *      Author: nzb3684
 */
#include "ui_eqBar.h"
#include "main.h"

UI_EQ_BAR* pThis;

void UI_EqBar_Draw(UI_EQ_BAR* pThis){
	
	UI_EQ_BAR_VALUE eq_value = pThis->value;
	UI_EQ_BAR_VALUE prev_eq_value = pThis->prevValue;
	UI_EQ_BAR_VALUE positiveDiff = pThis->value > pThis->prevValue;
	
	//height of filled in part of bar in pixels
	D4D_COOR actualHeight = ((float)pThis->value)/255 * pThis->scrSize.cy;
	D4D_COOR prevHeight = ((float)pThis->prevValue)/255 * pThis->scrSize.cy;
	D4D_COOR diffHeight;
	//Calculate boundary values of filled in part of bar where displayed on screen
	D4D_COOR LeftX = pThis->scrPos.x;
	D4D_COOR BottomY = pThis->scrPos.y + pThis->scrSize.cy;
	D4D_COOR TopY = BottomY - actualHeight;
	D4D_COOR RightX = LeftX + pThis->scrSize.cx;
	
	if(positiveDiff){
		diffHeight = actualHeight - prevHeight;
	}else{
		diffHeight = prevHeight - actualHeight;
	}
	
	//diffHeight = ((float)diff)/255 * pThis->scrSize.cy;
	//if this component was never drawn then draw it in fully
	if (!pThis->drawn){
		pThis->drawn = 1;
		//Draw in full grey background
		D4D_FillRectXY(pThis->scrPos.x, pThis->scrPos.y, RightX, TopY - 1, D4D_COLOR_LIGHT_GREY);
		//Draw in full green foreground
		D4D_FillRectXY( LeftX, TopY, RightX, BottomY, D4D_COLOR_GREEN);
	}
	//otherwise we want to optimize what's drawn by looking at what changed from the last draw.
	else if (pThis->prevValue != pThis->value){
		//if our difference is positive(value has increased) fill in difference with green
		if (positiveDiff){
			//Draw foreground green rectangle
			D4D_FillRectXY( LeftX, TopY, RightX, TopY + diffHeight, D4D_COLOR_GREEN);
		}
		//else we want to fill in the difference with grey
		else{
			D4D_FillRectXY( LeftX, TopY - diffHeight, RightX, TopY, D4D_COLOR_LIGHT_GREY);
		}
		
	}
	pThis->prevValue = pThis->value;
}

void UI_EqBar_Set_Value(UI_EQ_BAR* pThis, UI_EQ_BAR_VALUE value){
	if(pThis->value != value){
		pThis->update = 1;
		pThis->value = value;
	}
}
