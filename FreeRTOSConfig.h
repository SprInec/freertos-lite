/**
 * ****************************************************************************
 * @file FreeRTOSConfig.h
 * @author SprInec (JulyCub@163.com)
 * @brief FreeRTOS Configuration File
 * @version 1.1
 * @date 2024.10.31       -> 1.0 : Created
 * ****************************************************************************
 * @update 2024.11.01     -> 1.1 : Optimized comments. Added MCU, CMSIS
 *                                 Configuration Wizard and libstm adaptation.
 *
 * @copyright Copyright (c) 2024
 * ****************************************************************************
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> 工程环境配置选项
// =====================================================================
// <o> MCU 型号选择
//     <0=> STM32 F1
//     <1=> STM32 F4
//     <2=> STM32 H7
//     <3=> STM32 G4
// <i> 选择所使用的 MCU 型号
#define configMCU_TYPE 1

// <o> STM32开发库选择
//     <0=> STD
//     <1=> HAL
// <i> 选择所使用的 STM32 开发库
// <i> STD: 标准库
// <i> HAL: 硬件抽象层库
// <i> 注: STM32 H7 与 STM32 G4 芯片仅支持 HAL 库
#define configSTM32_LIB 1

// <e> libstm
// <i> 使用 libstm 库
#define configUSE_LIBSTM 1
// </e> !libstm

#if (configSTM32_LIB == 0)
#if (configMCU_TYPE == 0)
#include "stm32f10x.h"

#elif (configMCU_TYPE == 1)
#include "stm32f40x.h"
#endif /* !configMCU_TYPE */

#elif (configSTM32_LIB == 1) 
#if (configMCU_TYPE == 0)
#include "stm32f1xx_hal.h"

#elif (configMCU_TYPE == 1)
#include "stm32f4xx_hal.h"

#elif (configMCU_TYPE == 2)
#include "stm32h7xx_hal.h"

#elif (configMCU_TYPE == 3)
#include "stm32g4xx_hal.h"
#endif /* !configMCU_TYPE */
#endif /* !configSTM32_LIB */

// 针对不同的编译器调用不同的 stdint.h 头文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif
// </h> !工程环境配置选项

// <h> FreeRTOS 基础配置配置选项
// =====================================================================
// <o> RTOS 调度器类型
//     <0=> [0] 协作式调度器(时间片)
//     <1=> [1] 抢占式调度器
// <i> configUSE_PREEMPTION
// <i> 在多任务管理机制上，操作系统可以分为抢占式和协作式两种。
// <i> 协作式操作系统是任务主动释放 CPU 后，切换到下一个任务。
#define configUSE_PREEMPTION 1

// <e> 时间片调度
// <i> configUSE_TIME_SLICING
// <i> 默认: 1 (Enable)
// <i> 时间片调度是指当一个任务运行一段时间后，操作系统会自动切换到下一个任务。
// <i> 当使能此项后,处于就绪态的多个相同优先级任务将会以时间片切换的方式共享处理器。
// <i> 时间片调度可以提高系统的吞吐量, 但也会降低系统的响应速度/实时性。
// <i> 时间片的大小由 configTICK_RATE_HZ 决定。
#define configUSE_TIME_SLICING 1
// </e> !时间片调度

// <e> 开启断言
#define configUSE_ASSERT_INFO 0
// </e> !开启断言
#if configUSE_ASSERT_INFO
#include "bsp_usart.h"
#define vAssertCalled(char, int) printf("Error: %s, %d\r\n", char, int)
#define configASSERT(x) if ((x) == 0) vAssertCalled(__FILE__, __LINE__)
#endif

