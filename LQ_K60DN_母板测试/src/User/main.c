/****************************************************************************************************
��ƽ    ̨������K60DN���ܳ�VCĸ��
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
�������汾��V1.0
�������¡�2018��11��12��
��������?�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4������
��Target  ��MK60DN512ZVLQ10
��Crystal �� 50.000Mhz
��busclock��100.000MHz
��pllclock��200.000MHz
=============================================================
K60DNĸ����ʾ����
=================================
-------------------------------------------------------------
LED�ӿڶ��壺
LED1--PTA17
LED2--PTC0
LED3--PTD15 
LED4--PTE26
-------------------------------
����ͷ�ӿڶ��壺
Y0-7    PTD0-7   //D0-D7
HREF    PTC6     //LINE
VSY     PTC7     //FRM
PCK     PTA19    //PIX
VCC     3V3 
GND     GND
SCL     PTE1
SDA     PTE0
-------------------------------
OLEDģ��        ��Ƭ���ӿ�  ���İ��õ���PTC16-19
VCC             3.3V        �û������޸ĳ�ʼ���͹ܽŶ���
GND             GND
RST             PTC13
DC              PTC14
SDA             PTC15
CLK             PTC16
CS              PTC12
-------------------------------
���ڽӿڶ��壺
GND   GND
TX    PTE24   ��������RX
RX    PTE25   ��������TX
���İ��ϴ��ڽӿ��Ѿ����棬ֱ�Ӳ�����ģ�鼴�ɡ�TX˿ӡ��Ӧ��PTE25��RX˿ӡ��Ӧ��PTE24��
�޸���ʷ��
����ĸ��������̣�LED���������������ۣ�ADC��������,���ᣩ--20190125
˵����
��ƽ    ̨������K60DN���ܳ�VCĸ��
ĸ���ܲ���ʱ��K0 K1�������ƶ����?*������K2ȷ��������Զ�Ӧģ��?
���Զ���͵��ʱ�����PWMĬ��ֵΪ��ֵ�����PWMĬ��ֵΪ0��
��K0 K1�ɼӼ�PWM(����Ҽ�?)���в��ԣ���K2�ָ�Ĭ��ֵ��OLED��ʾ��
��λ���ص���ʼ���档
-------------------------------*/////

#include "include.h"
#define     Step_Right       3080 //锟斤拷锟斤拷频锟绞改变，值锟斤拷要锟斤拷锟铰标定
#define     Step_Middle      2800 //锟斤拷锟斤拷频锟绞改变，锟斤拷值锟斤拷要锟斤拷锟铰标定  
#define     Step_Left        2380 //锟斤拷锟斤拷频锟绞改变，值锟斤拷要锟斤拷锟铰标定

// ============ PID Control Parameters ============
typedef struct
{
    float kp;           // Proportional gain
    float ki;           // Integral gain
    float kd;           // Derivative gain
    
    float target;       // Target speed
    float feedback;     // Current feedback speed
    float error;        // Current error
    float last_error;   // Last error for derivative
    float integral;     // Integral accumulator
    
    float output;       // PID output
    float max_output;   // Max output limit
    float min_output;   // Min output limit
} PID_Controller;

volatile int16 g_target_speed = 0;
volatile int16 g_motor_pwm = 0;
volatile int16 g_last_motor_pwm = 0;
volatile int16 g_servo_pwm = Step_Middle;
volatile char g_last_cmd = 0;
volatile unsigned char g_last_cmd_text[16] = "CMD:    ";

// Command flags for synchronization between UART interrupt and main loop
volatile char cmd_flag = 0;  // 'S' for stop, 'R' for running, 0 for none

// PID control enable flag
volatile char g_pid_enabled = 1;  // 1 = PID mode, 0 = direct PWM mode for testing

// PID controller instance for motor speed control
PID_Controller motor_pid = {
    .kp = 1.2f,         // Reduced proportional gain for smoother low-speed response
    .ki = 0.02f,        // Smaller integral gain to avoid windup
    .kd = 0.1f,         // Smaller derivative gain to avoid sudden jumps
    .target = 0,
    .feedback = 0,
    .error = 0,
    .last_error = 0,
    .integral = 0,
    .output = 0,
    .max_output = 300,  // Max PWM output
    .min_output = -300  // Min PWM output
};

