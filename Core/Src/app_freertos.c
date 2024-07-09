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
uint32_t leftLight = 0;  // 左光传感器数值
uint32_t rightLight = 0; // 右光传感器数值
char leftLightStr[20];   // 用于存储左光传感器数值的字符串
char rightLightStr[20];  // 用于存储右光传感器数值的字符串
int fputc(int ch, FILE *f) 
{ 
// 等待发送完成
while((USART1->ISR & USART_ISR_TXE) == 0);

// 发送字符
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
	printf("start\r\n");// 打印任务开始信息到串口
	memset(data_rx,0,20);// 初始化接收数据缓冲区为0
	HAL_UART_Receive_IT(&huart2, data_rx, 4);// 启动UART接收中断，接收4个字节的数据到data_rx缓冲区
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
uint16_t r_queue = 0; // 定义一个用于接收消息队列数据的变量，初始值为0
uint16_t len = 0; // 定义长度变量，用于记录待发送数据的长度，初始值为0
char data_send[20] = {0}; // 定义一个长度为20的待发送数据缓冲区，初始所有元素为0
for (;;)
{
    osMessageQueueGet(myQueue01Handle, &r_queue, 0, osWaitForever);
    // 从消息队列（myQueue01Handle）中获取消息，等待时间设为永久(osWaitForever)，获取的消息存储在r_queue变量中
    HAL_UART_Transmit(&huart2, data_send, len, 1000);
    // 通过UART2发送数据，发送data_send缓冲区中的数据，发送长度为len个字节，超时时间为1000毫秒
    osDelay(1); // 延时1毫秒，控制任务执行频率
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
        ulTaskNotifyTake(1, osWaitForever); // 等待接收任务通知
        printf("datarx=%s\r\n", data_rx); // 打印接收到的数据				
		    // 比较接收到的数据并执行相应操作
        if(strcmp(data_rx, "OPEN") == 0)
        {
            x = 1;
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动PWM输出
            duty = 255;
            printf("当前档位为：%d档\r\n", x); // 打印当前档位
        }
        else if(strcmp(data_rx, "CLOS") == 0)
        {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // 停止PWM输出
            duty = 0;
            x = 0;
            printf("当前档位为：%d档\r\n", x); // 打印当前档位
        }
        else if(strcmp(data_rx, "high") == 0)
        {
            if(x < 5)
            {
                x++;
                printf("当前档位为：%d档\r\n", x); // 打印当前档位
            }
        }
        else if(strcmp(data_rx, "down") == 0)
        {
            if(x > 0)
            {
                x--;
                printf("当前档位为：%d档\r\n", x); // 打印当前档位
            }
        }
				
        // 根据档位设置PWM占空比
        switch(x)
        {
            case 0:
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // 停止PWM输出
                duty = 0;
                break;
            case 1:
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动PWM输出
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

        TIM1->CCR1 = duty; // 设置TIM1通道1的占空比

if (strcmp(data_rx, "left") == 0)  
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1200); // 设置TIM3通道1的比较值为1400
    osDelay(500); // 延时500毫秒
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // 设置TIM3通道1的比较值为1500
		printf("1");
}

if (strcmp(data_rx, "righ") == 0)  
{
	printf("2");
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1800); // 设置TIM3通道1的比较值为1600
    osDelay(500); // 延时500毫秒
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // 设置TIM3通道1的比较值为1500
}
        memset(data_rx, 0, 20); // 清空接收数据数组
			osDelay(1); // 延时1ms，释放处理器时间片
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
    uint8_t y = 0; // 按钮按下标志
    uint8_t button_pressed = 0; // 按钮按下标志
    for(;;)
    {
        // 检测按钮是否被按下
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) // 如果按键被按下
        {
            HAL_Delay(50); // 延时一段时间避免按键抖动影响
            if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) // 如果按键仍然被按下
            {
                x++; // 切换到下一个档位
                button_pressed = 1; // 设置按钮按下标志
							if(x>5){
								x=0;
							}
                HAL_Delay(10); // 延时一段时间以防止连续按下
            }
            while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET); // 等待按钮释放
        }

        // 根据档位设置PWM占空比
        if(button_pressed == 1) // 如果按钮被按下
        {
            switch(x)
            {
                case 0:
                    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // 停止PWM输出
                    duty = 0;
                    printf("\r\n当前档位为：%d档\r\n", x);
                    break;
                case 1:
                    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动PWM输出
                    duty = 199;
                    printf("\r\n当前档位为：%d档\r\n", x);
                    break;
                case 2:
                    duty = 399;
                    printf("\r\n当前档位为：%d档\r\n", x);
                    break;
                case 3:
                    duty = 599;
                    printf("\r\n当前档位为：%d档\r\n", x);
                    break;
                case 4:
                    duty = 799;
                    printf("\r\n当前档位为：%d档\r\n", x);
                    break;
                case 5:
                    duty = 999;
                    printf("\r\n当前档位为：%d档\r\n", x);
                    break;
                default:

                    break;
            }
            button_pressed = 0; // 清除按钮按下标志
            TIM1->CCR1 = duty; // 设置PWM占空比
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
	uint8_t data_tx; // 用于接收数据的变量
  for(;;)
  {
    if (HAL_UART_Receive(&huart3, &data_tx, 1, HAL_MAX_DELAY) == HAL_OK)
    {
        printf("接收到的数据：0x%02X\r\n", data_tx);
        
        if (data_tx == 0x01) // 如果接收到的数据是0x01
        {		
					  x = 1; // 设置档位为1
					  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动PWM输出
            duty = 255; // 将灯的占空比设为255，即灯全亮
            printf("当前档位为：%d档\r\n", x); // 打印当前档位
        }
        if (data_tx == 0x00) // 如果接收到的数据是0x00
        {
					  x = 0; // 设置档位为0
					  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // 停止PWM输出
            duty = 0; // 将灯的占空比设为0，即灯灭
            printf("当前档位为：%d档\r\n", x); // 打印当前档位
        }
				else if(data_tx == 0x02) // 如果接收到的数据是0x02
    {
        if(x < 5)
        {
            x++; // 档位加一
            printf("当前档位为：%d档\r\n", x); // 打印当前档位
        }
    }
    else if(data_tx == 0x03) // 如果接收到的数据是0x03
    {
        if(x > 0)
        {
            x--; // 档位减一
            printf("当前档位为：%d档\r\n", x); // 打印当前档位
        }
    }

    // 根据档位设置PWM占空比
    switch(x)
    {
        case 0:
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); // 停止PWM输出
            duty = 0;
            break;
        case 1:
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动PWM输出
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

    TIM1->CCR1 = duty; // 设置TIM1通道1的占空比

    memset(data_rx, 0, 20); // 清空接收数据数组

    osDelay(1); // 延时1ms，释放处理器时间片
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
    OLED_Init();  // OLED初始化
    char buffer[20];
    for (;;)
    {
        HAL_Delay(100);  // 延时，调整为适当的值
        // 模拟更新 leftLight 和 rightLight 的值
        // 这部分需要根据实际的传感器数据更新逻辑来实现
        // 这里使用 sprintf 模拟更新 leftLightStr 和 rightLightStr
				OLED_ShowChinese(0*16, 0, 4, 16, 1);  // 太
        OLED_ShowChinese(1*16, 0, 5, 16, 1);  // 阳
        OLED_ShowChinese(2*16, 0, 6, 16, 1);  // 能
        OLED_ShowChinese(3*16, 0, 7, 16, 1);  // 路
        OLED_ShowChinese(4*16, 0, 8, 16, 1);  // 灯
        OLED_ShowChinese(5*16, 0, 9, 16, 1);  // 系
        OLED_ShowChinese(6*16, 0, 10, 16, 1); // 统
        OLED_ShowChinese(0*16, 16*1, 11, 16, 1); // 当
        OLED_ShowChinese(1*16, 16*1, 12, 16, 1); // 前
        OLED_ShowChinese(2*16, 16*1, 13, 16, 1); // 挡
        OLED_ShowChinese(3*16, 16*1, 14, 16, 1); // 位
        OLED_ShowChinese(4*16, 16*1, 15, 16, 1); // 为
        OLED_ShowChinese(5*16, 16*1, 16, 16, 1); // ：	
        OLED_ShowChinese(7*16, 16*1, 13, 16, 1); // 挡			
        // 显示动态数据
			  sprintf(buffer, "%d", x); 
        OLED_ShowString(6*16, 16*1, buffer, 16, 1); // 档位值
        OLED_ShowString(0*16, 16*2, "left", 16, 1);
        OLED_ShowChinese(3*16, 16*2, 16, 16, 1);  // ：
        OLED_ShowString(4*16, 16*2, (uint8_t *)leftLightStr, 16, 1); // 档位值
        OLED_ShowString(0*16, 16*3, "right", 16, 1);
        OLED_ShowChinese(3*16, 16*3, 16, 16, 1);  // ：
        OLED_ShowString(4*16, 16*3, (uint8_t *)rightLightStr, 16, 1); // 档位值
        // 更新显示
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
    HAL_ADC_Start(&hadc2); // 启动ADC1转换
    HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY); // 等待ADC1转换完成
    rightLight = HAL_ADC_GetValue(&hadc2); // 获取右光传感器的数值
//    printf("Right Light Sensor Value: %lu\n", rightLight); // 打印右光传感器的数值
    HAL_Delay(50); // 延时50毫秒

    HAL_ADC_Start(&hadc1); // 启动ADC2转换
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // 等待ADC2转换完成
    leftLight = HAL_ADC_GetValue(&hadc1); // 获取左光传感器的数值
//    printf("Left Light Sensor Value: %lu\n", leftLight); // 打印左光传感器的数值
    HAL_Delay(50); // 延时50毫秒
		leftLight=leftLight+1000;
    rightLight=rightLight+1000;
		
		sprintf(leftLightStr, "%lu", leftLight);   // %lu 用于打印 unsigned long 整数值
    sprintf(rightLightStr, "%lu", rightLight); // %lu 用于打印 unsigned long 整数值
		
		    // 将左光传感器数值和右光传感器数值转换为字符串

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
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1200); // 设置TIM3通道1的比较值为1400
    osDelay(100); // 延时10毫秒
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // 设置TIM3通道1的比较值为1500
}

if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET)  
{
//	printf("2");
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1800); // 设置TIM3通道1的比较值为1600
    osDelay(100); // 延时10毫秒
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500); // 设置TIM3通道1的比较值为1500
}
		
    osDelay(1); // 操作系统延时1000毫秒
  }
  /* USER CODE END StartTask07 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

