/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/micro/debug_log.h"

#include "stm32wbxx_hal.h"

extern UART_HandleTypeDef huart1;


int _write(int file, char *ptr, int len) {

  HAL_StatusTypeDef tx_status;

  tx_status = HAL_UART_Transmit(&huart1, (uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);

  if (tx_status == HAL_OK) {
    return len;
  }
  else {
    return 0;
  }
}


int _strlen(const char* str) {
  int len = 0;

  while (str[len] != '\0') {
	len++;
  }

  return len;
}


extern "C" void DebugLog(const char *s) {
  int len = _strlen(s);
  _write(1, (char*)s, len);
}
