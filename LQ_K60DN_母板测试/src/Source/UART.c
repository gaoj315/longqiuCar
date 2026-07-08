/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��UART.c
 * ����             ������UART�ڹ���ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2015-10-16
 * ʵ��ƽ̨         ������ k60DN512Z���İ� 
 * ��������         ��IAR 7.3
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#include "include.h"
#include "UART.h"

#define     Step_Right       3080 //����Ƶ�ʸı䣬ֵ��Ҫ���±궨
#define     Step_Middle      2800 //����Ƶ�ʸı䣬��ֵ��Ҫ���±궨  
#define     Step_Left        2400 //����Ƶ�ʸı䣬ֵ��Ҫ���±궨

extern volatile int16 g_target_speed;
extern volatile int16 g_motor_pwm;
extern volatile int16 g_servo_pwm;
extern volatile int16 encoder_count;    // Encoder pulse count from FTM1
extern volatile int16 current_speed;    // Current speed feedback
extern volatile char cmd_flag;          // Command flag for synchronization
extern volatile char g_pid_enabled;     // PID enable flag
extern void Drive_Set_Motor_PWM(int16 pwm);
extern void Drive_Set_Steering(int16 pwm);

UART_MemMapPtr UARTN[UART_MAX] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; 

//-------------------------------------------------------------------------*
//������: uart_init                                                        
//��  ��: ��ʼ��UART                                                        
//��  ��: uratn:ģ�����磺UART0 
//        baud: ������
//��  ��: ��                                                              
//��  ��: uart_init(UART4,115200);UART4��Ӧ���Ųο�UART.H�ļ�                                   
//-------------------------------------------------------------------------*

