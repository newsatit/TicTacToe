//***************************************************
//ECE 210 Final Project
//Jason Zhou; Dawanit Satitsumpun; Avinash Narisetty
//**********GLOBAL VARIABLE**************************
#include "lab_buttons.h"
#include "ece210_api.h"
#include "Logo.black.h"
#include "stdlib.h"
#include "x_IMAGE_BITMAP.h"
#include "O_IMAGE.h"

#define LCD_SIZE_X 240              //X size of LCD screen in pixels
#define LCD_SIZE_Y 320              //Y size of LCD screen in pixels

#define	LCD_INIT_X			LCD_HALF_SIZE_X - Logo_WIDTH_PXL/2		// Start image in the center of the screen X
#define	LCD_INIT_Y			LCD_HALF_SIZE_Y - Logo_HEIGHT_PXL/2	// Start image in the center of the screen Y
#define LCD_MAX_DRAW_X	LCD_SIZE_X-1	// 0 to LCD_MAX_DRAW_X is = LCD_SIZE_X
#define LCD_MAX_DRAW_Y	LCD_SIZE_Y-1	// 0 to LCD_MAX_DRAW_Y is = LCD_SIZE_Y
#define LCD_HALF_SIZE_X	LCD_SIZE_X/2	// X center of screen in pixels
#define LCD_HALF_SIZE_Y	LCD_SIZE_Y/2	// Y center of screen in pixels


#define LOCAL_ID 0x00
#define REMOTE_ID 0x01
uint32_t character;              //if character==0, then I'm "X"
                                 //if character==1, then I'm "O"