// Encoder feedback variables
volatile int16 encoder_count = 0;        // Encoder pulse count per control period
volatile int16 current_speed = 0;        // Current speed (proportional to encoder count)
uint32 control_loop_counter = 0;         // Counter for 50ms control loop
uint32 lcd_diag_counter = 0;             // Counter for 1s LCD diagnostic refresh

uint16_t a[8];
unsigned short send_data[9];
s16 gyro_x,gyro_y,gyro_z;
s16 accle_x,accle_y,accle_z;


// ============ PID Control Functions ============
// Initialize PID controller
void PID_Init(PID_Controller *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->target = 0;
    pid->feedback = 0;
    pid->error = 0;
    pid->last_error = 0;
    pid->integral = 0;
    pid->output = 0;
    pid->max_output = 300;
    pid->min_output = -300;
}

// Calculate PID output
void PID_Calculate(PID_Controller *pid)
{
    const float dt = 0.05f;

    // Calculate error
    pid->error = pid->target - pid->feedback;

    // Reset integral when reversing direction to avoid windup
    if ((pid->error > 0.0f && pid->last_error < 0.0f) ||
        (pid->error < 0.0f && pid->last_error > 0.0f))
    {
        pid->integral = 0.0f;
    }

    // Proportional term
    float p_term = pid->kp * pid->error;

    // Integral term (with anti-windup)
    pid->integral += pid->error * dt;
    if (pid->integral > 200.0f) pid->integral = 200.0f;
    if (pid->integral < -200.0f) pid->integral = -200.0f;
    float i_term = pid->ki * pid->integral;

    // Derivative term (difference from last error)
    float d_term = pid->kd * (pid->error - pid->last_error) / dt;
    pid->last_error = pid->error;

    // Calculate output
    pid->output = p_term + i_term + d_term;

    // Limit output
    if (pid->output > pid->max_output) pid->output = pid->max_output;
    if (pid->output < pid->min_output) pid->output = pid->min_output;

    // Soft limit for low-speed startup: avoid sudden full saturation
    // when the error is still small or the feedback has not started moving.
    if (pid->error > 0.0f && pid->error < 20.0f)
    {
        if (pid->output > 20.0f) pid->output = 20.0f;
    }
    else if (pid->error < 0.0f && pid->error > -20.0f)
    {
        if (pid->output < -20.0f) pid->output = -20.0f;
    }
}

// Reset PID controller
void PID_Reset(PID_Controller *pid)
{
    pid->error = 0;
    pid->last_error = 0;
    pid->integral = 0;
    pid->output = 0;
}

static int16 Clamp_PWM(int16 pwm, int16 min_pwm, int16 max_pwm)
{
    if (pwm < min_pwm) return min_pwm;
    if (pwm > max_pwm) return max_pwm;
    return pwm;
}

static int16 Ramp_PWM(int16 current_pwm, int16 target_pwm, int16 max_step)
{
    int16 diff = target_pwm - current_pwm;

    if (diff > max_step) return current_pwm + max_step;
    if (diff < -max_step) return current_pwm - max_step;
    return target_pwm;
}

// Get encoder feedback and calculate current speed
void Update_Speed_Feedback(void)
{
    // Read encoder pulse count (clears counter on read)
    encoder_count = FTM_AB_Get(FTM1);

    // Speed feedback = encoder pulse count directly (no scaling)
    // encoder_count = pulses in 50ms period, range typically -100 to +100
    current_speed = encoder_count;  // Direct 1:1 mapping from pulses
}