// <o> 选择下一个执行任务的方法
//     <0=> [0] 通用方法
//     <1=> [1] 特殊方法
// <i> configUSE_PORT_OPTIMISED_TASK_SELECTION
// <i> 选择下一任务执行的方法, 0 为通用方法, 1 为特殊方法
// <i>
// <i> 某些运行 FreeRTOS 的硬件有两种方法选择下一个要执行的任务
// <i> 通用方法和特定于硬件的方法, (以下简称 " 特殊方法 " )
// <i> 
// <i> 通用方法: 扫描就绪链表法
// <i>   1. configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或硬件不支持
// <i>      这种特殊方法
// <i>   2. 可以用于所有 FreeRTOS 支持的硬件
// <i>   3. 完全用 C 实现, 效率略低于特殊方法
// <i>   4. 不强制要求限制最大可用优先级数目
// <i> 
// <i> 特殊方法: 硬件查找法
// <i>   1. 必须将 configUSE_PORT_OPTIMISED_TASK_SELECTION 设置为 1
// <i>   2. 依赖一个或多个特定架构的汇编指令 (一般是类似计算机前导零[CLZ])
// <i>      指令)
// <i>   3. 比通用方法更高效
// <i>   4. 一般强制限定最大可用优先级数目为 32
// <i> 
// <i> 一般是硬件计算前导指令, 如果所使用的 MCU 没有这些硬件指令的话此宏应
// <i> 设置为 0!
// <i>
// <i> 选择特殊方法可以提高 FreeRTOS 在特定硬件上的运行效率, 但也会增加代码量
// <i> 因此, 请根据具体硬件情况选择合适的方法!
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1

// <e> 低功耗 tickless 模式
// <i> configUSE_TICKLESS_IDLE
// <i> 0 Disable: 保持系统节拍 (tick) 中断一直运行
// <i> 1 Enable: 低功耗 tickless 模式
// <i> 低功耗 tickless 模式可以减少系统的功耗, 但也会降低系统的响应速度
// <i> 建议在系统空闲时使用低功耗 tickless 模式, 避免系统进入低功耗模式
#define configUSE_TICKLESS_IDLE 0
// </e> !低功耗 tickless 模式

// <o> CPU 内核时钟频率 (MHz)
// <i> configCPU_CLOCK_HZ
// <i> 自动获取, 无需更改
// <i> 写入实际的 CPU 内核时钟频率, 即 CPU 指令执行频率, 通常为 Fclk
// <i> Fclk 为供给 CPU 内核的时钟信号, 我们所说的 CPU 主频为 XX MHZ
// <i> 就是指的这个时钟信号, 相应的, 1/Fclk 就是 CPU 时钟周期 Tclk
#define _configCPU_CLOCK_HZ 0

#if (_configCPU_CLOCK_HZ == 0)
#define configCPU_CLOCK_HZ (SystemCoreClock)
#else
#define configCPU_CLOCK_HZ _configCPU_CLOCK_HZ
#endif

// <o> RTOS 系统节拍中断频率 (Hz)
// <i> configTICK_RATE_HZ
// <i> 即一秒钟中断的次数, 每次中断 RTOS 都会进任务调度
#define configTICK_RATE_HZ ((TickType_t)1000)

// <o> 可使用的最大优先级
// <i> configMAX_PRIORITIES
#define configMAX_PRIORITIES (32)

// <o> 空闲任务优先级
// <i> configIDLE_TASK_PRIORITY
#define configIDLE_TASK_PRIORITY ((UBaseType_t)0)

// <o> 空闲任务使用的堆栈大小
// <i> configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE ((unsigned short )128)

// <o> 任务名字字符串长度
// <i> configMAX_TASK_NAME_LEN
#define configMAX_TASK_NAME_LEN	(16)

// <o> 系统节拍计数器变量数据类型
//     <0=> [0] 32-bit int
//     <1=> [1] 16-bit int
// <i> configUSE_16_BIT_TICKS
// <i> TickType_t 变量用于表示系统节拍计数器的计数值
#define configUSE_16_BIT_TICKS 0

// <e> 空闲任务放弃 CPU 使用权给其他同优先级的用户任务
// <i> configIDLE_SHOULD_YIELD
#define configIDLE_SHOULD_YIELD	0
// </e> !空闲任务放弃 CPU 使用权给其他同优先级的用户任务