boolean myTurn;

																 
 const uint8_t*p_image=X_bitmap;
 const uint8_t*p_image2=O_bitmap;
 const uint8_t*p_imageDisplay;
  uint16_t 	x_pos =  20	;// x_pos holds the x position of the right corner of the image
  uint16_t 	y_pos = 20;		// y_pos holds the y position of the lower corner of the image
  uint16_t	width_pixels = X_WIDTH_PXL;		// width of the imsge in pixels
    uint16_t 	height_pixels = X_WIDTH_PXL;	// height of the image in pixels
    uint16_t renderArray[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t renderArrayPosX[9] = {20, 100, 180,  20, 100, 180,  20, 100, 180};
	uint16_t renderArrayPosY[9] = {20,  20,  20, 120, 120, 120, 200, 200, 200};
	uint8_t direction;
	uint16_t curIndex = 0;
																	
	int image=1;

//**********FUNCTION DECLARATION*********************
void start();
void play();
void select();

//***************************************************
int main(void)
{
    //main method starts
    ece210_initialize_board();  //start initializing the board
    start();        //call method start
    select();
    if(character == 0) {
        myTurn = true;
    } else {
        myTurn = false;
    }
    play();
}

void start()
{
    //***********LOCAL VARIABLE*************************
    uint32_t ready = 0;
    //**************************************************
    //display message first
    
   ece210_lcd_add_msg("Welcome to TIC-TAC-TOE", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);             
    ece210_lcd_add_msg("Press Any Key to Start", TERMINAL_ALIGN_CENTER, LCD_COLOR_BLUE);
    ece210_lcd_add_msg("Jason Zhou; Dawanit Satitsumpun; Avinash Narisetty", TERMINAL_ALIGN_CENTER, LCD_COLOR_YELLOW);
    
    
      ece210_lcd_draw_image(LCD_INIT_X,Logo_WIDTH_PXL,LCD_INIT_Y-50 ,Logo_HEIGHT_PXL, Logo_bitmap ,LCD_COLOR_RED, LCD_COLOR_BLACK);    //display the logo of the TIC-TAC-TOE
    
    //initialize wireless connection
    ece210_wireless_init(LOCAL_ID, REMOTE_ID);
    
    while(!ready){
        //when the board detects that a key is pressed, start play() method
        //which will call up the "x" and "o" selection page
    
        if(ece210_wireless_data_avaiable()) {
            ready = ece210_wireless_get();
            if(ready) {
                return;
            }
        }
        
        if(AlertButtons)
        {
            AlertButtons=false; //set AlertButton to false
            ready = 1;
            ece210_wireless_send(1);
            return; //leads to the charater selection page
        }
    }
}

void select()
{
    //********************LOCAL VARIABLE*******************
    uint32_t curIndex=0;
    uint32_t otherPlayer;
    uint8_t direction;
    //*****************************************************
    //display the message first
    for(uint8_t i = 0; i < 20 ; i ++){
    	 ece210_lcd_add_msg("", TERMINAL_ALIGN_CENTER, LCD_COLOR_BLACK);
    }
			//clear the pic from the screen
    ece210_lcd_add_msg("Press Right Button to select between X and O", TERMINAL_ALIGN_CENTER, LCD_COLOR_GREEN);
    ece210_lcd_add_msg("X Goes First", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
    
    ece210_lcd_draw_image(40,X_WIDTH_PXL,100 ,X_HEIGHT_PXL, X_bitmap ,LCD_COLOR_RED, LCD_COLOR_BLACK);            //draw the "x" on the left black rectangle
    ece210_lcd_draw_image(150,O_WIDTH_PXL,100 ,O_HEIGHT_PXL, O_bitmap ,LCD_COLOR_RED, LCD_COLOR_BLACK);            //draw the "o" on the right black rectangle
    
    while(1)
    {	
        otherPlayer = 0;
        if(ece210_wireless_data_avaiable()) {
            otherPlayer = ece210_wireless_get();
            // opponent chose X
            if(otherPlayer == 0) {
                character = 1;
            // opponent chose O    
            } else if(otherPlayer == 1) {
                character = 0;
            }
            return;
           
        }
        if(curIndex == 0) {
			ece210_lcd_draw_rectangle(40, 50, 100, 50, LCD_COLOR_BLACK);    //clear the pic from the screen
			ece210_wait_mSec(100);
			
			ece210_lcd_draw_image(40,X_WIDTH_PXL,100 ,X_HEIGHT_PXL, X_bitmap ,LCD_COLOR_RED, LCD_COLOR_BLACK);    //draw the index 0 pic to the screen
			ece210_wait_mSec(100);               //make it flashing
					
		} else if(curIndex == 1) {
			ece210_lcd_draw_rectangle(150, 50, 100, 50, LCD_COLOR_BLACK);    //clear the pic from the screen
			ece210_wait_mSec(100); 
			
			ece210_lcd_draw_image(150,O_WIDTH_PXL,100 ,O_HEIGHT_PXL, O_bitmap ,LCD_COLOR_RED, LCD_COLOR_BLACK);    //draw the index 0 pic to the screen
			ece210_wait_mSec(100); 
		}
				
		if(AlertButtons)
        {
            AlertButtons=false;
            if(btn_right_pressed())
            {
                ece210_wireless_send(curIndex);
                character = curIndex;
                play();
				break;
            }
        }
        direction=ece210_ps2_read_position();   
        switch(direction)
        {
            case PS2_RIGHT:
            {
                curIndex=1;
                break;
            }
            
            case PS2_LEFT:
            {
                curIndex=0;
                break;
            }
            			
			case PS2_DOWN:
			{
				ece210_wireless_send(curIndex);
				character = curIndex;
				return;			
			}
						
			default:
            {
                break;
            }
							
        }
				


    }
		
		
}
void render_board()
{
	ece210_lcd_draw_rectangle(80,5,y_pos,240,LCD_COLOR_RED);
	ece210_lcd_draw_rectangle(160,5,y_pos,240,LCD_COLOR_RED);
	ece210_lcd_draw_rectangle(10,220,100,5,LCD_COLOR_RED);
	ece210_lcd_draw_rectangle(10,220,190,5,LCD_COLOR_RED);
	
    for(uint16_t i = 0; i < 9; i++){
		
    	if ( renderArray[i] == 1){		
    		ece210_lcd_draw_image(renderArrayPosX[i],width_pixels, renderArrayPosY[i],height_pixels, p_image ,LCD_COLOR_RED, LCD_COLOR_BLACK);
    	}
    	if ( renderArray[i] == 2){		
    		ece210_lcd_draw_image(renderArrayPosX[i],width_pixels, renderArrayPosY[i],height_pixels, p_image2 ,LCD_COLOR_RED, LCD_COLOR_BLACK);
    	}
	}
}

void render_selection()
	{
	
		direction=ece210_ps2_read_position();   
        switch(direction)
        {
            case PS2_RIGHT:
            {
				if(curIndex < 8){
                    curIndex++;
				}
				if(curIndex != 0){
			         ece210_lcd_draw_rectangle(renderArrayPosX[curIndex-1],width_pixels,renderArrayPosY[curIndex-1],height_pixels,LCD_COLOR_BLACK);	
				}
                break;
            }
            
            case PS2_LEFT:
            {
                if(curIndex > 0){
                    curIndex--;
				}
				if(curIndex != 8){
			         ece210_lcd_draw_rectangle(renderArrayPosX[curIndex+1],width_pixels,renderArrayPosY[curIndex+1],height_pixels,LCD_COLOR_BLACK);	
				}
                break;
            }
			case PS2_UP:
            {
                if(curIndex > 2){
                    curIndex= curIndex - 3;;
				}
				if(curIndex < 6){
			         ece210_lcd_draw_rectangle(renderArrayPosX[curIndex+3],width_pixels,renderArrayPosY[curIndex+3],height_pixels,LCD_COLOR_BLACK);	
				}
                break;
            }
			case PS2_DOWN:
            {
                if(curIndex < 6){
                    curIndex= curIndex + 3;;
				}
				if(curIndex > 2){
			         ece210_lcd_draw_rectangle(renderArrayPosX[curIndex-3],width_pixels,renderArrayPosY[curIndex-3],height_pixels,LCD_COLOR_BLACK);	
				}
                break;
            }
            default:
            {
                break;
            }
        }
		ece210_lcd_draw_image(renderArrayPosX[curIndex],width_pixels, renderArrayPosY[curIndex],height_pixels, p_imageDisplay ,LCD_COLOR_RED, LCD_COLOR_BLACK);
		ece210_wait_mSec(  150 );	
}
void selection()
{
    if(AlertButtons){
		AlertButtons=false;
		if( btn_right_pressed()){

			renderArray[curIndex] = 1;
					
		}
			
		if(btn_left_pressed()){

			renderArray[curIndex] = 2;
					
		}
	}
}	
	
void play()	
{
	
    ece210_lcd_draw_rectangle(0,240,0,320,LCD_COLOR_BLACK);
	while(1){
	   render_board();
	   render_selection();
	   selection();
	   render_board();
		
	}
    
}