void UART_Init (UARTn_e uratn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    uint32 sysclk;     //ʱ��

    /* ���� UART���ܵ� ���ùܽ� */
    switch(uratn)
    {
    case UART0:
        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UART0 ʱ��

        if(UART0_RX == PTA1)
        {                    
            PORTA_PCR1= PORT_PCR_MUX(2);       //ʹ��PTA1���ŵڶ����ܣ���UART0_RXD
        }
        else if(UART0_RX == PTA15)
        {
             PORTA_PCR15= PORT_PCR_MUX(3);       //ʹ��PTA15���ŵ�3����
        }
        else if(UART0_RX == PTB16)
        {
             PORTB_PCR16= PORT_PCR_MUX(3);       //ʹ��PTB16���ŵ�3����
        }
        else if(UART0_RX == PTD6)
        {
             PORTD_PCR6= PORT_PCR_MUX(3);       //ʹ��PTD6���ŵ�3����
        }
        else
        {
            break;
        }

        if(UART0_TX == PTA2)
        {
             PORTA_PCR2= PORT_PCR_MUX(2);       //ʹ��PTA2���ŵڶ�����
        }
        else if(UART0_TX == PTA14)
        {
             PORTA_PCR14= PORT_PCR_MUX(3);       //PTA14
        }
        else if(UART0_TX == PTB17)
        {
             PORTB_PCR17= PORT_PCR_MUX(3);       //PTB17
        }
        else if(UART0_TX == PTD7)
        {
             PORTD_PCR7= PORT_PCR_MUX(3);       //PTD7
        }
        else
        {
             break;
        }

        break;

    case UART1:
        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

        if(UART1_RX == PTC3)
        {
             PORTC_PCR3= PORT_PCR_MUX(3);       //PTC3
        }
        else if(UART1_RX == PTE1)
        {
             PORTE_PCR1= PORT_PCR_MUX(3);       //PTE1
        }
        else
        {
            break;
        }

        if(UART1_TX == PTC4)
        {
             PORTC_PCR4= PORT_PCR_MUX(3);       //PTC4
        }
        else if(UART1_TX == PTE0)
        {
             PORTE_PCR0= PORT_PCR_MUX(3);       //PTE0
        }
        else
        {
            break;
        }

        break;

    case UART2:
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
         PORTD_PCR3= PORT_PCR_MUX(3);       //PTD3
         PORTD_PCR2= PORT_PCR_MUX(3);       //PTD2
        break;

    case UART3:
        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

        if(UART3_RX == PTB10)
        {
             PORTB_PCR10= PORT_PCR_MUX(3);       //PTB10
        }
        else if(UART3_RX == PTC16)
        {
             PORTC_PCR16= PORT_PCR_MUX(3);       //PTC16
        }
        else if(UART3_RX == PTE5)
        {
             PORTE_PCR5= PORT_PCR_MUX(3);       //PTE5
        }
        else
        {
             break;
        }

        if(UART3_TX == PTB11)
        {
             PORTB_PCR11= PORT_PCR_MUX(3);       //PTB11
        }
        else if(UART3_TX == PTC17)
        {
             PORTC_PCR17= PORT_PCR_MUX(3);       //PTC17
        }
        else if(UART3_TX == PTE4)
        {
             PORTE_PCR4= PORT_PCR_MUX(3);       //PTE4
        }
        else
        {
             break;
        }
        break;

    case UART4:
        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;

        if(UART4_RX == PTC14)
        {
             PORTC_PCR14= PORT_PCR_MUX(3);       //PTC14
        }
        else if(UART4_RX == PTE25)
        {
             PORTE_PCR25= PORT_PCR_MUX(3);       //PTE25
        }
        else
        {
            break;
        }

        if(UART4_TX == PTC15)
        {
             PORTC_PCR15= PORT_PCR_MUX(3);       //PTC15
        }
        else if(UART4_TX == PTE24)
        {
             PORTE_PCR24= PORT_PCR_MUX(3);       //PTE24
        }
        else
        {
             break;
        }
        break;

    case UART5:
        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

        if(UART5_RX == PTD8)
        {
             PORTD_PCR8= PORT_PCR_MUX(3);       //PTD8
        }
        else if(UART5_RX == PTE9)
        {
             PORTE_PCR9= PORT_PCR_MUX(3);       //PTE9
        }
        else
        {
             break;
        }

        if(UART5_TX == PTD9)
        {
             PORTD_PCR9= PORT_PCR_MUX(3);       //PTD9
        }
        else if(UART5_TX == PTE8)
        {
             PORTE_PCR8= PORT_PCR_MUX(3);       //PTE8
        }
        else
        {
             break;
        }
        break;
    default:
        break;
    }

    //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
    UART_C2_REG(UARTN[uratn]) &= ~(0
                                   | UART_C2_TE_MASK
                                   | UART_C2_RE_MASK
                                  );


    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ���? C1 ʵ�֣�
    UART_C1_REG(UARTN[uratn]) |= (0
                                  //| UART_C2_M_MASK                    //9 λ�� 8 λģʽѡ�� : 0 Ϊ 8λ ��1 Ϊ 9λ��ע���˱�ʾ0����8λ�� �������?9λ��λ8��UARTx_C3�
                                  //| UART_C2_PE_MASK                   //��żУ��ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_PT_MASK                   //У��λ���� : 0 Ϊ żУ�� ��1 Ϊ ��У��
                                 );

    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ��busʱ��
    if ((uratn == UART0) || (uratn == UART1))
    {
        sysclk = core_clk * 1000*1000;                                   //�ں�ʱ��
    }
    else
    {
        sysclk =  bus_clk * 1000*1000;                                    //busʱ��(���ں�ʱ�ӵ�һ��)
    }

    //UART ������ = UART ģ��ʱ�� / (16 �� (SBR[12:0] + BRFA))
    //������ BRFA ������£�? SBR = UART ģ��ʱ�� / (16 * UART ������)
    sbr = (uint16)(sysclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR �� 13bit������? 0x1FFF

    //��֪ SBR ���� BRFA =  = UART ģ��ʱ�� / UART ������ - 16 ��SBR[12:0]
    brfa = (sysclk / baud)  - (sbr * 16);
    

    //д SBR
    temp = UART_BDH_REG(UARTN[uratn]) & (~UART_BDH_SBR_MASK);           //���� ���? SBR �� UARTx_BDH��ֵ
    UART_BDH_REG(UARTN[uratn]) = temp |  UART_BDH_SBR(sbr >> 8);        //��д��SBR��λ
    UART_BDL_REG(UARTN[uratn]) = UART_BDL_SBR(sbr);                     //��д��SBR��λ

    //д BRFD
    temp = UART_C4_REG(UARTN[uratn]) & (~UART_C4_BRFA_MASK) ;           //���� ���? BRFA �� UARTx_C4 ��ֵ
    UART_C4_REG(UARTN[uratn]) = temp |  UART_C4_BRFA(brfa);             //д��BRFA



    //����FIFO(FIFO���������Ӳ�������ģ�������������?)
    UART_PFIFO_REG(UARTN[uratn]) |= (0
                                     | UART_PFIFO_TXFE_MASK               //ʹ��TX FIFO(ע�ͱ�ʾ��ֹ)
                                     //| UART_PFIFO_TXFIFOSIZE(0)         //��ֻ����TX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                                     | UART_PFIFO_RXFE_MASK               //ʹ��RX FIFO(ע�ͱ�ʾ��ֹ)
                                     //| UART_PFIFO_RXFIFOSIZE(0)         //��ֻ����RX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                                    );

    /* �������ͺͽ��� */
    UART_C2_REG(UARTN[uratn]) |= (0
                                  | UART_C2_TE_MASK                     //����ʹ��
                                  | UART_C2_RE_MASK                     //����ʹ��
                                  //| UART_C2_TIE_MASK                  //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                  //| UART_C2_TCIE_MASK                 //��������ж�ʹ�ܣ��?���˱�ʾ���ã�
                                  //| UART_C2_RIE_MASK                  //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                                 );


}

//-------------------------------------------------------------------------*
//������: uart_getchar                                                        
//��  ��: ����һ���ֽ�                                                        
//��  ��: uratn:ģ�����磺UART0 
//        ch: �����������ָ��?
//��  ��: ��                                                              
//��  ��: uart_getchar (UART4,a[]) �����յ������ݱ�����a������                                
//-------------------------------------------------------------------------*
char UART_Get_Char (UARTn_e uratn)
{

    while (!(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK));       //�ȴ���������

    //*ch =  UART_D_REG(UARTN[uratn]);
    return(UART_D_REG(UARTN[uratn]));

}


//-------------------------------------------------------------------------*
//������: uart_querychar                                                        
//��  ��: ��ѯ����1���ַ�                                                       
//��  ��: uratn:ģ�����磺UART0 
//          ch: �����������ָ��?
//��  ��: 1�����ɹ� 0����ʧ��                                                              
//��  ��: uart_querychar (UART4,a[]) �����յ������ݱ�����a������                                
//-------------------------------------------------------------------------*
char UART_Query_Char (UARTn_e uratn, char *ch)
{
    if( UART_RCFIFO_REG(UARTN[uratn]) )         //��ѯ�Ƿ���ܵ�����?
    {
        *ch  =   UART_D_REG(UARTN[uratn]);      //���ܵ�8λ������
        return  1;                              //���� 1 ��ʾ���ճɹ�
    }

    *ch = 0;                                    //���ղ�����Ӧ������˽�����?
    return 0;                                   //����0��ʾ����ʧ��
}


//-------------------------------------------------------------------------*
//������: uart_querychar                                                        
//��  ��: ��ѯ�����ַ���                                                       
//��  ��: uratn:ģ�����磺UART0 
//         str: �����������ָ��?
//     max_len: �����ճ���
//��  ��: iΪ�ַ������ȣ���Χ��0~max_len��                                                              
//��  ��: uart_querystr (UART4,a[],100) �����յ������ݱ�����a������                                
//-------------------------------------------------------------------------*
char UART_Query_Str (UARTn_e uratn, char *str, uint32 max_len)
{
    uint32 i = 0;
    while(UART_Query_Char(uratn, str + i)  )
    {
        if( *(str + i) == NULL )    //���յ��ַ���������
        {
            return i;
        }

        i++;
        if(i >= max_len)            //�����趨�����ֵ���˳�?
        {
            return i;
        }
    };

    return i;
}


//-------------------------------------------------------------------------*
//������: uart_putchar                                                        
//��  ��: ����һ���ֽ�                                                       
//��  ��: uratn:ģ�����磺UART0 
//         ch: ���͵��ֽ�
//��  ��: ��                                                              
//��  ��: uart_putchar (UART4, 0x66);                               
//-------------------------------------------------------------------------*
void UART_Put_Char (UARTn_e uratn, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK));

    //��������
    UART_D_REG(UARTN[uratn]) = (uint8)ch;
}



//-------------------------------------------------------------------------*
//������: uart_query                                                        
//��  ��: ��ѯ�Ƿ���ܵ�һ���ֽ�?                                                       
//��  ��: uratn:ģ�����磺UART0 
//��  ��: 1�����ɹ� 0����ʧ��                                                               
//��  ��: uart_query (UART4);                               
//-------------------------------------------------------------------------*
char UART_Query (UARTn_e uratn)
{
    if(UART_RCFIFO_REG(UARTN[uratn]))                 //���յ�����������0

    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//-------------------------------------------------------------------------*
//������: uart_putchar                                                        
//��  ��: ����ָ��len���ֽڳ������� ������ NULL Ҳ�ᷢ�ͣ�                                                       
//��  ��: uratn:ģ�����磺UART0 
//        buff: ���͵ĵ�ַ��
//        len : ����ָ������
//��  ��: ��                                                              
//��  ��: uart_putchar (UART4, "123456789",5);ʵ�ʷ���5���ֽڡ�1����2����3����4����5��                               
//-------------------------------------------------------------------------*
void UART_Put_Buff (UARTn_e uratn, s8 *buff, uint32 len)
{
    while(len--)
    {
        UART_Put_Char(uratn, *buff);
        buff++;
    }
}



//-------------------------------------------------------------------------*
//������: uart_putstr                                                        
//��  ��: �����ַ���(�� NULL ֹͣ����)                                                       
//��  ��: uratn:ģ�����磺UART0 
//        str: ���͵ĵ�ַ��
//��  ��: ��                                                              
//��  ��: UART_Put_Str (UART4, "123456789");ʵ�ʷ���9���ֽ�                              
//-------------------------------------------------------------------------*
void UART_Put_Str (UARTn_e uratn, char *str)
{
    while(*str)
    {
        UART_Put_Char(uratn, *str++);
    }
}


//-------------------------------------------------------------------------*
//������: UART_Irq_En                                                        
//��  ��: �����ڽ����ж�                                                      
//��  ��: uratn:ģ�����磺UART0      
//��  ��: ��                                                              
//��  ��: UART_Irq_En (UART4);                             
//-------------------------------------------------------------------------*
void UART_Irq_En(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) |= UART_C2_RIE_MASK;    //����UART�����ж�
    enable_irq((uratn << 1) + 45);			        //���������ŵ�IRQ�ж�
}



//-------------------------------------------------------------------------*
//������: UART_Irq_Dis                                                        
//��  ��: �ش��ڽ����ж�                                                      
//��  ��: uratn:ģ�����磺UART0      
//��  ��: ��                                                              
//��  ��: UART_Irq_Dis (UART4);                             
//-------------------------------------------------------------------------*
void UART_Irq_Dis(UARTn_e uratn)
{
    UART_C2_REG(UARTN[uratn]) &= ~UART_C2_RIE_MASK;   //��ֹUART�����ж�
    disable_irq((uratn << 1) + 45);			        //�ؽ������ŵ�IRQ�ж�
}


void UART0_IRQHandler(void)
{
    s8 data[20];

    //����һ���ֽ����ݲ��ط�
       
    sprintf(data,"UARTData:%c\n",UART_Get_Char (UART0));
    
    UART_Put_Str(UART0,data);      //�ͷ��ͳ�ȥ
}
void UART1_IRQHandler(void)
{
    s8 data[20];

    //����һ���ֽ����ݲ��ط�
       
    sprintf(data,"�㷢�͵�����Ϊ��%c\n",UART_Get_Char (UART1));
    
    UART_Put_Str(UART1,data);      //�ͷ��ͳ�ȥ
}
void UART2_IRQHandler(void)
{
    s8 data[100];

    //����һ���ֽ����ݲ��ط�
       
    sprintf(data,"�㷢�͵�����Ϊ��%c\n",UART_Get_Char (UART2));
    
    UART_Put_Str(UART2,data);      //�ͷ��ͳ�ȥ
}
void UART3_IRQHandler(void)
{
    s8 data[20];

    //����һ���ֽ����ݲ��ط�
       
    sprintf(data,"�㷢�͵�����Ϊ��%c\n",UART_Get_Char (UART3));
    
    UART_Put_Str(UART3,data);      //�ͷ��ͳ�ȥ
}
void UART4_IRQHandler(void)
{
    char cmd = UART_Get_Char(UART4);
    char rsp[32];

    extern volatile char g_last_cmd;
    g_last_cmd = cmd;

    switch(cmd)
    {
        case 'L':
            g_servo_pwm += 100;
            if (g_servo_pwm > Step_Right) g_servo_pwm = Step_Right;
            Drive_Set_Steering(g_servo_pwm);
            sprintf(rsp, "Steer Left %d\r\n", g_servo_pwm);
            UART_Put_Str(UART4, rsp);
            break;

        case 'R':
            g_servo_pwm -= 100;
            if (g_servo_pwm < Step_Left) g_servo_pwm = Step_Left;
            Drive_Set_Steering(g_servo_pwm);
            sprintf(rsp, "Steer Right %d\r\n", g_servo_pwm);
            UART_Put_Str(UART4, rsp);
            break;

        case 'F':
            g_target_speed += 5;  // Smaller step for smoother acceleration
            if (g_target_speed > 200) g_target_speed = 200;  // Max ~200 pulses per 50ms
            sprintf(rsp, "Speed+ %d\r\n", g_target_speed);
            UART_Put_Str(UART4, rsp);
            break;

        case 'B':
            g_target_speed -= 5;  // Smaller step for smoother deceleration
            if (g_target_speed < -200) g_target_speed = -200;  // Min ~-200 pulses per 50ms
            sprintf(rsp, "Speed- %d\r\n", g_target_speed);
            UART_Put_Str(UART4, rsp);
            break;

        case 'C':
            g_servo_pwm = Step_Middle;
            Drive_Set_Steering(g_servo_pwm);
            sprintf(rsp, "Center servo %d\r\n", g_servo_pwm);
            UART_Put_Str(UART4, rsp);
            break;

        case 'S':
            cmd_flag = 'S';  // Signal main loop to stop
            sprintf(rsp, "Emergency stop\r\n");
            UART_Put_Str(UART4, rsp);
            break;

        case 'Q':
            g_target_speed = 0;
            cmd_flag = 'S';  // Signal main loop to stop
            g_servo_pwm = Step_Middle;
            Drive_Set_Steering(g_servo_pwm);
            sprintf(rsp, "Reset\r\n");
            UART_Put_Str(UART4, rsp);
            break;

        case 'D':
            // Diagnostic: Show full state with proper number formatting
            // Output format: T=XXX C=XXX P=±XXX E=±XXXX
            // T: target speed, C: current speed, P: PWM output, E: encoder count
            UART_Put_Str(UART4, "T=");
            {
                int16 val = g_target_speed;
                if (val < 0) { UART_Put_Char(UART4, '-'); val = -val; }
                // Display up to 3 digits for speed
                if (val >= 100) UART_Put_Char(UART4, (char)('0' + val / 100));
                if (val >= 10) UART_Put_Char(UART4, (char)('0' + (val / 10) % 10));
                UART_Put_Char(UART4, (char)('0' + val % 10));
            }
            UART_Put_Str(UART4, " C=");
            {
                int16 val = current_speed;
                if (val < 0) { UART_Put_Char(UART4, '-'); val = -val; }
                if (val >= 100) UART_Put_Char(UART4, (char)('0' + val / 100));
                if (val >= 10) UART_Put_Char(UART4, (char)('0' + (val / 10) % 10));
                UART_Put_Char(UART4, (char)('0' + val % 10));
            }
            UART_Put_Str(UART4, " P=");
            {
                int16 val = g_motor_pwm;
                if (val < 0) { UART_Put_Char(UART4, '-'); val = -val; }
                // Display up to 3 digits for PWM (0-300)
                if (val >= 100) UART_Put_Char(UART4, (char)('0' + val / 100));
                if (val >= 10) UART_Put_Char(UART4, (char)('0' + (val / 10) % 10));
                UART_Put_Char(UART4, (char)('0' + val % 10));
            }
            UART_Put_Str(UART4, " E=");
            {
                int16 val = encoder_count;
                if (val < 0) { UART_Put_Char(UART4, '-'); val = -val; }
                // Display up to 4 digits for encoder
                if (val >= 1000) UART_Put_Char(UART4, (char)('0' + val / 1000));
                if (val >= 100) UART_Put_Char(UART4, (char)('0' + (val / 100) % 10));
                if (val >= 10) UART_Put_Char(UART4, (char)('0' + (val / 10) % 10));
                UART_Put_Char(UART4, (char)('0' + val % 10));
            }
            UART_Put_Str(UART4, "\r\n");
            break;

        case 'W':
            // Direct PWM mode - set PWM directly (for motor testing when encoder is broken)
            // Format: W150 (PWM=150), W-100 (PWM=-100)
            {
                int16 pwm_val = 0;
                char *ptr = rsp;
                char sign = 1;
                
                // Skip 'W' and get the number
                if (*ptr == '-') { sign = -1; ptr++; }
                
                while (*ptr >= '0' && *ptr <= '9') {
                    pwm_val = pwm_val * 10 + (*ptr - '0');
                    ptr++;
                }
                pwm_val *= sign;
                
                // Limit to valid range
                if (pwm_val > 300) pwm_val = 300;
                if (pwm_val < -300) pwm_val = -300;
                
                g_motor_pwm = pwm_val;
                
                // Switch to direct PWM mode temporarily (disable PID)
                g_pid_enabled = 0;
                
                sprintf(rsp, "PWM direct: %d (PID off)\r\n", pwm_val);
                UART_Put_Str(UART4, rsp);
            }
            break;

        case 'X':
            // Toggle PID mode on/off
            g_pid_enabled = !g_pid_enabled;
            if (g_pid_enabled) {
                sprintf(rsp, "PID mode ON\r\n");
            } else {
                sprintf(rsp, "PID mode OFF (direct PWM)\r\n");
            }
            UART_Put_Str(UART4, rsp);
            break;

        default:
            sprintf(rsp, "Unknown %c\r\n", cmd);
            UART_Put_Str(UART4, rsp);
            break;
    }
}
void UART5_IRQHandler(void)
{
    s8 data[20];

    //����һ���ֽ����ݲ��ط�
       
    sprintf(data,"�㷢�͵�����Ϊ��%c\n",UART_Get_Char (UART5));
    
    UART_Put_Buff(UART5,data,20);      //�ͷ��ͳ�ȥ
}