// <e> 启用队列
// <i> configUSE_QUEUE_SETS
#define configUSE_QUEUE_SETS 1
// </e> !启用队列

// <e> 使能任务通知功能
// <i> configUSE_TASK_NOTIFICATIONS
#define configUSE_TASK_NOTIFICATIONS 0
// </e> !使能任务通知功能

// <e> 使能二值信号量
// <i> configUSE_BINARY_SEMAPHORES
#define configUSE_BINARY_SEMAPHORES 1
// </e> !使能二值信号量

// <e> 使能互斥信号量
// <i> configUSE_MUTEXES
#define configUSE_MUTEXES 1
// </e> !使能互斥信号量

// <e> 使能递归互斥信号量
// <i> configUSE_RECURSIVE_MUTEXES
#define configUSE_RECURSIVE_MUTEXES	0
// </e> !使能递归互斥信号量

// <e> 使能计数信号量
// <i> configUSE_COUNTING_SEMAPHORES
#define configUSE_COUNTING_SEMAPHORES 0
// </e> !使能计数信号量

// <o> 可注册的信号量和消息队列个数
// <i> configQUEUE_REGISTRY_SIZE
#define configQUEUE_REGISTRY_SIZE 10

// <e> 设置任务标签功能
// <i> configUSE_APPLICATION_TASK_TAG
#define configUSE_APPLICAITON_TASK_TAG 0
// </e> !设置任务标签功能
// </h> !FreeRTOS 基础配置配置选项


// <h> FreeRTOS 与内存申请有关配置选项
// =====================================================================
// <e> 动态内存申请
// <i> configSUPPORT_DYNAMIC_ALLOCATION
#define configSUPPORT_DYNAMIC_ALLOCATION 1
// </e> !动态内存申请

// <e> 静态内存分配
// <i> configSUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION	 0
// </e> !静态内存分配

// <o> 系统总的堆大小 (KB)
// <i> configTOTAL_HEAP_SIZE
// <i> 单位: KB (1024 bytes)
// <i> 默认: 36KB (36 * 1024)
#define configTOTAL_HEAP_SIZE ((size_t)(48 * 1024))
// </h> !FreeRTOS 与内存申请有关配置选项

// <h> FreeRTOS 与钩子函数有关的配置选项
// =====================================================================
// <e> 使能空闲钩子
// <i> configUSE_IDLE_HOOK
// <i> 0 Disable: 忽略空闲钩子
// <i> 1 Enable: 使用空闲钩子
// <i> 空闲钩子是一个函数, 该函数由用户来实现
// <i> FreeRTOS 规定了函数的名字和参数: void vApplicationIdleHook(void)
// <i> 这个函数在每个空闲任务周器都会被调用
// <i> 对于已经删除的 RTOS 任务, 空闲任务可以释放分配给他们的堆栈内存
// <i> 因此必须保证空闲任务可以被 CPU 执行
// <i> 使用空闲钩子函数设置 CPU 进入省电模式是很常见的
// <i> 不可以调用会引起空闲任务阻塞的 API 函数
#define configUSE_IDLE_HOOK	0
// </e> !使能空闲钩子

// <e> 使能时间片钩子
// <i> configUSE_TICK_HOOK
// <i> 0 Disable: 忽略时间片钩子
// <i> 1 Enable: 使用时间片钩子
// <i> 时间片钩子是一个函数, 该函数由用户来实现
// <i> FreeRTOS 规定了函数的名字和参数: void vApplicationTickHook(void)
// <i> 时间片中断可以周期性的调用
// <i> 函数必须非常短小, 不能大量使用堆栈
// <i> 不能调用以 "FromISR" 或 "FROM_ISR" 结尾的 API 函数
#define configUSE_TICK_HOOK	0
// </e> !使能时间片钩子

// <e> 使能内存申请失败钩子
// <i> configUSE_MALLOC_FAILED_HOOK
#define configUSE_MALLOC_FAILED_HOOK	0
// </e> !使能内存申请失败钩子

