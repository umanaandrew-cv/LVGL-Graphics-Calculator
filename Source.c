/*
    C file authored by Andrew Umana
*/

#include "../../../lv_examples.h"
#include "stdio.h"
#include "time.h"
#include "string.h"
#include "math.h"

//declarations of fonts
LV_FONT_DECLARE(digital_12);
LV_FONT_DECLARE(digital_18);
LV_FONT_DECLARE(digital_24);
LV_FONT_DECLARE(digital_36);
LV_FONT_DECLARE(digital_40);
LV_FONT_DECLARE(digital_72);
LV_FONT_DECLARE(digital7_64);
LV_FONT_DECLARE(nasalization_24);
LV_FONT_DECLARE(digital_play_st_24);
LV_FONT_DECLARE(digital_play_st_36);
LV_FONT_DECLARE(digital_play_st_48);


int operation;
lv_task_t* operations;
lv_task_t* numbers;

//creations of lv_obj_t button objects
lv_obj_t* period_;
lv_obj_t* zero_;
lv_obj_t* one_btn;
lv_obj_t* two_btn;
lv_obj_t* three_btn;
lv_obj_t* four_btn;
lv_obj_t* five_btn;
lv_obj_t* six_btn;
lv_obj_t* seven_btn;
lv_obj_t* eight_btn;
lv_obj_t* nine_btn;
lv_obj_t* clear_btn;
lv_obj_t* add_btn;
lv_obj_t* minus_btn;
lv_obj_t* equal_btn;
lv_obj_t* multiply_btn;
lv_obj_t* negative_btn;
lv_obj_t* divide_btn;
lv_obj_t* in_label;
lv_obj_t* calc_container;

//arrays for determining PEMDAS operations
double numbers_[100];
char operations_[100];
int tracker = 0;

//buffer holding the value
char buff1[256]="";

void calculator();
void createCalLayout();
lv_obj_t* createLabelButton2(int x, int y, int sizeX, int sizeY, const char* text);
void eventButton0(lv_obj_t* obj, lv_event_t event);
void eventButton1(lv_obj_t* obj, lv_event_t event);
void eventButton2(lv_obj_t* obj, lv_event_t event);
void eventButton3(lv_obj_t* obj, lv_event_t event);
void eventButton4(lv_obj_t* obj, lv_event_t event);
void eventButton5(lv_obj_t* obj, lv_event_t event);
void eventButton6(lv_obj_t* obj, lv_event_t event);
void eventButton7(lv_obj_t* obj, lv_event_t event);
void eventButton8(lv_obj_t* obj, lv_event_t event);
void eventButton9(lv_obj_t* obj, lv_event_t event);
void eventButtonAC(lv_obj_t* obj, lv_event_t event);
void eventButtonEQ(lv_obj_t* obj, lv_event_t event);
void eventButtonADD(lv_obj_t* obj, lv_event_t event);
void eventButtonMINUS(lv_obj_t* obj, lv_event_t event);
void eventButtonMULTIPLY(lv_obj_t* obj, lv_event_t event);
void eventButtonDIVIDE(lv_obj_t* obj, lv_event_t event);
void eventButtonNEG(lv_obj_t* obj, lv_event_t event);
void eventButtonPERIOD(lv_obj_t* obj, lv_event_t event);
void taskListener();
void createNumLabel();
void setButtonEvents();
void updateLabel_(char* str);
void resetCalc();
void createCalcContainer();

