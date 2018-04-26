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
                select();
            }
        }
        
        if(AlertButtons)
        {
            AlertButtons=false; //set AlertButton to false
            ready = 1;
            ece210_wireless_send(1);
            select(); //leads to the charater selection page
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
            select();
            
            break;
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
            
            default:
            {
                break;
            }
        }
        
        if(AlertButtons)
        {
            AlertButtons=false;
            if(btn_down_pressed())
            {
                ece210_wireless_send(curIndex);
                character = curIndex;
                play();
                break;
            }
        }
    }
}

void play()        
{
    
}