// <o> 栈溢出检测方式
//     <0=> [0] 不启用
//     <1=> [1] 栈顶指针检测
//     <2=> [2] 栈底指针检测
// <i> configCHECK_FOR_STACK_OVERFLOW
// <i> 若使用此功能: 用户必须提供一个栈溢出钩子函数
#define configCHECK_FOR_STACK_OVERFLOW 0
// </h> !FreeRTOS 与钩子函数有关的配置选项

// <h> FreeRTOS 与运行时间和任务状态收集有关的配置选项
// =====================================================================
// <e> 运行时间统计功能
// <i> configGENERATE_RUN_TIME_STATS
#define configGENERATE_RUN_TIME_STATS 0
// </e> !运行时间统计功能

/* 启用可视化跟踪调试 */

// <e> 可视化跟踪调试
// <i> configUSE_TRACE_FACILITY
#define configUSE_TRACE_FACILITY 0
// </e> !可视化跟踪调试

// <e> 统计格式化函数
// <i> configUSE_STATS_FORMATTING_FUNCTIONS
// <i> 与运行时间统计功能同时使能时启用任务跟踪功能
// <i> 启用后会编译下面 3 个函数:
// <i> - prvWriteNametoBuffer()
// <i> - vTaskList()
// <i> - vTaskGetRunTimeStats()
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
// </e> !统计格式化函数
// </h> !FreeRTOS 与运行时间和任务状态收集有关的配置选项

#if (configGENERATE_RUN_TIME_STATS)
extern volatile uint32_t CPU_RunTime;

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() (CPU_RunTime = 0ul)
#define portGET_RUN_TIME_COUNTER_VALUE() CPU_RunTime
#endif /* configGENERATE_RUN_TIME_STATS */

// <h> FreeRTOS 与协程有关的配置选项
// =====================================================================
// <e> 使能协程功能
// <i> configUSE_CO_ROUTINES
// <i> 启用后必须添加文件 croutine.c 
#define configUSE_CO_ROUTINES 0
// </e> !使能协程功能

// <o> 最大协程优先级数目
// <i> configMAX_CO_ROUTINE_PRIORITIES
#define configMAX_CO_ROUTINE_PRIORITIES (2)
// </h> !FreeRTOS 与协程有关的配置选项

// <h> FreeRTOS 与软件定时器有关的配置选项
// =====================================================================
// <e> 使能软件定时器
// <i> configUSE_TIMERS
#define configUSE_TIMERS 1
// </e> !使能软件定时器

// <o> 软件定时器优先级
// <i> configTIMER_TASK_PRIORITY
// <i> 此处配置的数值是最大优先级减去的数值
// <i> 默认: 1 (configMAX_PRIORITIES - 1)
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)

// <o> 软件定时器队列长度
// <i> configTIMER_QUEUE_LENGTH
#define configTIMER_QUEUE_LENGTH 10

// <o> 软件定时器任务栈大小
// <i> configTIMER_TASK_STACK_DEPTH
// <i> 默认: 2 (2 * configMINIMAL_STACK_SIZE(空闲任务使用的堆栈大小))
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2)
// </h> !FreeRTOS 与软件定时器有关的配置选项

// <h> FreeRTOS 可选函数配置选项
// =====================================================================
// <e> INCLUDE_vTaskSuspend
// <i> 任务挂起函数/任务恢复函数
// <i> 此宏定义必须设置为 1 才能使用
// <i> vTaskSuspend(), vTaskResume() 这两个 API 函数接口。
#define INCLUDE_vTaskSuspend 1
// </e> !INCLUDE_vTaskSuspend

// <e> INCLUDE_xTaskResumeFromISR
// <i> 任务从中断恢复函数
// <i> 此宏与 INCLUDE_vTaskSuspend 定义必须同时设置为 1
// <i> 才能使用 xTaskResumeFromISR() 这个 API 函数接口。
#define INCLUDE_xTaskResumeFromISR 1
// </e> !INCLUDE_xTaskResumeFromISR

