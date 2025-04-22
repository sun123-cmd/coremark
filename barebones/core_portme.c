#include "coremark.h"
#include "core_portme.h"


#define TIMER_BASE_ADDRESS  0x40000000
#define TIMER_COUNTER_VALUE (*(volatile ee_u32 *)(TIMER_BASE_ADDRESS))

#define UART_BASE_ADDRESS   0x40001000
#define UART_DATA           (*(volatile ee_u32 *)(UART_BASE_ADDRESS))
#define UART_STATUS         (*(volatile ee_u32 *)(UART_BASE_ADDRESS + 0x4))
#define UART_READY_MASK     0x1

#if VALIDATION_RUN
volatile ee_s32 seed1_volatile = 0x3415;
volatile ee_s32 seed2_volatile = 0x3415;
volatile ee_s32 seed3_volatile = 0x66;
#endif
#if PERFORMANCE_RUN
volatile ee_s32 seed1_volatile = 0x0;
volatile ee_s32 seed2_volatile = 0x0;
volatile ee_s32 seed3_volatile = 0x66;
#endif
#if PROFILE_RUN
volatile ee_s32 seed1_volatile = 0x8;
volatile ee_s32 seed2_volatile = 0x8;
volatile ee_s32 seed3_volatile = 0x8;
#endif
volatile ee_s32 seed4_volatile = ITERATIONS;
volatile ee_s32 seed5_volatile = 0;

static CORETIMETYPE start_time_val, stop_time_val;


CORETIMETYPE barebones_clock() {
    return TIMER_COUNTER_VALUE;  
}


#define GETMYTIME(_t)              (*_t = barebones_clock())
#define MYTIMEDIFF(fin, ini)       ((fin) - (ini))
#define TIMER_RES_DIVIDER          1
#define SAMPLE_TIME_IMPLEMENTATION 1
#define EE_TICKS_PER_SEC           (1000000 / TIMER_RES_DIVIDER) 

void start_time(void) {
    GETMYTIME(&start_time_val);
}


void stop_time(void) {
    GETMYTIME(&stop_time_val);
}

/* 获取经过的时间 */
CORE_TICKS get_time(void) {
    return (CORE_TICKS)(MYTIMEDIFF(stop_time_val, start_time_val));
}

/* 将时间转换为秒 */
secs_ret time_in_secs(CORE_TICKS ticks) {
    secs_ret retval = ((secs_ret)ticks) / (secs_ret)EE_TICKS_PER_SEC;
    return retval;
}

ee_u32 default_num_contexts = 1;

/* UART 输出：实现 ee_printf */
int ee_printf(const char *fmt, ...) {
    va_list args;
    char buffer[128];
    int len;

    va_start(args, fmt);
    len = vsnprintf(buffer, sizeof(buffer), fmt, args);  // 格式化字符串
    va_end(args);

    for (int i = 0; i < len; i++) {
        while (!(UART_STATUS & UART_READY_MASK)) {
        }
        UART_DATA = buffer[i]; 
    }

    return len;
}

void portable_init(core_portable *p, int *argc, char *argv[]) {


    (void)argc;
    (void)argv; 

    if (sizeof(ee_ptr_int) != sizeof(ee_u8 *)) {
        ee_printf("ERROR! ee_ptr_int 必须能容纳指针类型！\n");
    }
    if (sizeof(ee_u32) != 4) {
        ee_printf("ERROR! ee_u32 必须是 32 位无符号类型！\n");
    }

    p->portable_id = 1;  // 表示初始化完成
}

/* 终止硬件（裸机终止） */
void portable_fini(core_portable *p) {
    p->portable_id = 0; 
}