//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"

#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_desc.h"
#include "BlinkLed.h"
#include "stm32f4xx_hal.h"

// ----------------------------------------------------------------------------
//
// STM32F4 led blink sample (trace via ITM).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// To demonstrate POSIX retargetting, reroute the STDOUT and STDERR to the
// trace device and display messages on both of them.
//
// Then demonstrates how to blink a led with 1Hz, using a
// continuous loop and SysTick delays.
//
// On DEBUG, the uptime in seconds is also displayed on the trace device.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the ITM output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//


// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define TIMER_FREQUENCY_HZ 1000
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 2 / 3)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)



/* Variables used for USB */
USBD_HandleTypeDef  hUSBDDevice;

static uint32_t Demo_USBConfig(void);


// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
  // By customising __initialize_args() it is possible to pass arguments,
  // for example when running tests with semihosting you can pass various
  // options to the test.
  // trace_dump_args(argc, argv);

  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // The standard output and the standard error should be forwarded to
  // the trace device. For this to work, a redirection in _write.c is
  // required.
  puts("Standard output message.");
  fprintf(stderr, "Standard error message.\n");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %uHz\n", SystemCoreClock);

  blink_led_init();

  Demo_USBConfig();

  uint32_t seconds = 0;

  // Infinite loop
  while (1)
    {
      blink_led_on();
      HAL_Delay(BLINK_ON_TICKS);

      blink_led_off();
      HAL_Delay(BLINK_OFF_TICKS);

      ++seconds;

      // Count seconds on the trace device.
      trace_printf("Second %u\n", seconds);
    }
  // Infinite loop, never return.
}



/**
  * @brief  Initializes the USB for the demonstration application.
  * @param  None
  * @retval None
  */
static uint32_t Demo_USBConfig(void)
{
  /* Init Device Library */
  USBD_Init(&hUSBDDevice, &CDC_Desc, 0);

  hUSBDDevice.pUserData = &USBD_CDC_Usr_fops;

  /* Add Supported Class */
  USBD_RegisterClass(&hUSBDDevice, USBD_CDC_CLASS);

  /* Start Device Process */
  USBD_Start(&hUSBDDevice);

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
