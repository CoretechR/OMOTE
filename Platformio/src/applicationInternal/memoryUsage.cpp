#include <lvgl.h>
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/hardware/hardwarePresenter.h"

bool showMemoryUsage = 0;

#if (LV_USE_STDLIB_MALLOC == 0)
lv_mem_monitor_t mon;
#endif

static unsigned long updateSerialLogTimer = 0;

bool getShowMemoryUsage() {
  return showMemoryUsage;
}
// forward declaration
void doLogMemoryUsage();
void setShowMemoryUsage(bool aShowMemoryUsage) {
  showMemoryUsage = aShowMemoryUsage;
  showMemoryUsageBar(showMemoryUsage);
  doLogMemoryUsage();
}

void doLogMemoryUsage() {
  // Serial.println("inside doLogMemoryUsage");
  unsigned long systemHeapSize;
  unsigned long freeSystemHeap;
  unsigned long maxAllocSystemHeap;
  unsigned long minfreeSystemHeap;
  get_heapUsage(&systemHeapSize, &freeSystemHeap, &maxAllocSystemHeap, &minfreeSystemHeap);

  int thresholdForESP32HeapFreeWarning;  // in bytes free in heap
  #if (ENABLE_WIFI_AND_MQTT == 1) && (ENABLE_BLUETOOTH == 1)
    thresholdForESP32HeapFreeWarning = 15000;
  #elif ENABLE_WIFI_AND_MQTT == 1
    thresholdForESP32HeapFreeWarning = 11000;
  #elif ENABLE_BLUETOOTH == 1
    thresholdForESP32HeapFreeWarning = 5000;
  #elif ENABLE_WIFI_AND_MQTT == 1
    thresholdForESP32HeapFreeWarning = 5000;
  #endif
  bool doESPHeapWarning = (freeSystemHeap < thresholdForESP32HeapFreeWarning);

  bool doLVGLMemoryWarning = false;
  #if (LV_USE_STDLIB_MALLOC == 0)
  int thresholdForLVGLmemoryFreeWarning = 20; // in percentage free
  lv_mem_monitor(&mon);
  doLVGLMemoryWarning = ((100 - mon.used_pct) < thresholdForLVGLmemoryFreeWarning);
  #endif

  #if defined(SHOW_LOG_ON_SERIAL)
  // Serial log every 5 sec
  if(millis() - updateSerialLogTimer >= 5000) {
    // Serial.println("inside doLogMemoryUsage: will do serial log");
    updateSerialLogTimer = millis();

    if (doESPHeapWarning) {
      Serial.println("WARNING: ESP heap is getting low. You might encounter weird behaviour of your OMOTE, especially when using WiFi and/or BLE.");
    }
    Serial.printf(
      "ESP32 heap:  size: %6lu, used: %6lu (%2.0f%%), free: %6lu (%2.0f%%), heapMax:  %6lu, heapMin:      %6lu\r\n",
      systemHeapSize,
      systemHeapSize - freeSystemHeap, float(systemHeapSize - freeSystemHeap) / systemHeapSize * 100,
      freeSystemHeap,                     float(freeSystemHeap)                     / systemHeapSize * 100,
      maxAllocSystemHeap, minfreeSystemHeap);

    #if (LV_USE_STDLIB_MALLOC == 0)
    if (doLVGLMemoryWarning) {
      Serial.println("WARNING: LVGL memory is getting low. You GUI might stop working. In that case, increase \"-D LV_MEM_SIZE\" in platformio.ini");
    }
    Serial.printf(
      "lvgl memory: size: %6lu, used: %6lu (%2d%%), free: %6lu (%2d%%), max_used: %6lu, free_biggest: %6lu, frag_pct: %2d%%\r\n",
      mon.total_size,
      mon.total_size - mon.free_size, mon.used_pct,
      mon.free_size,                  100-mon.used_pct,
      mon.max_used, mon.free_biggest_size, mon.frag_pct);
    #endif
  } else {
    // Serial.println("inside doLogMemoryUsage: serial log skipped");
  }
  #endif

  if (showMemoryUsage) {
    char bufferHeap[40];
    char bufferLVGLmemory[40];
    #ifdef SHOW_USED_MEMORY_INSTEAD_OF_FREE_IN_GUI
      //sprintf(buffer, ESP32HeapWarnBegin.append("%lu/%lu (%.0f%%)").append(ESP32HeapWarnEnd).append(" / ").append(LVGLMemorWarnBegin).append("%lu/%lu (%d%%)").append(LVGLMemorWarnEnd).c_str(), systemHeapSize-freeSystemHeap, systemHeapSize, float(systemHeapSize-freeSystemHeap) / systemHeapSize * 100, mon.total_size - mon.free_size, mon.total_size, mon.used_pct);
      sprintf(bufferHeap,       std::string("").append("%lu/%lu (%.0f%%)\0").c_str(), systemHeapSize-freeSystemHeap,  systemHeapSize, float(systemHeapSize-freeSystemHeap) / systemHeapSize * 100);
      #if (LV_USE_STDLIB_MALLOC == 0)
      sprintf(bufferLVGLmemory, std::string("").append("%lu/%lu (%d%%)\0").c_str(),   mon.total_size - mon.free_size, mon.total_size, mon.used_pct);
      #endif
    #else
      //sprintf(buffer, ESP32HeapWarnBegin.append("%lu/%lu (%.0f%%)").append(ESP32HeapWarnEnd).append(" / ").append(LVGLMemorWarnBegin).append("%lu/%lu (%d%%)").append(LVGLMemorWarnEnd).c_str(), freeSystemHeap,                systemHeapSize, float(freeSystemHeap)                / systemHeapSize * 100, mon.free_size,                  mon.total_size, 100-mon.used_pct);
      sprintf(bufferHeap,       std::string("").append("%lu/%lu (%.0f%%)\0").c_str(), freeSystemHeap,                 systemHeapSize, float(freeSystemHeap)                / systemHeapSize * 100);
      #if (LV_USE_STDLIB_MALLOC == 0)
      sprintf(bufferLVGLmemory, std::string("").append("%lu/%lu (%d%%)\0").c_str(),   mon.free_size,                  mon.total_size, 100-mon.used_pct);
      #endif
    #endif

    // convert "." to ","
    for (int i=0; i<strlen(bufferHeap); i++) {
      if (bufferHeap[i] == '.') {
        bufferHeap[i] = ',';
      }
    }
    for (int i=0; i<strlen(bufferLVGLmemory); i++) {
      if (bufferLVGLmemory[i] == '.') {
        bufferLVGLmemory[i] = ',';
      }
    }

    // set text and color of heap used
    if (MemoryUsageSpanHeap != NULL) {
      lv_span_set_text(MemoryUsageSpanHeap, bufferHeap);
      if (doESPHeapWarning) {
        lv_style_set_text_color(&MemoryUsageSpanHeap->style, lv_palette_main(LV_PALETTE_RED));
      } else {
        lv_style_set_text_color(&MemoryUsageSpanHeap->style, lv_palette_main(LV_PALETTE_GREEN));
      }
    }  
    
    // only if LVGL's built in implementation of memory management is used 
    #if (LV_USE_STDLIB_MALLOC == 0)
    // first the separator
    if (MemoryUsageSpanSeparator != NULL) {
      lv_span_set_text(MemoryUsageSpanSeparator, " / ");
    }

    // set text and color of LVGL memory used
    if (MemoryUsageSpanLVGLmemory != NULL) {
      lv_span_set_text(MemoryUsageSpanLVGLmemory, bufferLVGLmemory);
      if (doLVGLMemoryWarning) {
        lv_style_set_text_color(&MemoryUsageSpanLVGLmemory->style, lv_palette_main(LV_PALETTE_RED));
      } else {
        lv_style_set_text_color(&MemoryUsageSpanLVGLmemory->style, lv_palette_main(LV_PALETTE_GREEN));
      }
    }
    #endif

  } else {
    // we don't show memory usage, so clear text
    if (MemoryUsageSpanHeap != NULL) {
      lv_span_set_text(MemoryUsageSpanHeap, "");
    }
    if (MemoryUsageSpanLVGLmemory != NULL) {
      lv_span_set_text(MemoryUsageSpanLVGLmemory, "");
    }
  }
}

