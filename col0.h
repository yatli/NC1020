#pragma once
#include <cstdint>

/* col0 reverse engineering, again, after 20 years */

#define LCD_DIGIT0_SEG0         0  
#define LCD_DIGIT0_SEG1         1  
#define LCD_DIGIT0_SEG2         2  
#define LCD_DIGIT0_SEG3         3  
#define LCD_R_CURSOR0           4  
#define LCD_DIGIT1_SEG0         5  
#define LCD_DIGIT1_SEG1         6  
#define LCD_DIGIT1_SEG2         7  
#define LCD_DIGIT1_SEG3         8  
#define LCD_DIGIT_COLON         9  
#define LCD_DIGIT2_SEG0         10 
#define LCD_DIGIT2_SEG1         11 
#define LCD_R_CURSOR1           12 
#define LCD_DIGIT2_SEG2         13 
#define LCD_DIGIT2_SEG3         14 
#define LCD_DIGIT3_SEG0         15 
#define LCD_DIGIT3_SEG1         16 
#define LCD_DIGIT3_SEG2         17 
#define LCD_DIGIT3_SEG3         18 
#define LCD_DIGIT3_SEG4         19 
#define LCD_R_CURSOR2           20 
#define LCD_DIGIT3_SEG5         21 
#define LCD_DIGIT3_SEG6         22 
#define LCD_DIGIT_DOT2          23 
#define LCD_DIGIT2_SEG4         24 
#define LCD_DIGIT2_SEG5         25 
#define LCD_DIGIT2_SEG6         26 
#define LCD_DIGIT_DOT1          27 
#define LCD_R_CURSOR3           28 
#define LCD_DIGIT1_SEG4         29 
#define LCD_DIGIT1_SEG5         30 
#define LCD_DIGIT1_SEG6         31 
#define LCD_DIGIT_DOT0          32 
#define LCD_DIGIT0_SEG4         33 
#define LCD_DIGIT0_SEG5         34 
#define LCD_DIGIT0_SEG6         35 
#define LCD_R_CURSOR4           36 
#define LCD_ICON_STAR           37 
#define LCD_ICON_PGUP           38 
#define LCD_NUM                 39 
#define LCD_ENG                 40 
#define LCD_CAPS                41 
#define LCD_SHIFT               42 
#define LCD_VBAR0               43 
#define LCD_R_CURSOR5           44 
#define LCD_VBAR1               45 
#define LCD_ICON_CHARGE         46 
#define LCD_ICON_BELL           47 
#define LCD_ICON_MUSICNOTE      48 
#define LCD_ICON_BROADCAST      49 
#define LCD_ICON_SPEAKER        50 
#define LCD_ICON_ALARM          51 
#define LCD_R_CURSOR6           52 
#define LCD_ICON_MIC            53 
#define LCD_ICON_TAPE           54 
#define LCD_ICON_SEP            55 
#define LCD_VBAR2               56 
#define LCD_VBAR5               57 
#define LCD_ICON_BATTERY        58 
#define LCD_ICON_KEY            59 
#define LCD_R_CURSOR7           60 
#define LCD_ICON_PGLEFT         61 
#define LCD_ICON_PGRIGHT        62 
#define LCD_SCREEN_ARROW_LEFT   63 
#define LCD_ICON_PGDOWN         64 
#define LCD_VBAR_FRAME          65 
#define LCD_VBAR_DOWN           66 
#define LCD_HBAR0               67 
#define LCD_R_CURSOR8           68 
#define LCD_HBAR1               69 
#define LCD_SCREEN_ARROW_RIGHT  70 
#define LCD_HBAR2               71 
#define LCD_HBAR_FRAME          72 
#define LCD_VBAR8 LAST          73 
#define LCD_R_CURSOR9           74 
#define LCD_VBAR7               75 
#define LCD_VBAR6               76 
#define LCD_VBAR4               77 
#define LCD_VBAR3               78 
#define LCD_VBAR_UP             79 

constexpr uint8_t lcd_digit_nr_seg = 7;
constexpr uint8_t lcd_digit0[] = { LCD_DIGIT0_SEG0, LCD_DIGIT0_SEG1, LCD_DIGIT0_SEG2, LCD_DIGIT0_SEG3, LCD_DIGIT0_SEG4, LCD_DIGIT0_SEG5, LCD_DIGIT0_SEG6 };
constexpr uint8_t lcd_digit1[] = { LCD_DIGIT1_SEG0, LCD_DIGIT1_SEG1, LCD_DIGIT1_SEG2, LCD_DIGIT1_SEG3, LCD_DIGIT1_SEG4, LCD_DIGIT1_SEG5, LCD_DIGIT1_SEG6 };
constexpr uint8_t lcd_digit2[] = { LCD_DIGIT2_SEG0, LCD_DIGIT2_SEG1, LCD_DIGIT2_SEG2, LCD_DIGIT2_SEG3, LCD_DIGIT2_SEG4, LCD_DIGIT2_SEG5, LCD_DIGIT2_SEG6 };
constexpr uint8_t lcd_digit3[] = { LCD_DIGIT3_SEG0, LCD_DIGIT3_SEG1, LCD_DIGIT3_SEG2, LCD_DIGIT3_SEG3, LCD_DIGIT3_SEG4, LCD_DIGIT3_SEG5, LCD_DIGIT3_SEG6 };
