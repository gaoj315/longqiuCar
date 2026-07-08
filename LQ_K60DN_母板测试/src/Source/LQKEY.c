/******************** LQ_K60_ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ v1.0 ********************
 * ï¿½Ä¼ï¿½ï¿½ï¿½           ï¿½ï¿½LQKEY.c
 * ï¿½ï¿½ï¿½ï¿½             ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
 * ï¿½ï¿½×¢             ï¿½ï¿½ï¿½Ù·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Þ¸ï¿½
 * ï¿½ï¿½ï¿½ï¿½             ï¿½ï¿½2016-09-01
 * Êµï¿½ï¿½Æ½Ì¨         ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ K60VGï¿½ï¿½ï¿½Ä°ï¿½
 * ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½         ï¿½ï¿½IAR 7.3
 * ï¿½ï¿½ï¿½ï¿½             ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 006
 * ï¿½Ô±ï¿½ï¿½ï¿½           ï¿½ï¿½https://longqiu.taobao.com
 * ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü³ï¿½ï¿½ï¿½ï¿½ï¿½Èº ï¿½ï¿½202949437
 *************************************************************/

#include "include.h"
#define Step_Right 3600  // ï¿½ï¿½ï¿½ï¿½Æµï¿½Ê¸Ä±ä£¬Öµï¿½ï¿½Òªï¿½ï¿½ï¿½Â±ê¶¨
#define Step_Middle 2750 // ï¿½ï¿½ï¿½ï¿½Æµï¿½Ê¸Ä±ä£¬ï¿½ï¿½Öµï¿½ï¿½Òªï¿½ï¿½ï¿½Â±ê¶¨
#define Step_Left 1150   // ï¿½ï¿½ï¿½ï¿½Æµï¿½Ê¸Ä±ä£¬Öµï¿½ï¿½Òªï¿½ï¿½ï¿½Â±ê¶¨
//-------------------------------------------------------------------------*
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½: KEY_Initï¿½ï¿½ï¿½ï¿½                                                      *
// ï¿½ï¿½  ï¿½ï¿½: ï¿½ï¿½Ê¼ï¿½ï¿½IOï¿½ï¿½                                                      *
// ï¿½ï¿½  ï¿½ï¿½:                                                                *
// ï¿½ï¿½  ï¿½ï¿½: ï¿½ï¿½                                                              *
// ï¿½ï¿½  ï¿½ï¿½: KEY_Initï¿½ï¿½ï¿½ï¿½                                                     *
//-------------------------------------------------------- -----------------*
void KEY_Init(void)
{
  GPIO_Init(PORTB, 20, GPI_UP, 1);
  GPIO_Init(PORTB, 21, GPI_UP, 1);
  GPIO_Init(PORTB, 22, GPI_UP, 1);
  GPIO_Init(PORTC, 8, GPO, 1);
  GPIO_Init(PORTC, 9, GPO, 1);
  GPIO_Init(PORTC, 10, GPO, 1);
  GPIO_Init(PORTC, 11, GPO, 1);
}
/**************************************************************************
ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Öµï¿½ï¿½ï¿½ï¿??
ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½int
ï¿½ï¿½ï¿½ï¿½  Öµï¿½ï¿½unsigned int
**************************************************************************/
int My_Abs(int v)
{
  if (v < 0)
    v = -v;
  return v;
}
//-------------------------------------------------------------------------*
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½: KEY_Readï¿½ï¿½ï¿½ï¿½                                                      *
// ï¿½ï¿½  ï¿½ï¿½: ï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½Öµ                                                       *
// ï¿½ï¿½  ï¿½ï¿½:                                                                *
// ï¿½ï¿½  ï¿½ï¿½:                                                              *
// ï¿½ï¿½  ï¿½ï¿½: KEY_Read( KEY0)                                                *
//-------------------------------------------------------------------------*
u8 KEY_Read(KEYn_e keyno)
{

  switch (keyno)
  {
  case KEY0:
    return GPIO_Get(PTB20);
    break;

  case KEY1:
    return GPIO_Get(PTB21);
    break;

  case KEY2:
    return GPIO_Get(PTB22);
    break;

  case KEY3: // KEY3-KEY6ï¿½Ç²ï¿½ï¿½ë¿ªï¿½Ø¡ï¿½PTC8-11
    return GPIO_Get(PTC8);
    break;

  case KEY4:
    return GPIO_Get(PTC9);
    break;

  case KEY5:
    return GPIO_Get(PTC10);
    break;

  case KEY6:
    return GPIO_Get(PTC11);
    break;

  default:
    return 0XFF;
    break;
  }
}
/*****************************kEY************************/
u8 LQ_KEY_Read(u8 mode)
{
  static u8 key_up = 1; // ï¿½ï¿½ï¿½ï¿½ï¿½É¿ï¿½ï¿½ï¿½Ö¾
  if (mode == 1)
  {
    key_up = 1; // Ö§ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
  }
  if (key_up && (KEY_Read(KEY0) == 0) || KEY_Read(KEY1) == 0 || KEY_Read(KEY2) == 0)
  {
    LPTMR_delay_ms(100); // ï¿½ï¿½ï¿½ï¿½
    key_up = 0;
    if (KEY_Read(KEY1) == 0) // KEY1
    {
      return 1;
    }
    else if (KEY_Read(KEY2) == 0) // KEY2
    {
      return 2;
    }
    else if (KEY_Read(KEY0) == 0) // KEY0
    {
      return 3;
    }
  }
  if (KEY_Read(KEY0) == 1 && KEY_Read(KEY1) == 1 && KEY_Read(KEY2) == 1)
  {
    key_up = 1;
  }
  return 0; // ï¿½Þ°ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
}

