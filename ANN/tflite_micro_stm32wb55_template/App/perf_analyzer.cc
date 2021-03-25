/*
 * perf_analyzer.cc
 *
 */
/* System headers */
#include "perf_analyzer.h"
#include "main.h" // this will also include the HAL functions
#include "tensorflow/lite/micro/micro_error_reporter.h"
// #include "tensorflow/lite/c/common.h"

/* APP configuration 0: disabled 1: enabled */
#define FIX_CLK_OVERFLOW 1

tflite::ErrorReporter* error_reporter = nullptr;

#if defined(CHECK_STM32_FAMILY)
#if !defined(STM32F7) && !defined(STM32L4) && !defined(STM32L5) && !defined(STM32F4) && !defined(STM32H7) && !defined(STM32F3)
#error Only STM32H7, STM32F7, STM32F4, STM32L4, STM32L5 or STM32F3 device are supported
#endif
#endif

/* -----------------------------------------------------------------------------
 * Device-related functions
 * -----------------------------------------------------------------------------
 */

static struct cyclesCount {
  uint32_t dwt_max;
  uint32_t dwt_start;
  uint32_t tick_start;
} cyclesCount;


static inline void crcIpInit(void) {
#if defined(STM32H7)
  /* By default the CRC IP clock is enabled */
  __HAL_RCC_CRC_CLK_ENABLE();
#else
  if (!__HAL_RCC_CRC_IS_CLK_ENABLED())
    TF_LITE_REPORT_ERROR(error_reporter,
                         "W: CRC IP clock is NOT enabled\r\n");

  /* By default the CRC IP clock is enabled */
  __HAL_RCC_CRC_CLK_ENABLE();
#endif
}