void Drive_Set_Motor_PWM(int16 pwm)
{
    u16 duty = (pwm < 0) ? -pwm : pwm;

    if (pwm > 0)
    {
        FTM_PWM_Duty(FTM0, FTM_CH0, 1000);
        FTM_PWM_Duty(FTM0, FTM_CH1, 1000 - duty);
        FTM_PWM_Duty(FTM0, FTM_CH2, 1000);
        FTM_PWM_Duty(FTM0, FTM_CH3, 1000 - duty);
    }
    else if (pwm < 0)
    {
        FTM_PWM_Duty(FTM0, FTM_CH0, 1000 - duty);
        FTM_PWM_Duty(FTM0, FTM_CH1, 1000);
        FTM_PWM_Duty(FTM0, FTM_CH2, 1000 - duty);
        FTM_PWM_Duty(FTM0, FTM_CH3, 1000);
    }
    else
    {
        FTM_PWM_Duty(FTM0, FTM_CH0, 1000);
        FTM_PWM_Duty(FTM0, FTM_CH1, 1000);
        FTM_PWM_Duty(FTM0, FTM_CH2, 1000);
        FTM_PWM_Duty(FTM0, FTM_CH3, 1000);
    }
}

void Drive_Set_Steering(int16 pwm)
{
    if (pwm < Step_Left) pwm = Step_Left;
    if (pwm > Step_Right) pwm = Step_Right;
    FTM_PWM_Duty(FTM2, FTM_CH1, pwm);
}

static void LCD_Show_Diagnostics(void)
{
    char line1[24];
    char line2[24];
    int16 target_val = (g_target_speed > 0) ? g_target_speed : -g_target_speed;
    int16 current_val = (current_speed > 0) ? current_speed : -current_speed;
    int16 pwm_val = (g_motor_pwm > 0) ? g_motor_pwm : -g_motor_pwm;
    int16 error_val = (motor_pid.error > 0.0f) ? (int16)motor_pid.error : (int16)(-motor_pid.error);

    sprintf(line1, "TAR:%03d CUR:%03d", target_val, current_val);
    sprintf(line2, "PWM:%03d ERR:%03d", pwm_val, error_val);

    LCD_Print(0, 2, (unsigned char *)line1);
    LCD_Print(0, 4, (unsigned char *)line2);
}

