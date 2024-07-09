/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"
#include "oled.h"
uint16_t duty = 0;
uint8_t x=0;
uint8_t  data_rx[20]={0};
uint32_t leftLight = 0;  // ��⴫������ֵ
uint32_t rightLight = 0; // �ҹ⴫������ֵ
char leftLightStr[20];   // ���ڴ洢��⴫������ֵ���ַ���
char rightLightStr[20];  // ���ڴ洢�ҹ⴫������ֵ���ַ���
int fputc(int ch, FILE *f) 
{ 
// �ȴ��������
while((USART1->ISR & USART_ISR_TXE) == 0);

// �����ַ�
USART1->TDR = (char) ch; 
return ch; 
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ARR 20000
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask06 */
osThreadId_t myTask06Handle;
const osThreadAttr_t myTask06_attributes = {
  .name = "myTask06",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask07 */
osThreadId_t myTask07Handle;
const osThreadAttr_t myTask07_attributes = {
  .name = "myTask07",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 vTaskNotifyGiveFromISR(myTask03Handle ,0);
	HAL_UART_Receive_IT(&huart2, data_rx, 4);
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);
void StartTask05(void *argument);
void StartTask06(void *argument);
void StartTask07(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(StartTask04, NULL, &myTask04_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(StartTask05, NULL, &myTask05_attributes);

  /* creation of myTask06 */
  myTask06Handle = osThreadNew(StartTask06, NULL, &myTask06_attributes);

  /* creation of myTask07 */
  myTask07Handle = osThreadNew(StartTask07, NULL, &myTask07_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	printf("start\r\n");// ��ӡ����ʼ��Ϣ������
	memset(data_rx,0,20);// ��ʼ���������ݻ�����Ϊ0
	HAL_UART_Receive_IT(&huart2, data_rx, 4);// ����UART�����жϣ�����4���ֽڵ����ݵ�data_rx������
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
uint16_t r_queue = 0; // ����һ�����ڽ�����Ϣ�������ݵı�������ʼֵΪ0
uint16_t len = 0; // ���峤�ȱ��������ڼ�¼���������ݵĳ��ȣ���ʼֵΪ0
char data_send[20] = {0}; // ����һ������Ϊ20�Ĵ��������ݻ���������ʼ����Ԫ��Ϊ0
for (;;)
{
    osMessageQueueGet(myQueue01Handle, &r_queue, 0, osWaitForever);
    // ����Ϣ���У�myQueue01Handle���л�ȡ��Ϣ���ȴ�ʱ����Ϊ����(osWaitForever)����ȡ����Ϣ�洢��r_queue������
    HAL_UART_Transmit(&huart2, data_send, len, 1000);
    // ͨ��UART2�������ݣ�����data_send�������е����ݣ����ͳ���Ϊlen���ֽڣ���ʱʱ��Ϊ1000����
    osDelay(1); // ��ʱ1���룬��������ִ��Ƶ��
}

  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
    for(;;)
    {
        ulTaskNotifyTake(1, osWaitForever); // �ȴ���������֪ͨ
        printf("datarx=%s\r\n", data_rx); // ��ӡ���յ�������				
		    // �ȽϽ��յ������ݲ�ִ����Ӧ����
        if(strcmp(data_rx, "OPEN") == 0)
        {
            x = 1;
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // ����PWM���
            duty = 255;
            printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
        }
        else if(strcmp(data_rx, "CLOS") == 0)
        {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // ֹͣPWM���
            duty = 0;
            x = 0;
            printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
        }
        else if(strcmp(data_rx, "high") == 0)
        {
            if(x < 5)
            {
                x++;
                printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
            }
        }
        else if(strcmp(data_rx, "down") == 0)
        {
            if(x > 0)
            {
                x--;
                printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
            }
        }
				
        // ���ݵ�λ����PWMռ�ձ�
        switch(x)
        {
            case 0:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // ֹͣPWM���
                duty = 0;
                break;
            case 1:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // ����PWM���
                duty = 199;
                break;
            case 2:
                duty = 399;
                break;
            case 3:
                duty = 599;
                break;
            case 4:
                duty = 799;
                break;
            case 5:
                duty = 999;
                break;
            default:
                x = 0;
                duty = 0;
                break;
        }

        TIM1->CCR1 = duty; // ����TIM1ͨ��1��ռ�ձ�

if (strcmp(data_rx, "left") == 0)  
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1200); // ����TIM3ͨ��1�ıȽ�ֵΪ1400
    osDelay(500); // ��ʱ500����
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // ����TIM3ͨ��1�ıȽ�ֵΪ1500
		printf("1");
}

if (strcmp(data_rx, "righ") == 0)  
{
	printf("2");
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1800); // ����TIM3ͨ��1�ıȽ�ֵΪ1600
    osDelay(500); // ��ʱ500����
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // ����TIM3ͨ��1�ıȽ�ֵΪ1500
}
        memset(data_rx, 0, 20); // ��ս�����������
			osDelay(1); // ��ʱ1ms���ͷŴ�����ʱ��Ƭ
    }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */
  /* Infinite loop */
    uint8_t y = 0; // ��ť���±�־
    uint8_t button_pressed = 0; // ��ť���±�־
    for(;;)
    {
        // ��ⰴť�Ƿ񱻰���
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) // �������������
        {
            HAL_Delay(50); // ��ʱһ��ʱ����ⰴ������Ӱ��
            if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) // ���������Ȼ������
            {
                x++; // �л�����һ����λ
                button_pressed = 1; // ���ð�ť���±�־
							if(x>5){
								x=0;
							}
                HAL_Delay(10); // ��ʱһ��ʱ���Է�ֹ��������
            }
            while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET); // �ȴ���ť�ͷ�
        }

        // ���ݵ�λ����PWMռ�ձ�
        if(button_pressed == 1) // �����ť������
        {
            switch(x)
            {
                case 0:
                    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // ֹͣPWM���
                    duty = 0;
                    printf("\r\n��ǰ��λΪ��%d��\r\n", x);
                    break;
                case 1:
                    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // ����PWM���
                    duty = 199;
                    printf("\r\n��ǰ��λΪ��%d��\r\n", x);
                    break;
                case 2:
                    duty = 399;
                    printf("\r\n��ǰ��λΪ��%d��\r\n", x);
                    break;
                case 3:
                    duty = 599;
                    printf("\r\n��ǰ��λΪ��%d��\r\n", x);
                    break;
                case 4:
                    duty = 799;
                    printf("\r\n��ǰ��λΪ��%d��\r\n", x);
                    break;
                case 5:
                    duty = 999;
                    printf("\r\n��ǰ��λΪ��%d��\r\n", x);
                    break;
                default:

                    break;
            }
            button_pressed = 0; // �����ť���±�־
            TIM1->CCR1 = duty; // ����PWMռ�ձ�
        }
    }
		osDelay(1);
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask05 */
void StartTask05(void *argument)
{
  /* USER CODE BEGIN StartTask05 */
  /* Infinite loop */
	uint8_t data_tx; // ���ڽ������ݵı���
  for(;;)
  {
    if (HAL_UART_Receive(&huart3, &data_tx, 1, HAL_MAX_DELAY) == HAL_OK)
    {
        printf("���յ������ݣ�0x%02X\r\n", data_tx);
        
        if (data_tx == 0x01) // ������յ���������0x01
        {		
					  x = 1; // ���õ�λΪ1
					  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // ����PWM���
            duty = 255; // ���Ƶ�ռ�ձ���Ϊ255������ȫ��
            printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
        }
        if (data_tx == 0x00) // ������յ���������0x00
        {
					  x = 0; // ���õ�λΪ0
					  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // ֹͣPWM���
            duty = 0; // ���Ƶ�ռ�ձ���Ϊ0��������
            printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
        }
				else if(data_tx == 0x02) // ������յ���������0x02
    {
        if(x < 5)
        {
            x++; // ��λ��һ
            printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
        }
    }
    else if(data_tx == 0x03) // ������յ���������0x03
    {
        if(x > 0)
        {
            x--; // ��λ��һ
            printf("��ǰ��λΪ��%d��\r\n", x); // ��ӡ��ǰ��λ
        }
    }

    // ���ݵ�λ����PWMռ�ձ�
    switch(x)
    {
        case 0:
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // ֹͣPWM���
            duty = 0;
            break;
        case 1:
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // ����PWM���
            duty = 199;
            break;
        case 2:
            duty = 399;
            break;
        case 3:
            duty = 599;
            break;
        case 4:
            duty = 799;
            break;
        case 5:
            duty = 999;
            break;
        default:
            x = 0;
            duty = 0;
            break;
    }

    TIM1->CCR1 = duty; // ����TIM1ͨ��1��ռ�ձ�

    memset(data_rx, 0, 20); // ��ս�����������

    osDelay(1); // ��ʱ1ms���ͷŴ�����ʱ��Ƭ
	}
	}
  /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_StartTask06 */
/**
* @brief Function implementing the myTask06 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask06 */
void StartTask06(void *argument)
{
    OLED_Init();  // OLED��ʼ��
    char buffer[20];
    for (;;)
    {
        HAL_Delay(100);  // ��ʱ������Ϊ�ʵ���ֵ
        // ģ����� leftLight �� rightLight ��ֵ
        // �ⲿ����Ҫ����ʵ�ʵĴ��������ݸ����߼���ʵ��
        // ����ʹ�� sprintf ģ����� leftLightStr �� rightLightStr
				OLED_ShowChinese(0*16, 0, 4, 16, 1);  // ̫
        OLED_ShowChinese(1*16, 0, 5, 16, 1);  // ��
        OLED_ShowChinese(2*16, 0, 6, 16, 1);  // ��
        OLED_ShowChinese(3*16, 0, 7, 16, 1);  // ·
        OLED_ShowChinese(4*16, 0, 8, 16, 1);  // ��
        OLED_ShowChinese(5*16, 0, 9, 16, 1);  // ϵ
        OLED_ShowChinese(6*16, 0, 10, 16, 1); // ͳ
        OLED_ShowChinese(0*16, 16*1, 11, 16, 1); // ��
        OLED_ShowChinese(1*16, 16*1, 12, 16, 1); // ǰ
        OLED_ShowChinese(2*16, 16*1, 13, 16, 1); // ��
        OLED_ShowChinese(3*16, 16*1, 14, 16, 1); // λ
        OLED_ShowChinese(4*16, 16*1, 15, 16, 1); // Ϊ
        OLED_ShowChinese(5*16, 16*1, 16, 16, 1); // ��	
        OLED_ShowChinese(7*16, 16*1, 13, 16, 1); // ��			
        // ��ʾ��̬����
			  sprintf(buffer, "%d", x); 
        OLED_ShowString(6*16, 16*1, buffer, 16, 1); // ��λֵ
        OLED_ShowString(0*16, 16*2, "left", 16, 1);
        OLED_ShowChinese(3*16, 16*2, 16, 16, 1);  // ��
        OLED_ShowString(4*16, 16*2, (uint8_t *)leftLightStr, 16, 1); // ��λֵ
        OLED_ShowString(0*16, 16*3, "right", 16, 1);
        OLED_ShowChinese(3*16, 16*3, 16, 16, 1);  // ��
        OLED_ShowString(4*16, 16*3, (uint8_t *)rightLightStr, 16, 1); // ��λֵ
        // ������ʾ
        OLED_Refresh();
    }
}
/* USER CODE BEGIN Header_StartTask07 */
/**
* @brief Function implementing the myTask07 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask07 */
void StartTask07(void *argument)
{
  /* USER CODE BEGIN StartTask07 */
  /* Infinite loop */
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  for (;;)
  {
    HAL_ADC_Start(&hadc2); // ����ADC1ת��
    HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY); // �ȴ�ADC1ת�����
    rightLight = HAL_ADC_GetValue(&hadc2); // ��ȡ�ҹ⴫��������ֵ
//    printf("Right Light Sensor Value: %lu\n", rightLight); // ��ӡ�ҹ⴫��������ֵ
    HAL_Delay(50); // ��ʱ50����

    HAL_ADC_Start(&hadc1); // ����ADC2ת��
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // �ȴ�ADC2ת�����
    leftLight = HAL_ADC_GetValue(&hadc1); // ��ȡ��⴫��������ֵ
//    printf("Left Light Sensor Value: %lu\n", leftLight); // ��ӡ��⴫��������ֵ
    HAL_Delay(50); // ��ʱ50����
		leftLight=leftLight+1000;
    rightLight=rightLight+1000;
		
		sprintf(leftLightStr, "%lu", leftLight);   // %lu ���ڴ�ӡ unsigned long ����ֵ
    sprintf(rightLightStr, "%lu", rightLight); // %lu ���ڴ�ӡ unsigned long ����ֵ
		
		    // ����⴫������ֵ���ҹ⴫������ֵת��Ϊ�ַ���

				if (rightLight > (leftLight + 300)) {
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1400);
     } else if (leftLight > (rightLight + 300)) {
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1600);
     } else {
      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1500);
     }
		
if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET)  
{
//	printf("1");
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1200); // ����TIM3ͨ��1�ıȽ�ֵΪ1400
    osDelay(100); // ��ʱ10����
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // ����TIM3ͨ��1�ıȽ�ֵΪ1500
}

if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET)  
{
//	printf("2");
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1800); // ����TIM3ͨ��1�ıȽ�ֵΪ1600
    osDelay(100); // ��ʱ10����
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // ����TIM3ͨ��1�ıȽ�ֵΪ1500
}
		
    osDelay(1); // ����ϵͳ��ʱ1000����
  }
  /* USER CODE END StartTask07 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

