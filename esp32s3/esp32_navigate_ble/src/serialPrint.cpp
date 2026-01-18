#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "serialPrint.h"

SemaphoreHandle_t __print_mutex;

void __vSafePrintInit(void) {
    __print_mutex = xSemaphoreCreateMutex();
}

void __vSafeSerialPrint(const String& out) {
    if (!__print_mutex) return;
	xSemaphoreTake(__print_mutex, portMAX_DELAY);
	Serial.print(out);
	xSemaphoreGive(__print_mutex);
}

void __vSafeSerialPrintln(const String& out) {
    if (!__print_mutex) return;
	xSemaphoreTake(__print_mutex, portMAX_DELAY);
	Serial.println(out);
	xSemaphoreGive(__print_mutex);
}