// <e> INCLUDE_vTaskDelete
// <i> 任务删除函数
// <i> 此宏定义必须设置为 1 才能使用 vTaskDelete() 这个 API 函数接口。
#define INCLUDE_vTaskDelete 1
// </e> !INCLUDE_vTaskDelete

// <e> INCLUDE_vTaskDelay
// <i> 任务延时函数 - 相对延时
// <i> 此宏定义必须设置为 1 才能使用 vTaskDelay() 这个 API 函数接口。
#define INCLUDE_vTaskDelay 1
// </e> !INCLUDE_vTaskDelay

// <e> INCLUDE_vTaskDelayUntil
// <i> 任务延时函数 - 绝对延时
// <i> 此宏定义必须设置为 1 才能使用 vTaskDelayUntil() 这个 API 函数接口。
#define INCLUDE_vTaskDelayUntil 1
// </e> !INCLUDE_vTaskDelayUntil

// <e> INCLUDE_xTaskGetSchedulerState
// <i> 此宏定义必须设置为 1 才能
// <i> 使用 xTaskGetSchedulerState() 这个 API 函数接口。
#define INCLUDE_xTaskGetSchedulerState 1
// </e> !INCLUDE_xTaskGetSchedulerState

// <e> INCLUDE_vTaskPrioritySet
// <i> 此宏定义必须设置为 1 才能使用 vTaskPrioritySet() 这个 API 函数接口。
#define INCLUDE_vTaskPrioritySet 1
// </e> !INCLUDE_vTaskPrioritySet

// <e> INCLUDE_uxTaskPriorityGet
// <i> 此宏定义必须设置为 1 才能使用 uxTaskPriorityGet() 这个 API 函数接口。
#define INCLUDE_uxTaskPriorityGet 1
// </e> !INCLUDE_uxTaskPriorityGet

// <e> INCLUDE_vTaskCleanUpResources
#define INCLUDE_vTaskCleanUpResources 1
// </e> !INCLUDE_vTaskCleanUpResources

// <e> INCLUDE_eTaskGetState
#define INCLUDE_eTaskGetState 1
// </e> !INCLUDE_eTaskGetState

// <e> INCLUDE_xTimerPendFunctionCall
// <i> 此宏定义必须设置为 1 才能使用 xEventGroupSetBitsFromISR() 这个 API 函数接口。
#define INCLUDE_xTimerPendFunctionCall 0
// </e> !INCLUDE_xTimerPendFunctionCall
// </h> !FreeRTOS 可选函数配置选项

// <h> FreeRTOS 与中断有关的配置选项
// =====================================================================
#ifdef __NVIC_PRIO_BITS
#if (__NVIC_PRIO_BITS == 4)
#define configPRIO_BITS 4
#elif (__NVIC_PRIO_BITS == 3)
#define configPRIO_BITS 5
#else
// <o> 中断优先级位数
// <i> configPRIO_BITS
#define configPRIO_BITS 4
#endif // (__NVIC_PRIO_BITS == 4)
#endif // __NVIC_PRIO_BITS

// <o> 中断最低优先级
// <i> configLIBRARY_LOWEST_INTERRUPT_PRIORITY
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY	15

// <o> 系统可管理的最高中断优先级
// <i> configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
// </h> !FreeRTOS 与中断有关的配置选项

#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << configPRIO_BITS)
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << configPRIO_BITS)

// <h> FreeRTOS 与中断服务函数有关的配置选项
// =====================================================================
#if (configUSE_TRACE_FACILITY)

// <e> 启用被 Trace 源码调用的可选函数
// <i> INCLUDE_xTaskGetCurrentTaskHandle
#define INCLUDE_xTaskGetCurrentTaskHandle 0
// </e> !启用被 Trace 源码调用的可选函数
#endif
// </h> !FreeRTOS 与中断服务函数有关的配置选项

// <<< end of configuration section >>>
#endif /* FREERTOS_CONFIG_H */