//creates container holding the Calc value
void createCalcContainer()
{
    calc_container = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_align(calc_container, NULL, LV_ALIGN_CENTER, -68, -77);
    lv_obj_set_height(calc_container, 55); //55
    lv_obj_set_width(calc_container, 220);
    lv_obj_set_drag(calc_container, false);

    lv_obj_set_style_local_border_color(calc_container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

    lv_obj_set_style_local_bg_color(calc_container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_bg_opa(calc_container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);
}

//resets the calc, called in AC button
void resetCalc()
{
    lv_label_set_text(in_label, "0");
    char text[] = "";
    strcpy(buff1, text);
}

//updates the label, only allows numbers of 6 places
void updateLabel_(char* str)
{ 
    if (strlen(str) <= 6)
    {
        lv_label_set_text(in_label, str);
        lv_obj_set_auto_realign(in_label, true);
    }
}

//creates the createNumLabel in the main Container
void createNumLabel()
{
    static lv_style_t style_label_bg;
    lv_style_init(&style_label_bg);

    lv_style_set_pad_left(&style_label_bg, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_label_bg, LV_STATE_DEFAULT, 15);

    lv_style_set_text_color(&style_label_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_label_bg, LV_STATE_DEFAULT, &digital_play_st_24);

    in_label = lv_label_create(calc_container, NULL);
    
    lv_obj_align(in_label, NULL, LV_ALIGN_IN_TOP_RIGHT, 10, 15);
    lv_obj_add_style(in_label, LV_LABEL_PART_MAIN, &style_label_bg);
    lv_obj_set_auto_realign(in_label, true);
    lv_label_set_text(in_label, "0");
}

//creates all the label buttons of this type
lv_obj_t* createLabelButton2(int x, int y, int sizeX, int sizeY, const char* text)
{
    lv_obj_t* obj = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_t* label = lv_label_create(obj, NULL);
    lv_label_set_text(label, text);

    static lv_style_t style_label_bg;
    lv_style_init(&style_label_bg);

    lv_style_set_text_color(&style_label_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_color(&style_label_bg, LV_STATE_DEFAULT, LV_COLOR_RED);

    lv_color_t black_ = lv_color_lighten(LV_COLOR_BLACK, LV_OPA_0);
    lv_obj_set_style_local_bg_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, black_);

    lv_obj_set_ext_click_area(obj, 3, 3, 3, 3);

    lv_obj_align(obj, NULL, LV_ALIGN_CENTER, x, y);
    lv_obj_set_width(obj, sizeX);
    lv_obj_add_style(obj, LV_BTN_PART_MAIN, &style_label_bg);
    lv_obj_set_height(obj, sizeY);

    return obj;
}

//this is the most important function. calculator(); is called in main Simulator and shows the calculator with everything that is needed. 
void calculator()
{
    imageBackground();
    createCalcContainer();
    createCalLayout();
    setButtonEvents();
    createNumLabel();  
}

//listens to new button clicks to update the value in the calculator container
void taskListener()
{
    updateLabel_(buff1);
}

//sets the callback events for each button, ex button 2 will be set to appending 2 to the end of the current calc container value
void setButtonEvents()
{
    lv_obj_set_event_cb(period_, eventButtonPERIOD);
    lv_obj_set_event_cb(zero_, eventButton0);
    lv_obj_set_event_cb(one_btn, eventButton1);
    lv_obj_set_event_cb(two_btn, eventButton2);
    lv_obj_set_event_cb(three_btn, eventButton3);
    lv_obj_set_event_cb(four_btn, eventButton4);
    lv_obj_set_event_cb(five_btn, eventButton5);
    lv_obj_set_event_cb(six_btn, eventButton6);
    lv_obj_set_event_cb(seven_btn, eventButton7);
    lv_obj_set_event_cb(eight_btn, eventButton8);
    lv_obj_set_event_cb(nine_btn, eventButton9);
    lv_obj_set_event_cb(add_btn, eventButtonADD);
    lv_obj_set_event_cb(minus_btn, eventButtonMINUS);
    lv_obj_set_event_cb(clear_btn, eventButtonAC);
    lv_obj_set_event_cb(divide_btn, eventButtonDIVIDE);
    lv_obj_set_event_cb(negative_btn, eventButtonNEG);
    lv_obj_set_event_cb(multiply_btn, eventButtonMULTIPLY);
    lv_obj_set_event_cb(equal_btn, eventButtonEQ);   
}

//creates the buttons and sets them in the screen
void createCalLayout()
{
    int w = 45;
    int h = 25;

    int xoff = 43;
    int yoff = 50;

    period_ = createLabelButton2(xoff + 25, yoff + 55, w, h, ".");
    zero_ = createLabelButton2(xoff - 75, yoff + 55, w + 48, h, "0");

    one_btn = createLabelButton2(xoff - 75, yoff + 25, w, h, "1");
    two_btn = createLabelButton2(xoff - 25, yoff + 25, w, h, "2");
    three_btn = createLabelButton2(xoff + 25, yoff + 25, w, h, "3");

    four_btn = createLabelButton2(xoff - 75, yoff - 5, w, h, "4");
    five_btn = createLabelButton2(xoff - 25, yoff - 5, w, h, "5");
    six_btn = createLabelButton2(xoff + 25, yoff - 5, w, h, "6");

    seven_btn = createLabelButton2(xoff - 75, yoff - 35, w, h, "7");
    eight_btn = createLabelButton2(xoff - 25, yoff - 35, w, h, "8");
    nine_btn = createLabelButton2(xoff + 25, yoff - 35, w, h, "9");

    clear_btn = createLabelButton2(xoff - 75, yoff - 65, w + 25, h, "AC");
    negative_btn = createLabelButton2(xoff, yoff - 65, w + 25, h, "+/-");

    divide_btn = createLabelButton2(xoff + 75, yoff - 65, w, h, "/");
    multiply_btn = createLabelButton2(xoff + 75, yoff - 35, w, h, "x");
    minus_btn = createLabelButton2(xoff + 75, yoff - 5, w, h, "-");
    add_btn = createLabelButton2(xoff + 75, yoff + 25, w, h, "+");
    equal_btn = createLabelButton2(xoff + 75, yoff + 55, w, h, "=");

}

//Callback Events of each button
//set the text, append to buffer and update the label

void eventButton0(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "0";
        strncat(buff1, text, 1);
        taskListener();
    }

}

void eventButton1(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "1";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton2(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "2";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton3(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1)<7))
    {
        char text[] = "3";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton4(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "4";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton5(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "5";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton6(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "6";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton7(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "7";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton8(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "8";
        strncat(buff1, text, 1);
        taskListener();
    }
}

void eventButton9(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1) < 7))
    {
        char text[] = "9";
        strncat(buff1, text, 1);
        taskListener();       
    }
}

void eventButtonAC(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        resetCalc();
        tracker = 0;
    }
}

//Deletes the index of the value of array
void delete_index(double* arr, int size, int index)
{
    int c = 0;

    double temparr[256];
    if (size > 1)
    {
        for (c = 0; c < size - 1; c++)
        {
            if (c >= index)
            {
                temparr[c] = arr[c + 1];
            }
            else
            {
                temparr[c] = arr[c];
            }
        }
    }

    arr = temparr;   
}

//deletes the value of the index array
void delete_index_char(char* arr, int size, int index)
{
    int c = 0;
    char temparr[256];
    if (size > 1)
    {
        for (c = 0; c < size - 1; c++)
        {
            if (c >= index)
            {
                temparr[c] = arr[c + 1];
            }
            else
            {
                temparr[c] = arr[c];
            }
        }
    }

    arr = temparr;

    memset(operations_, 0, (size - 1)*sizeof(double));

    for (int i = 0; i < size-1; i++)
    {
        operations_[i] = temparr[i];
    }    
}

//Callbacks for operations
//Equals button is the most complex

void eventButtonEQ(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        double val = atof(buff1);
        numbers_[tracker] = val;
        tracker = tracker + 1;

        int checker = tracker-1;
        int a = 0;

        while (a < tracker-1)
        {
            if (operations_[a] == 'x')
            {
                if (checker >= 1)
                {
                    numbers_[a] = numbers_[a] * numbers_[a + 1];
                    int dex = a+1 ;
                    delete_index(numbers_, tracker, dex);
                    tracker = tracker - 1;
                }
                if (checker >= 1)
                {
                    delete_index_char(operations_, checker, a);
                }  
                
                a = 0;
            }
            else if (operations_[a] == '/')
            {
                if (checker >= 1)
                {
                    numbers_[a] = numbers_[a] / numbers_[a + 1];
                    int dex = a + 1;
                    delete_index(numbers_, tracker, dex);
                    tracker = tracker - 1;
                }
                if (checker >= 1)
                {
                    delete_index_char(operations_, checker, a);
                }

                a = 0;
            }
            else
            {
                a = a + 1;
            }
        }

        a = 0;

        while (a < tracker-1)
        {
            if (operations_[a] == '+')
            {

               if (checker >= 1)
               {
                  numbers_[a] = numbers_[a] + numbers_[a + 1];
                  int dex = a + 1;
                  delete_index(numbers_, tracker, dex);
                  tracker = tracker - 1;
               }
               if (checker >= 1)
               {
                  delete_index_char(operations_, checker, a);
                }
                a = 0;
            }
            else if (operations_[a] == '-')
            {
                if (checker >= 1)
                {
                   numbers_[a] = numbers_[a] - numbers_[a + 1];
                   int dex = a + 1;
                   delete_index(numbers_, tracker, dex);
                   tracker = tracker - 1;
                }
                if (checker >= 1)
                {
                    delete_index_char(operations_, checker, a);
                }
                a = 0;
            }
            else
            {
                a = a + 1;
            }
        }
        if(strlen(buff1)>9)
            sprintf(buff1, "%.1e", numbers_[0]);
        else
            sprintf(buff1, "%.2f", numbers_[0]);

        double hold = atof(buff1);
        resetCalc();

        if (hold>999999999 )
        {
            strcpy(buff1, "");
            sprintf(buff1, "%.1e", hold);
            lv_label_set_text(in_label, buff1);
        }
        else
        {
            strcpy(buff1, "");
            sprintf(buff1, "%.2f", hold);
            lv_label_set_text(in_label, buff1);
        }

           
        tracker = 0;
    }
}