void Test_ALL(void)
{
    int LPULSE = 0, ratio;
    float time = 0;
    char txt[16] = "X:";
    
    static int key_count = 1;
    // ÒýÈë×´Ì¬¿ØÖÆ£º0-Ö÷²Ëµ¥£¬1-ÕýÔÚ½øÐÐ×Ó²âÊÔ
    int run_status = 0; 
    int last_key_count = 0; // ÓÃÓÚÅÐ¶Ï¹â±êÊÇ·ñ¸Ä±ä£¬±ÜÃâLCDÉÁË¸

    // ³õÊ¼»¯Ò»Ð©²»ÐèÒªÖØ¸´Ë¢ÐÂµÄ»ù´¡½çÃæ
    LCD_CLS();

    while (1)
    {
        // ==================== ×´Ì¬ 0£ºÖ÷²Ëµ¥Ä£Ê½ ====================
        if (run_status == 0)
        {
            LED_Ctrl(LED1, RVS); // LEDÖ¸Ê¾Ö÷²Ëµ¥ÔËÐÐ×´Ì¬
            
            // Ö»ÓÐ¹â±êÎ»ÖÃ±äÁË²ÅÖØ»æ²Ëµ¥£¬³¹µ×½â¾öÉÁË¸ÎÊÌâ
            if (key_count != last_key_count)
            {
                LCD_P6x8Str(20, 0, "LQ_Test_ALL");
                LCD_P6x8Str(8, 1, "LED_test");
                LCD_P6x8Str(8, 2, "Servo_test");
                LCD_P6x8Str(8, 3, "Motor_test");
                LCD_P6x8Str(8, 4, "LQV034_test");
                LCD_P6x8Str(8, 5, "FTM_AB_test");
                LCD_P6x8Str(8, 6, "ADC_test");
                LCD_P6x8Str(8, 7, "9AX_test");  
                LCD_P6x8Str(68, 7, "Switch_T"); 

                // Çå³ýËùÓÐ¹â±ê£¬²¢¸ù¾Ý key_count ´òÓ¡µ±Ç°¹â±ê '*'
                for(int i = 1; i <= 7; i++) LCD_P6x8Str(0, i, (uint8_t *)" ");
                LCD_P6x8Str(60, 7, (uint8_t *)" ");
                
                if(key_count <= 7) 
                    LCD_P6x8Str(0, key_count, (uint8_t *)"*");
                else 
                    LCD_P6x8Str(60, 7, (uint8_t *)"*");

                last_key_count = key_count;
            }

            // Ö÷²Ëµ¥°´¼ü´¦Àí
            switch (LQ_KEY_Read(0))
            {
                case 1: key_count++; break; // KEY0: ÏÂÒÆ
                case 3: key_count--; break; // KEY1: ÉÏÒÆ
                case 2: run_status = 1;  break; // KEY2: È·¶¨½øÈë×Ó²âÊÔ
                default: break;
            }

            // ±ß½çÑ­»·
            if (key_count > 8)  key_count = 1;
            if (key_count < 1)  key_count = 8;
            
            LPTMR_delay_ms(50); // ÊÊµ±ÑÓÊ±£¬ÎÈ¶¨°´¼ü¼ì²â
        }
        // ==================== ×´Ì¬ 1£º×Ó²âÊÔÄ£Ê½ ====================
        else if (run_status == 1)
        {
            LCD_CLS(); // ½øÈë×Ó²âÊÔ£¬ÇåÒ»´ÎÆÁ

            // ----------- 1. LED ²âÊÔ -----------
            if (key_count == 1)
            {
                LCD_P6x8Str(1, 1, "LED_test");
                LCD_P6x8Str(1, 3, "Press KEY2 to Exit");
                while (1)
                {
                    LED_Ctrl(LEDALL, RVS);
                    LPTMR_delay_ms(200);
                    // ºËÐÄÐÞ¸´£ºÄÚ²¿±ØÐëÌá¹©ÍË³ö»úÖÆ
                    if (LQ_KEY_Read(0) == 2) { run_status = 0; break; } 
                }
            }
            // ----------- 2. ¶æ»ú²âÊÔ -----------
            else if (key_count == 2)
            {
                Servo_FTM_PWM_Init(FTM2, FTM_CH1, bus_clk * 1000000 / 64 / 50, Step_Middle);
                int servopwm = 0;
                LCD_P6x8Str(1, 1, "Servo_test");
                LCD_P6x8Str(0, 7, "KEY2:Exit K0/K1:+-");

                while (1)
                {
                    int key = LQ_KEY_Read(0);
                    if (key == 2) { run_status = 0; break; } // KEY2 ÍË³ö
                    
                    if (key == 1) {
                        servopwm -= 50;
                        if (servopwm < -1200) servopwm = -1200;
                    }
                    else if (key == 3) {
                        servopwm += 50;
                        if (servopwm > 1300) servopwm = 1300;
                    }

                    FTM_PWM_Duty(FTM2, FTM_CH1, Step_Middle + servopwm);
                    time = (Step_Middle + servopwm) * 20.0 / 31250.0;

                    // UI Ë¢ÐÂ
                    sprintf((char *)txt, "servopwm:%06d", Step_Middle + servopwm);
                    LCD_P6x8Str(0, 3, (uint8 *)txt);
                    LCD_P6x8Str(0, 4, "High level time:   ");
                    sprintf((char *)txt, "time:%4.3f ms", time);
                    LCD_P6x8Str(0, 5, (uint8 *)txt);
                    
                    LPTMR_delay_ms(50); // ·À¶¶¼Ó¿ØËÙ
                }
            }
            // ----------- 3. µç»ú²âÊÔ -----------
            else if (key_count == 3)
            {
                FTM_PWM_Init(FTM0, FTM_CH0, bus_clk * 1000000 / 12500 / 8, 0);
                FTM_PWM_Init(FTM0, FTM_CH1, bus_clk * 1000000 / 12500 / 8, 0);
                FTM_PWM_Init(FTM0, FTM_CH2, bus_clk * 1000000 / 12500 / 8, 0);
                FTM_PWM_Init(FTM0, FTM_CH3, bus_clk * 1000000 / 12500 / 8, 0);
                short motorpwm = 0;
                LCD_P6x8Str(1, 1, "Motor_test");

                while (1)
                {
                    int key = LQ_KEY_Read(0);
                    if (key == 2) { 
                        // ÍË³öÇ°Ç¿ÐÐ¹Øµç»ú£¬·ÀÖ¹·èÅÜ
                        FTM_PWM_Duty(FTM0, FTM_CH0, 1000); FTM_PWM_Duty(FTM0, FTM_CH1, 1000);
                        FTM_PWM_Duty(FTM0, FTM_CH2, 1000); FTM_PWM_Duty(FTM0, FTM_CH3, 1000);
                        run_status = 0; 
                        break; 
                    }

                    if (key == 1 || key == 3) // ¼ÓËÙ»ò¼õËÙ
                    {
                        motorpwm += (key == 3) ? 50 : -50;
                        if (motorpwm > 400)  motorpwm = 400;
                        if (motorpwm < -400) motorpwm = -400;

                        int temp_pwm = (motorpwm < 0) ? -motorpwm : motorpwm;
                        if (motorpwm < 0) {
                            FTM_PWM_Duty(FTM0, FTM_CH0, 1000 - temp_pwm); FTM_PWM_Duty(FTM0, FTM_CH1, 1000);
                            FTM_PWM_Duty(FTM0, FTM_CH2, 1000 - temp_pwm); FTM_PWM_Duty(FTM0, FTM_CH3, 1000);
                        } else {
                            FTM_PWM_Duty(FTM0, FTM_CH0, 1000); FTM_PWM_Duty(FTM0, FTM_CH1, 1000 - temp_pwm);
                            FTM_PWM_Duty(FTM0, FTM_CH2, 1000); FTM_PWM_Duty(FTM0, FTM_CH3, 1000 - temp_pwm);
                        }
                        ratio = temp_pwm / 10;
                    }

                    sprintf((char *)txt, "motorpwm:%06d", motorpwm);
                    LCD_P6x8Str(0, 3, (uint8 *)txt);
                    sprintf((char *)txt, "Ratio:%02d%%    ", ratio);
                    LCD_P6x8Str(0, 5, (uint8 *)txt);
                    LPTMR_delay_ms(50);
                }
            }
            // ----------- 4. ÉãÏñÍ·²âÊÔ -----------
            else if (key_count == 4)
            {
                LCD_P6x8Str(1, 1, "LQV034_test");
                LQMT9V034_Init();
                while (1)
                {
                    TEST_LQV034(); // ×¢Òâ£ºÈ·±£¸Ã²âÊÔº¯ÊýÄÚ²¿²»»áËøËÀ£¬»òÕßÄÜÊ¶±ðÍË³ö¼ü
                    if (LQ_KEY_Read(0) == 2) { run_status = 0; break; }
                }
            }
            // ----------- 5. ±àÂëÆ÷²âÊÔ -----------
            else if (key_count == 5)
            {
                FTM_AB_Init(FTM1);
                PIT_Init(PIT0, 50);
                LCD_P6x8Str(1, 1, "FTM_AB_test");
                while (1)
                {
                    LPULSE = FTM_AB_Get(FTM1);
                    sprintf((char *)txt, "LPULSE:%06d", LPULSE);
                    LCD_P6x8Str(0, 2, (uint8 *)txt);
                    LPTMR_delay_ms(100);
                    if (LQ_KEY_Read(0) == 2) { run_status = 0; break; }
                }
            }
            // ----------- 6. ADC²âÊÔ -----------
            else if (key_count == 6)
            {
                ADC_Init(ADC0); ADC_Init(ADC1);
                uint16_t adc1, adc2, adc3, adc4;
                LCD_P6x8Str(1, 1, "ADC_test");
                while (1)
                {
                    adc1 = ADC_Ave(ADC1, ADC1_SE8, ADC_12bit, 50);
                    adc2 = ADC_Ave(ADC1, ADC1_SE9, ADC_12bit, 50);
                    adc3 = ADC_Ave(ADC0, ADC0_SE12, ADC_12bit, 50);
                    adc4 = ADC_Ave(ADC0, ADC1_SE13, ADC_12bit, 50);
                    
                    sprintf((char *)txt, "AD0:%04d AD1:%04d", adc1, adc2);
                    LCD_P6x8Str(0, 2, (uint8 *)txt);
                    sprintf((char *)txt, "AD2:%04d AD3:%04d", adc3, adc4);
                    LCD_P6x8Str(0, 4, (uint8 *)txt);
                    
                    LPTMR_delay_ms(100);
                    if (LQ_KEY_Read(0) == 2) { run_status = 0; break; }
                }
            }
            // ----------- 7. ¾ÅÖá²âÊÔ -----------
            else if (key_count == 7)
            {
                LCD_P6x8Str(1, 1, "9AX_test");
                while (1)
                {
                    Test_9AX();
                    if (LQ_KEY_Read(0) == 2) { run_status = 0; break; }
                }
            }
            // ----------- 8. ²¦Âë/°´¼ü¿ª¹Ø²âÊÔ -----------
            else if (key_count == 8)
            {
                LCD_P6x8Str(1, 1, "Switch_Test");
                while (1)
                {
                    if (LQ_KEY_Read(0) == 2) { 
                        LED_Ctrl(LEDALL, OFF);
                        run_status = 0; 
                        break; 
                    }
                    // ÄãµÄÔ­¿ª¹ØÂß¼­...
                    if ((KEY_Read(KEY3) == 0) || KEY_Read(KEY4) == 0 || KEY_Read(KEY5) == 0 || KEY_Read(KEY6) == 0)
                    {
                        LPTMR_delay_ms(20); // ÉÔÎ¢¼õÉÙÑÓÊ±Ìá¸ßÏìÓ¦
                        if (KEY_Read(KEY3) == 0)      { LED_Ctrl(LED0, ON);  LED_Ctrl(LED1, OFF); LED_Ctrl(LED2, OFF); LED_Ctrl(LED3, OFF); }
                        else if (KEY_Read(KEY4) == 0) { LED_Ctrl(LED0, OFF); LED_Ctrl(LED1, ON);  LED_Ctrl(LED2, OFF); LED_Ctrl(LED3, OFF); }
                        else if (KEY_Read(KEY5) == 0) { LED_Ctrl(LED0, OFF); LED_Ctrl(LED1, OFF); LED_Ctrl(LED2, ON);  LED_Ctrl(LED3, OFF); }
                        else if (KEY_Read(KEY6) == 0) { LED_Ctrl(LED0, OFF); LED_Ctrl(LED1, OFF); LED_Ctrl(LED2, OFF); LED_Ctrl(LED3, ON);  }
                    }
                    else
                    {
                        LED_Ctrl(LEDALL, OFF);
                    }
                }
            }

            // ÍË³ö×Ó²âÊÔ»Øµ½Ö÷²Ëµ¥Ê±£¬Ç¿ÖÆÇåÆÁ²¢´¥·¢²Ëµ¥ÖØ»æ
            LCD_CLS();
            last_key_count = 0; 
        }
    }
}