//������
void main(void)
{
    DisableInterrupts;           //�ر��ж�
    PLL_Init(PLL200);            //��ʼ��PLLΪ180M
                     
      LCD_Init();                           //LCD��ʼ��//Ĭ��ʹ��:���� OLED SPIģ��
      LCD_CLS();                            //����
      KEY_Init();                           //��ʼ������
      Draw_LQLogo();                        //��ʾ����LOGO
      LCD_P14x16Str(8,0,"Loading..."); //�ַ�����ʾ
     
      LPTMR_delay_ms(1000);           //��ʱ  
       
      LCD_CLS();                      //����
      LCD_Print(0,0,"Remote control");
      LPTMR_delay_ms(50);             //��ʱ  
      EnableInterrupts;            
      LED_Ctrl(LEDALL, OFF);  
      //GPIO_Init (PORTB, 20, GPI,1);     //ͼ��ɼ�����ָʾ��?
    LED_Init();
    GPIO_Init (PORTA, 25, GPO,0);     //����֡�����ţ�ʾ����
    UART_Init(UART4,115200);          //����4��ʼ��
    UART_Irq_En(UART4);               //UART4������ж�?

    Servo_FTM_PWM_Init(FTM2, FTM_CH1, bus_clk * 1000000 / 64 / 50, Step_Middle);
    FTM_PWM_Init(FTM0, FTM_CH0, bus_clk * 1000000 / 12500 / 8, 1000);
    FTM_PWM_Init(FTM0, FTM_CH1, bus_clk * 1000000 / 12500 / 8, 1000);
    FTM_PWM_Init(FTM0, FTM_CH2, bus_clk * 1000000 / 12500 / 8, 1000);
    FTM_PWM_Init(FTM0, FTM_CH3, bus_clk * 1000000 / 12500 / 8, 1000);
    Drive_Set_Motor_PWM(0);
    Drive_Set_Steering(Step_Middle);
    FTM_AB_Init(FTM1);
    
    // Initialize PID controller
    PID_Init(&motor_pid, 2.0f, 0.05f, 0.3f);

    UART_Put_Str(UART4, "Remote control ready (PID mode)\r\n");

    while(1)
    {
        // Check for stop command
        if (cmd_flag == 'S')
        {
            // Stop command takes priority
            g_target_speed = 0;
            g_motor_pwm = 0;
            PID_Reset(&motor_pid);
            Drive_Set_Motor_PWM(0);
            cmd_flag = 0;  // Clear flag
            LPTMR_delay_ms(50);
            continue;  // Skip PID calculation
        }
        
        // Update speed feedback from encoder every 50ms
        Update_Speed_Feedback();
        
        if (g_pid_enabled)
        {
            // PID closed-loop control mode
            // target_speed: -250 to +250 (target pulse count per 50ms)
            // current_speed: encoder count per 50ms
            motor_pid.target = (float)g_target_speed;
            motor_pid.feedback = (float)current_speed;

            if (g_target_speed == 0)
            {
                PID_Reset(&motor_pid);
                g_motor_pwm = 0;
            }
            else if ((g_target_speed > 0 && g_target_speed < 180) ||
                     (g_target_speed < 0 && g_target_speed > -180))
            {
                // Low-speed / mid-speed zone: use the manual PWM curve below 180.
                // Above this, we switch to the PID path so the output will not be
                // stuck at the mid-speed mapping value such as 160.
                int16 abs_target = (g_target_speed > 0) ? g_target_speed : -g_target_speed;
                int16 desired_pwm = 0;
                int16 max_step = 6;

                if (abs_target <= 60)
                {
                    desired_pwm = (int16)(70.0f + abs_target * 1.0f);
                    max_step = 8;
                }
                else if (abs_target <= 140)
                {
                    desired_pwm = (int16)(130.0f + (abs_target - 60) * 0.4f);
                    max_step = 8;
                }
                else
                {
                    desired_pwm = (int16)(148.0f + (abs_target - 140) * 0.2f);
                    max_step = 8;
                }

                desired_pwm = Clamp_PWM(desired_pwm, 70, 180);

                if (g_target_speed > 0)
                {
                    if (g_motor_pwm <= 0)
                    {
                        g_motor_pwm = 70;
                    }
                    else
                    {
                        g_motor_pwm = Ramp_PWM(g_motor_pwm, desired_pwm, max_step);
                    }
                }
                else
                {
                    if (g_motor_pwm >= 0)
                    {
                        g_motor_pwm = -70;
                    }
                    else
                    {
                        g_motor_pwm = Ramp_PWM(g_motor_pwm, -desired_pwm, max_step);
                    }
                }
            }
            else
            {
                // High-speed zone: use a gentle PID output that avoids sudden saturation.
                PID_Calculate(&motor_pid);
                float raw_pwm = motor_pid.output;

                if (raw_pwm > 0.0f)
                {
                    if (raw_pwm < 60.0f) raw_pwm = 60.0f;
                    if (raw_pwm > 240.0f) raw_pwm = 240.0f;
                }
                else if (raw_pwm < 0.0f)
                {
                    if (raw_pwm > -60.0f) raw_pwm = -60.0f;
                    if (raw_pwm < -240.0f) raw_pwm = -240.0f;
                }
                else
                {
                    raw_pwm = 0.0f;
                }

                g_motor_pwm = (int16)raw_pwm;
            }
        }
        else
        {
            // Direct PWM mode - g_motor_pwm is set directly via UART
            // Used for motor diagnostics when encoder feedback is unavailable
        }

        if (g_motor_pwm > -5 && g_motor_pwm < 5)
        {
            g_motor_pwm = 0;
        }

        g_last_motor_pwm = g_motor_pwm;
        
        // Apply motor control
        Drive_Set_Motor_PWM(g_motor_pwm);

        lcd_diag_counter++;
        if (lcd_diag_counter >= 20)
        {
            lcd_diag_counter = 0;
            LCD_Show_Diagnostics();
        }
        
        // Delay for 50ms control loop
        LPTMR_delay_ms(50);
    }
}