void eventButtonADD(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && atof(buff1) != 0)
    {
        //holds the int val
        double val = atof(buff1);

        //pushes value to the numbers array
        numbers_[tracker] = val;

        //pushes operation to the numbers array
        operations_[tracker] = '+';

        //increment tracker
        tracker = tracker + 1;

        //clear the buffer
        strcpy(buff1, "");
    }

}

void eventButtonMINUS(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && atof(buff1) != 0)
    {
        //holds the int val
        double val = atof(buff1);

        //pushes value to the numbers array
        numbers_[tracker] = val;

        //pushes operation to the numbers array
        operations_[tracker] = '-';

        //increment tracker
        tracker = tracker + 1;

        //clear the buffer
        strcpy(buff1, "");
    }
}

void eventButtonMULTIPLY(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && atof(buff1) != 0)
    {
        //holds the int val
        double val = atof(buff1);

        //pushes value to the numbers array
        numbers_[tracker] = val;

        //pushes operation to the numbers array
        operations_[tracker] = 'x';

        //increment tracker
        tracker = tracker + 1;

        //clear the buffer
        strcpy(buff1, "");
    }
}

void eventButtonDIVIDE(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && atof(buff1) != 0)
    {
        //holds the int val
        double val = atof(buff1);

        //pushes value to the numbers array
        numbers_[tracker] = val;

        //pushes operation to the numbers array
        operations_[tracker] = '/';

        //increment tracker
        tracker = tracker + 1;

        //clear the buffer
        strcpy(buff1, "");
    }
}

void eventButtonNEG(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        double val = atof(buff1);
        val = val * -1;
        strcpy(buff1, "");

        if (floor(val) == val)
        {
            sprintf(buff1, "%d",(int) val);
        }
        else
            sprintf(buff1, "%.2f", val);

        lv_label_set_text(in_label, buff1);
    }
}

void eventButtonPERIOD(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && (strlen(buff1)<6))
    {
        char text[] = ".";
        strncat(buff1, text, 1);
        taskListener();
    }
}


