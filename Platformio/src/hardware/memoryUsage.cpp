#include <Arduino.h>
#include <lvgl.h>
#include "gui_general_and_keys/guiBase.h"

bool showMemoryUsage = 0;

#if LV_MEM_CUSTOM == 0
lv_mem_monitor_t mon;
#endif

static unsigned long updateSerialLogTimer = 0;

bool getShowMemoryUsage() {
  return showMemoryUsage;
}
void setShowMemoryUsage(bool aShowMemoryUsage) {
  showMemoryUsage = aShowMemoryUsage;
  showMemoryUsageBar(showMemoryUsage);
}

void doLogMemoryUsage() {
  // Serial.println("inside doLogMemoryUsage");

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
  bool doESPHeapWarning = (ESP.getFreeHeap() < thresholdForESP32HeapFreeWarning);

  bool doLVGLMemoryWarning = false;
  #if LV_MEM_CUSTOM == 0
  int thresholdForLVGLmemoryFreeWarning = 20; // in percentage free
  lv_mem_monitor(&mon);
  doLVGLMemoryWarning = ((100 - mon.used_pct) < thresholdForLVGLmemoryFreeWarning);
  #endif

  // Serial log every 5 sec
  if(millis() - updateSerialLogTimer >= 5000) {
    // Serial.println("inside doLogMemoryUsage: will do serial log");
    updateSerialLogTimer = millis();

    if (doESPHeapWarning) {
      Serial.println("WARNING: ESP heap is getting low. You might encounter weird behaviour of your OMOTE, especially when using WiFi and/or BLE.");
    }
    Serial.printf(
      "ESP32 heap:  size: %6lu, used: %6lu (%2.0f%%), free: %6lu (%2.0f%%), heapMax:  %6lu, heapMin:      %6lu\r\n",
      ESP.getHeapSize(),
      ESP.getHeapSize() - ESP.getFreeHeap(), float(ESP.getHeapSize() - ESP.getFreeHeap()) / ESP.getHeapSize() * 100,
      ESP.getFreeHeap(),                     float(ESP.getFreeHeap())                     / ESP.getHeapSize() * 100,
      ESP.getMaxAllocHeap(), ESP.getMinFreeHeap());

    #if LV_MEM_CUSTOM == 0
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

  if (showMemoryUsage) {
    char buffer[80];
    std::string ESP32HeapWarnBegin = "#00ff00 "; // green
    std::string ESP32HeapWarnEnd   = "#";
    std::string LVGLMemorWarnBegin = "#00ff00 "; // green
    std::string LVGLMemorWarnEnd   = "#";
    if (doESPHeapWarning) {
      ESP32HeapWarnBegin = "#ff0000 "; // red
      ESP32HeapWarnEnd   = "#";
    }
    if (doLVGLMemoryWarning) {
      LVGLMemorWarnBegin = "#ff0000 "; // red
      LVGLMemorWarnEnd   = "#";
    }
    #if LV_MEM_CUSTOM != 0
      #ifdef SHOW_USED_MEMORY_INSTEAD_OF_FREE_IN_GUI

      sprintf(buffer, ESP32HeapWarnBegin.append("%lu/%lu (%.0f%%)").append(ESP32HeapWarnEnd).c_str()                                                                                           , ESP.getHeapSize()-ESP.getFreeHeap(), ESP.getHeapSize(), float(ESP.getHeapSize()-ESP.getFreeHeap()) / ESP.getHeapSize() * 100);
      #else
      sprintf(buffer, ESP32HeapWarnBegin.append("%lu/%lu (%.0f%%)").append(ESP32HeapWarnEnd).c_str()                                                                                           , ESP.getFreeHeap(),                   ESP.getHeapSize(), float(ESP.getFreeHeap())                   / ESP.getHeapSize() * 100);
      #endif
    #else
      #ifdef SHOW_USED_MEMORY_INSTEAD_OF_FREE_IN_GUI
      sprintf(buffer, ESP32HeapWarnBegin.append("%lu/%lu (%.0f%%)").append(ESP32HeapWarnEnd).append(" / ").append(LVGLMemorWarnBegin).append("%lu/%lu (%d%%)").append(LVGLMemorWarnEnd).c_str(), ESP.getHeapSize()-ESP.getFreeHeap(), ESP.getHeapSize(), float(ESP.getHeapSize()-ESP.getFreeHeap()) / ESP.getHeapSize() * 100, mon.total_size - mon.free_size, mon.total_size, mon.used_pct);
      #else
      sprintf(buffer, ESP32HeapWarnBegin.append("%lu/%lu (%.0f%%)").append(ESP32HeapWarnEnd).append(" / ").append(LVGLMemorWarnBegin).append("%lu/%lu (%d%%)").append(LVGLMemorWarnEnd).c_str(), ESP.getFreeHeap(),                   ESP.getHeapSize(), float(ESP.getFreeHeap())                   / ESP.getHeapSize() * 100, mon.free_size,                  mon.total_size, 100-mon.used_pct);
      #endif
    #endif

    for (int i=0; i<strlen(buffer); i++) {
      if (buffer[i] == '.') {
        buffer[i] = ',';
      }
    }
    if (MemoryUsageLabel != NULL) {
      // Serial.printf("inside doLogMemoryUsage: will do GUI log %s\r\n", buffer);
      lv_label_set_text(MemoryUsageLabel, buffer);
    }
  } else {
    if (MemoryUsageLabel != NULL) {
      lv_label_set_text(MemoryUsageLabel, "");
    }
  }
}

