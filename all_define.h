#ifndef ALL_DEFINE_H
#define ALL_DEFINE_H

#include "setwidgetbgp_class.h"

/****** Basic Window Size and Ratio ******/
#define WH_RATIO 1.78    // Ratio between Window Width & Height
#define WIN_W 1080    // Window Original Width
#define WIN_H 1920    // Window Original Height
/****** UI Idx Control ******/
#define START_UI 0    // Start UI Index
#define MODEL_SELECT_UI 1    // Model Select UI Index
#define ROLE_SELECT_UI 2    // Role Select UI Index
#define RECORD_UI 3    // Record List UI Index
#define HELP_UI 4    // Help UI Index
#define GALLERY_UI 5    // Gallery UI Index
#define GAME_UI 6    // Main Game UI Index
/****** Start UI ******/
// Main Button in Start UI Related
#define MAIN_BTN_W 435    // Main Select Function Button's Width
#define MAIN_BTN_H 120    // Main Select Function Button's Height
#define FIRST_BTN_X 405          // Main Menu Scene First Button's X Pos
#define FIRST_BTN_Y 870          // Main Menu Scene First Button's Y Pos
#define SECOND_BTN_X 280      // Main Menu Scene Second Button's X Pos
#define SECOND_BTN_Y 1210    // Main Menu Scene Second Button's Y Pos
#define THIRD_BTN_X 470         // Main Menu Scene Third Button's X Pos
#define THIRD_BTN_Y 1515        // Main Menu Scene Third Button's Y Pos
#define BACK_SIZE 200    // Back Button's Size
#define BACK_X 1720    // Back Button's  X Position
#define BACK_Y 680    // Back Button's  Y Position
#define BOTTOM_COVER_H 120    // Bottom Cover Grass Height
#define BOTTOM_COVER_X 0    // Bottom Cover Grass X Position
#define BOTTOM_COVER_Y 1800    // Bottom Cover Grass Y Position
// Animate Cloud and Bubble in Start UI
#define CLOUD_W 225    // Animate Cloud Width
#define CLOUD_H 90    // Animate Cloud Height
#define CLOUD_Y 180    // Animate Cloud Position Y
#define CLOUD_SPEED 50    // Animate Cloud Move Speed
#define CLOUD_MOVE_STEP 2    // Animate Cloud Move Step
#define ANIMATE_BUBBLE_SIZE 200    // Animate Bubble Size
#define ANIMATE_BUBBLE_SPEED 20    // Animate Bubble Move Speed
#define ANIMATE_BUBBLE_MOVE_STEP 1    // Animate Bubble Move Step
#define ANIMATE_BUBLE_MOVE_TIME_MAX 200    //Animate Bubble Move Time Before Change Direction
/****** Model Select UI ******/
// Main Button in Model Select UI Related
#define SCORE_MODEL_X 180    // Score Model Button's X Position
#define SCORE_MODEL_Y 300    // Score Model Button's Y Position
#define SURVIVE_MODEL_X 80     // Survive Model Button's X Position
#define SURVIVE_MODEL_Y 450    // Survive Model Button's Y Position
#define KING_MODEL_X 180    // King Model Button's X Position
#define KING_MODEL_Y 600    // King Model Button's Y Position
// Model Idx Control
#define SCORE_MODEL 0    // Score Model Index
#define SURVIVE_MODEL 1    // Survive Model Index
#define KING_MODEL 2    // King Model Index
/****** Role Select UI ******/
// Level Select Related
#define LEV_BTN_W 200    // Level Select Button's Width
#define LEV_BTN_H 240    // Level Select Button's Height
#define LEV_BTN_Y 1650    // Level Select Button's Y Position
#define EASY_X 25    // Easy Level Button's X Position
#define NORMAL_X 295    // Normal Level Button's X Position
#define HARD_X 565    // Hard Level Button's X Position
// Lev Index Control
#define EASY_LEV 0    // Easy Level Index
#define NORMAL_LEV 1    // Normal Level Index
#define HARD_LEV 2    // Hard Level Index
#define KING_HARD_CONTROL 3    // Hard Level Control for King Model
#define LEV_ADJUST 200    // For Game Test Only
// Role Select Related
#define SELECT_BTN_SIZE 300    // Role Select Button's Size
#define SELECT_XS_X 100    // Select XS Button Position
#define SELECT_XS_Y 450
#define SELECT_ZJ_X 630    // Select ZJ Button Position
#define SELECT_ZJ_Y 450
#define SELECT_YH_X 100    // Select YH Button Position
#define SELECT_YH_Y 1080
#define SELECT_DC_X 630    // Select DC Button Position
#define SELECT_DC_Y 1080
// Role Idx Control
#define XS_ROLE 0    // XS Index
#define ZJ_ROLE 1    // ZJ Index
#define YH_ROLE 2    // YH Index
#define DC_ROLE 3    // DC Index
    //#define HIDE_ROLE 4    // Hide Index