static inline void dwtIpInit(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

#ifdef STM32F7
  DWT->LAR = 0xC5ACCE55;
#endif

  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk | DWT_CTRL_CPIEVTENA_Msk;
  // DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}


static inline void dwtReset(void) {
  DWT->CYCCNT = 0; /* Clear DWT cycle counter */
}


static inline uint32_t dwtGetCycles(void) {
  return DWT->CYCCNT;
}


static inline uint32_t systemCoreClock(void) {
#if !defined(STM32H7)
  return HAL_RCC_GetHCLKFreq();
#else
  return HAL_RCC_GetSysClockFreq();
#endif
}


static inline int dwtCyclesToTime(uint64_t clks, struct dwtTime *t) {
  if (!t)
    return -1;
  uint32_t fcpu = systemCoreClock();
  uint64_t s  = clks / fcpu;
  uint64_t ms = (clks * 1000) / fcpu;
  uint64_t us = (clks * 1000 * 1000) / fcpu;
  ms -= (s * 1000);
  us -= (ms * 1000 + s * 1000000);
  t->fcpu = fcpu;
  t->s = s;
  t->ms = ms;
  t->us = us;
  return 0;
}


static inline const char *devIdToStr(uint16_t dev_id) {
  /* DEV_ID field from DBGMCU register
   * see also: https://community.st.com/s/question/0D50X00009XkbKDSAZ/where-is-the-mcus-id
   * */
  const char *str;
  switch (dev_id) {
    case 0x422: str = "STM32F303xB/C"; break;
    case 0x438: str = "STM32F303x6/8"; break;
    case 0x446: str = "STM32F303xD/E"; break;
    case 0x431: str = "STM32F411xC/E"; break;
    case 0x421: str = "STM32F446xx"; break;
    case 0x435: str = "STM32L43xxx"; break;
    case 0x462: str = "STM32L45xxx"; break;
    case 0x415: str = "STM32L4x6xx"; break;
    case 0x470: str = "STM32L4Rxxx"; break;
    case 0x472: str = "STM32L5[5,6]2xx"; break;
    case 0x449: str = "STM32F74xxx"; break;
    case 0x450: str = "STM32H743/753 and STM32H750"; break;
    default:    str = "UNKNOWN";
  }
  return str;
}


#if !defined(STM32F3)
static inline const char* bitToStr(uint32_t val) {
  if (val)
    return "True";
  else
    return "False";
}
#endif


static inline void cyclesCounterInit(void) {
    struct dwtTime t;
    dwtCyclesToTime(UINT32_MAX, &t);
    cyclesCount.dwt_max = t.s * 1000 + t.ms;
    dwtIpInit();
}


static inline void cyclesCounterStart(void) {
    cyclesCount.tick_start = HAL_GetTick();
    dwtReset();
    cyclesCount.dwt_start = dwtGetCycles();
}


static inline uint64_t cyclesCounterEnd(void) {
#if FIX_CLK_OVERFLOW == 1
    uint64_t dwt_e;
    uint64_t tick_e;
    dwt_e = dwtGetCycles() - cyclesCount.dwt_start;
    tick_e = HAL_GetTick() - cyclesCount.tick_start;
    if (tick_e > cyclesCount.dwt_max) {
        // overflow detected
        // dwt_e = (tick_e * (uint64_t)systemCoreClock()) / 1000;
        dwt_e = ((tick_e / cyclesCount.dwt_max) * (uint64_t)UINT32_MAX + (uint64_t)dwt_e);
    }
    return dwt_e;
#else
    return (uint64_t)(dwtGetCycles() - cyclesCount.dwt_start);
#endif
}


static uint32_t mconf;

uint32_t getFlashCacheConf(void) {
  return mconf;
}


void cycles_per_macc_benchmark() {
  // Test cycles per MACC for int8_t and float data types:
  const uint8_t array_size = 3;
  int8_t v1_data_int8[] = {1, 2, 3};
  int8_t v2_data_int8[] = {4, 5, 6};
  int8_t acc_int8 = 0;
  float v1_data_float[] = {1.0f, 2.0f, 3.0f};
  float v2_data_float[] = {4.0f, 5.0f, 6.0f};
  float acc_float = 0.0f;
  uint32_t cycles;

  // Reset cycle counter
  DWT->CYCCNT = 0;
  // Compute 3 MACCs (int8_t)
  for (uint8_t i = 0; i < array_size; i++) {
    acc_int8 += v1_data_int8[i] * v2_data_int8[i];
  }
  // Read the number of clock cycles passed
  cycles = DWT->CYCCNT;
  TF_LITE_REPORT_ERROR(error_reporter,
                       " Cycles / MACC (avg. value) for int8_t data: %u",
                       cycles / array_size);

  // Reset cycle counter
  DWT->CYCCNT = 0;
  // Compute 3 MACCs (float)
  for (uint8_t i = 0; i < array_size; i++) {
    acc_float += v1_data_float[i] * v2_data_float[i+1];
  }
  // Read the number of clock cycles passed
  cycles = DWT->CYCCNT;

  TF_LITE_REPORT_ERROR(error_reporter,
                       " Cycles / MACC (avg. value) for float data: %u\n",
                       cycles / array_size);
}


void logDeviceConf(void) {
#if !defined(STM32L5)
  uint32_t acr = FLASH->ACR;
#endif
  uint32_t val;

  TF_LITE_REPORT_ERROR(error_reporter, "STM32 Runtime configuration...");

  TF_LITE_REPORT_ERROR(error_reporter,
                       " Device       : DevID:%x (%s) RevID:%x",
                       (int)HAL_GetDEVID(),
                       devIdToStr(HAL_GetDEVID()),
                       (int)HAL_GetREVID());

  TF_LITE_REPORT_ERROR(error_reporter,
                       " Core Arch.   : M%d - %s %s",
                       __CORTEX_M,
#if (__FPU_PRESENT == 1)
                       "FPU PRESENT",
                       __FPU_USED ? "and used" : "and not used!"
#else
                       "FPU NOT PRESENT",
                       ""
#endif
  );

#if (__FPU_PRESENT == 1) && __FPU_USED
  mconf = (1 << 16);  /* FPU present and used */
#endif

  TF_LITE_REPORT_ERROR(error_reporter,
                       " HAL version  : %x", (int)HAL_GetHalVersion());

  val = systemCoreClock() / 1000000;

#if !defined(STM32H7)
  TF_LITE_REPORT_ERROR(error_reporter, " system clock : %u MHz", (int)val);
#else
  TF_LITE_REPORT_ERROR(error_reporter, " SYSCLK clock : %u MHz", (int)val);
  TF_LITE_REPORT_ERROR(error_reporter, " HCLK clock   : %u MHz",
                       (int)HAL_RCC_GetHCLKFreq() / 1000000);
#endif

#if defined(STM32F7) || defined(STM32H7)
  val = SCB->CCR;
#if !defined(STM32H7)
  mconf |= (2 << 24);  /* F7 conf. */
  mconf |= ((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);
  if ((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos)
    mconf |= (1 << 8);
  if ((acr & FLASH_ACR_ARTEN_Msk) >> FLASH_ACR_ARTEN_Pos)
    mconf |= (1 << 9);
  TF_LITE_REPORT_ERROR(error_reporter,
                       " FLASH conf.  : ACR=%x - Prefetch=%s ART=%s latency=%d",
                       (int)acr,
                       bitToStr((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos),
                       bitToStr((acr & FLASH_ACR_ARTEN_Msk) >> FLASH_ACR_ARTEN_Pos),
                       (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#else
  mconf |= (3 << 24);  /* H7 conf. */
  mconf |= (acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos;
  TF_LITE_REPORT_ERROR(error_reporter,
                       " FLASH conf.  : ACR=0x%08x - latency=%d",
                       (int)acr,
                       (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#endif
#if !defined(CORE_M4)
  if (val & SCB_CCR_IC_Msk)
    mconf |= (1 << 10);
  if (val & SCB_CCR_DC_Msk)
    mconf |= (1 << 11);
  TF_LITE_REPORT_ERROR(error_reporter, " CACHE conf.  : $I/$D=(%s,%s)",
                       bitToStr(val & SCB_CCR_IC_Msk), bitToStr(val & SCB_CCR_DC_Msk));
#endif
#else
#if !defined(STM32F3) && !defined(STM32L5)
  mconf |= (1 << 24);  /* F4/L4 conf. */
  mconf |= ((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);
  if ((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos)
    mconf |= (1 << 8);
  if ((acr & FLASH_ACR_ICEN_Msk) >> FLASH_ACR_ICEN_Pos)
    mconf |= (1 << 9);
  if ((acr & FLASH_ACR_DCEN_Msk) >> FLASH_ACR_DCEN_Pos)
    mconf |= (1 << 10);
  TF_LITE_REPORT_ERROR(error_reporter,
                       " FLASH conf.  : ACR=%x - Prefetch=%s $I/$D=(%s,%s) latency=%d",
                       (int)acr,
                       bitToStr((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos),
                       bitToStr((acr & FLASH_ACR_ICEN_Msk) >> FLASH_ACR_ICEN_Pos),
                       bitToStr((acr & FLASH_ACR_DCEN_Msk) >> FLASH_ACR_DCEN_Pos),
                       (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#endif
#if defined(STM32L5)
  TF_LITE_REPORT_ERROR(error_reporter,
                       " ICACHE       : %s",
                       bitToStr(READ_BIT(ICACHE->CR, ICACHE_CR_EN)));
#endif
#endif

  cycles_per_macc_benchmark();
}


static inline uint32_t disableInts(void) {
  uint32_t state;

  state = __get_PRIMASK();
  __disable_irq();

  return state;
}


static inline void restoreInts(uint32_t state) {
  __set_PRIMASK(state);
}


float dwtCyclesToFloatMs(uint64_t clks) {
  float fcpu = (float)systemCoreClock();

  return ((float)clks * 1000.0f) / fcpu;
}


void perfAnalyzerInit() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // crcIpInit();
  cyclesCounterInit();
  logDeviceConf();

  dwtReset();
}


void perfAnalyzerStart(void) {
  cyclesCounterStart();
}


float perfAnalyzerGetLatency(void) {
  uint64_t dwt_cycles = cyclesCounterEnd();

  return dwtCyclesToFloatMs(dwt_cycles);
}


/* -----------------------------------------------------------------------------
 * low-level I/O functions
 * -----------------------------------------------------------------------------
 */

static struct ia_malloc {
  uint32_t cfg;
  uint32_t alloc;
  uint32_t free;
  uint32_t alloc_req;
  uint32_t free_req;
} ia_malloc;

#define MAGIC_MALLOC_NUMBER 0xefdcba98

void* __real_malloc(size_t bytes);
void __real_free(void *ptr);

void* __wrap_malloc(size_t bytes) {
  uint8_t *ptr;

  ia_malloc.cfg |= 1 << 1;

  /* ensure alignment for magic number */
  bytes = (bytes + 3) & ~3;

  /* add 2x32-bit for size and magic  number */
  ptr = (uint8_t*)__real_malloc(bytes + 8);

  /* remember size */
  if (ptr) {
    *((uint32_t*)ptr) = bytes;
    *((uint32_t*)(ptr + 4 + bytes)) = MAGIC_MALLOC_NUMBER;
  }

  if ((ptr) && (ia_malloc.cfg & 1UL)) {
    ia_malloc.alloc_req++;
    ia_malloc.alloc += bytes;
  }
  return ptr?(ptr + 4):NULL;
}

void __wrap_free(void *ptr) {
  uint8_t* p;
  uint32_t bytes;

  ia_malloc.cfg |= 1 << 2;

  if (!ptr)
    return;

  p = (uint8_t*)ptr - 4;
  bytes = *((uint32_t*)p);

  if (*((uint32_t*)(p + 4 + bytes)) == MAGIC_MALLOC_NUMBER) {
    *((uint32_t*)(p + 4 + bytes)) = 0;
  }

  if (ia_malloc.cfg & 1UL) {
    ia_malloc.free_req++;
    ia_malloc.free += bytes;
  }
  __real_free(p);
}

