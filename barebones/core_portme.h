#ifndef CORE_PORTME_H
#define CORE_PORTME_H

/************************/
/* Data types and settings */
/************************/

/* Configuration : HAS_FLOAT
   Define to 1 if the platform supports floating point.
*/
#ifndef HAS_FLOAT
#define HAS_FLOAT 1
#endif

/* Configuration : HAS_TIME_H
   Define to 1 if platform has the time.h header file,
   and implementation of functions thereof.
   For bare-metal, set to 0.
*/
#ifndef HAS_TIME_H
#define HAS_TIME_H 0
#endif

/* Configuration : USE_CLOCK
   Define to 1 if platform has the time.h header file,
   and implementation of functions thereof.
   For bare-metal, set to 0.
*/
#ifndef USE_CLOCK
#define USE_CLOCK 0
#endif

/* Configuration : HAS_STDIO
   Define to 1 if the platform has stdio.h.
   For bare-metal, set to 0.
*/
#ifndef HAS_STDIO
#define HAS_STDIO 0
#endif

/* Configuration : HAS_PRINTF
   Define to 1 if the platform has stdio.h and implements the printf function.
   For bare-metal, set to 0.
*/
#ifndef HAS_PRINTF
#define HAS_PRINTF 0
#endif

/* Definitions : COMPILER_VERSION, COMPILER_FLAGS, MEM_LOCATION */
#ifndef COMPILER_VERSION
#ifdef __GNUC__
#define COMPILER_VERSION "GCC " __VERSION__
#else
#define COMPILER_VERSION "Please put compiler version here (e.g. gcc 4.1)"
#endif
#endif

#ifndef COMPILER_FLAGS
#define COMPILER_FLAGS "-O2 -ffreestanding -nostartfiles"
#endif

#ifndef MEM_LOCATION
#define MEM_LOCATION "STACK"
#endif

/* Data Types */
typedef signed short   ee_s16;
typedef unsigned short ee_u16;
typedef signed int     ee_s32;
typedef double         ee_f32;
typedef unsigned char  ee_u8;
typedef unsigned int   ee_u32;
typedef ee_u32         ee_ptr_int;
typedef size_t         ee_size_t;
#define NULL ((void *)0)

/* align_mem: Align pointer to 32-bit boundary */
#define align_mem(x) (void *)(4 + (((ee_ptr_int)(x)-1) & ~3))

/* Configuration : CORE_TICKS
   Define type of return from the timing functions.
 */
#define CORETIMETYPE ee_u32
typedef ee_u32 CORE_TICKS;

/* Configuration : SEED_METHOD */
#ifndef SEED_METHOD
#define SEED_METHOD SEED_VOLATILE
#endif

/* Configuration : MEM_METHOD */
#ifndef MEM_METHOD
#define MEM_METHOD MEM_STATIC
#endif

/* Configuration : MULTITHREAD */
#ifndef MULTITHREAD
#define MULTITHREAD 1
#endif

/* Configuration : MAIN_HAS_NOARGC */
#ifndef MAIN_HAS_NOARGC
#define MAIN_HAS_NOARGC 1
#endif

/* Configuration : MAIN_HAS_NORETURN */
#ifndef MAIN_HAS_NORETURN
#define MAIN_HAS_NORETURN 1
#endif

/* Variable : default_num_contexts */
extern ee_u32 default_num_contexts;

typedef struct CORE_PORTABLE_S
{
    ee_u8 portable_id;
} core_portable;

/* Target specific init/fini */
void portable_init(core_portable *p, int *argc, char *argv[]);
void portable_fini(core_portable *p);

/* Timer functions for bare-metal */
void start_time(void);
void stop_time(void);
CORE_TICKS get_time(void);

/* Custom printf for bare-metal */
int ee_printf(const char *fmt, ...);

/* Define run type based on TOTAL_DATA_SIZE */
#if !defined(PROFILE_RUN) && !defined(PERFORMANCE_RUN) && !defined(VALIDATION_RUN)
#if (TOTAL_DATA_SIZE == 1200)
#define PROFILE_RUN 1
#elif (TOTAL_DATA_SIZE == 2000)
#define PERFORMANCE_RUN 1
#else
#define VALIDATION_RUN 1
#endif
#endif

#endif /* CORE_PORTME_H */