/****** Record List Board UI ******/
#define RECORD_TEXT_X 150    // Record Text X Position
#define RECORD_BOARD_W 780    // Record Board Width
#define RECORD_BOARD_H 200    // Record Board Height
#define RECORD_TITLE_X 150    // Record Title Y Position
#define RECORD_TITLE_Y 200    // Record Title Y Position
#define RECORD_BTN_SIZE 150    // Record Button Size
#define RECORD_X 15    // First Record Board Y Position
#define RECORD_FIRST_Y 500    // First Record Board Y Position
#define RECORD_SECOND_Y 800    // Second Record Board Y Position
#define RECORD_THIRD_Y 1100    // Third Record Board Y Position
#define RECORD_FOURTH_Y 1400    // Fouth Record Board Y Position
#define RECORD_SCORE 0    // Score Model Record Index
#define RECORD_SURVIVE 1    // Survive Model Record Index
#define RECORD_KING 2    // King Model Record Index

typedef struct
{
    int iScoreLev[4];
    int iTimeRecord[4];
    int iKingLev[5];
} RECORD;   //  Record Property

/****** Gallery UI ******/
#define GALLERY_NUM 12    // Gallery Picture Number

/****** Game UI ******/
// Cover Bar in Game UI
#define COVER_BAR_VERTICAL_W 48    // Left & Right Cover Bar's Width
#define COVER_BAR_RIGHT_X 1872    // Right Cover Bar's X Position
#define COVER_BAR_TOP_H 194    // Top Cover Bar's Height
#define COVER_BAR_BOTTOM_H 179    // Bottom Cover Bar's Height
#define COVER_BAR_BOTTOM_Y 901    // Top Cover Bar's Y Position
// Menu Bar in Game UI
#define MARK_LAB_X 50    // Mark Label X Position
#define MARK_LAB_Y 35    // Mark Label Y Position
#define MARK_LAB_H 100    // Mark Label Width
#define MARK_LAB_W 178    // Mark Label Height
#define RECORD_LAB_Y 35    // Mark Label Y Position
#define RECORD_LAB_H 100    // Record Text Label Height
#define RECORD_LAB_W 400    // Record Text Label Width
#define RECORD_FONT_SIZE 25    // Record Text Font Size
#define CONTROL_BTN_SIZE 120    // Game Control Button Size
#define CONTROL_BTN_Y 30    // Game Control Button Y Position
#define PAUSE_BTN_X 750    // Pause Button X Position
#define BACK_BTN_X 900    // Back Button X Position
// Bubble in Game UI
#define GAME_BUBBLE_SIZE 135    // Click Bubble Size
#define GAME_BUBBLE_NUM 112    // Click Bubble Total Number
#define GAME_BUBBLE_ROW_NUM 12    // Click Bubble Row Number
#define GAME_BUBBLE_COL_NUM 8    // Click Bubble Column Number
#define GAME_BUBBLE_X 0    // Click Bubble X Position
#define GENERATE_BUBBLE_SPEED 100    // New Bubble Generate Speed
#define COVER_BAR_W 10    // Cover Bar Width
#define BUBBLE_REMOVE_MIN_NUM 2    // Bubble Remove Number Identify Control
// Bubble Animate Control
#define BUBBLE_MOVE_TIME 300    // Bubble Move Up Animate Time
#define BUBBLE_COLLECT_TIME 300    // Bubble Vanish Animate Time
#define BUBBLE_FALL_TIME 600    // Bubble Fall Animate Time
/****** Media Control ******/
#define MUSIC_TYPE 0    // Loop Music
#define SOUND_TYPE 1    // One Time Sound
/****** Game Result ******/
#define GAME_FAILED 0    // Game Failed
#define GAME_SUCCEED 1    // Game Succeed
#define GAMING 2    // Gaming
/****** Remind Dialog Type ******/
#define GAME_SUCCEED_DLG 0    // Game Succeed Dialog
#define GAME_FAILED_DLG 1    // Game Failed Dialog
#define SURVIVE_UNLOCK_DLG 2    // Survive Model Unlock Dialog
#define KING_UNLOCK_DLG 3    // King Model Unlock Dialog
#define SURVIVE_LOCKED_DLG 4    // Survive Model Locked Remind Dialog
#define KING_LOCKED_DLG 5    // King Model Locked Remind Dialog
#define CONTINUE_DLG 6    // Continue Current Game Dialog
#define GAME_OVER_DLG 7    // Game Over Dialog
#define NEW_RECORD_DLG 8    // New Record Dialog
#define GAME_PAUSE_DLG 9    // Game Pause Dialog
#define GIVE_UP_DLG 10    // Give Up Current Game Dialog
#define SCORE_REMIND_DLG 11    // Score Model Reminding Dialog
#define SURVIVE_REMIND_DLG 12    // Survive Model Reminding Dialog
#define KING_REMIND_DLG 13    // King Model Reminding Dialog
#define REMIND_X 150    // Remind Dialog X Position
#define REMIND_Y 650    // Remind Dialog Y Position
#define REMIND_W 800    // Remind Dialog Width
#define REMIND_H 600    // Remind Dialog Height
#define REMING_BTN_X1 50    // Remind Dialog Button 1 X Position
#define REMING_BTN_X2 430    // Remind Dialog Button 2 X Position
#define REMING_BTN_Y 430    // Remind Dialog Button Y Position
#define REMING_BTN_W 300   // Remind Dialog Button Width
#define REMING_BTN_H 100   // Remind Dialog Button Height
/****** Help Dlg ******/
#define HELP_LAB_X 50    // Help Label X Position
#define HELP_LAB_Y 50    // Help Label Y Position
#define HELP_LAB_W 400    // Help Label Width
#define HELP_LAB_H 600    // Help Label Height
#define HELP_LEFT_X 200    // Help Left Button X Position
#define HELP_RIGHT_X 550    // Help Right Button X Position
#define HELP_BTN_SIZE 200    // Help Button Size
#define HELP_BTN_Y 1725    // Help Button Y Position
#define HELP_PAGE_X 200    // Help Page X Position
#define HELP_PAGE_W 100    // Help Page Y Pisition
/****** Swip, Interlude & Animate Related ******/
#define MIN_SWIP_SIZE 50    // Minimum Swip Happened Distance
#define ANIMATE_TIME 200    // Swip Animate Time Last
#define ANIMATE_INTER 1    // Swip Animate Time Interval
#define ANIMATE_STEP 100    // Swip Animate Move Step
#define INTERLUDE_INTER 30    // Interlude Animate Speed Interval
#define INTERLUDE_BUBBLE_ROW 10    // Interlude Bubble Row Number
#define INTERLUDE_BUBBLE_COL 6    // Interlude Bubble Col Number
#define INTERLUDE_TIME 1000    // Swip Animate Time Last

#endif // ALL_DEFINE_H
