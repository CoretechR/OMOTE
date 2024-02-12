#include <lvgl.h>
#include <string>
#include <iostream>
#include "hardware/tft.h"
#include "hardware/sleep.h"
#include "hardware/infrared_receiver.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"

lv_obj_t* menuBoxToggle;
lv_obj_t* menuBoxMessages;
int16_t boxHeightDeactivated = 0;
int16_t boxHeightActivated = 200;
const int maxCountMessages = 15;
lv_obj_t* irReceivedMessage[maxCountMessages];
String IRmessages[maxCountMessages];
int messagePos = 0;
int messageCount = 0;

void printReceivedMessages(bool clearMessages = false) {
  //Serial.println("");
  int messagePosLoop;
  if (messageCount < maxCountMessages) {
    messagePosLoop = 0;
  } else {
    messagePosLoop = messagePos;
  }

  //Serial.printf("will start printing messages, beginning at position %d\r\n", messagePosLoop);
  for (int i=0; i<maxCountMessages; i++) {
    if (clearMessages) {
      IRmessages[messagePosLoop] = "";
    }
    //Serial.printf("will print at line %d the message at position %d: %s\r\n", i, messagePosLoop, IRmessages[messagePosLoop].c_str());
    lv_label_set_text(irReceivedMessage[i], IRmessages[messagePosLoop].c_str());
    messagePosLoop += 1;
    if (messagePosLoop == maxCountMessages) {
      messagePosLoop = 0;  
    }
  }
  if (clearMessages) {
    lv_obj_set_size(menuBoxMessages, lv_pct(100), boxHeightDeactivated);
  }
}

void showNewIRmessage(String message) {
  resetStandbyTimer(); // Reset the sleep timer when a IR message is received

  // Serial.printf("  new IR message received: %s\r\n", message.c_str());
  // const char *a = message.c_str();
  std::string messageStr;
  messageStr.append(message.c_str());
  // std::string aMessage = s(a);
  messageStr.erase(std::remove(messageStr.begin(), messageStr.end(), '\n'), messageStr.cend());
  
  //Serial.printf(" will put message %s to list\r\n", messageStr.c_str());
  messageCount += 1;
  IRmessages[messagePos] = (std::to_string(messageCount) + ": " + messageStr).c_str();
  //Serial.printf(" this is the message at position %d: %s\r\n", messagePos, IRmessages[messagePos].c_str());
  messagePos += 1;
  if (messagePos == maxCountMessages) {
    messagePos = 0;  
  }
  printReceivedMessages();
}

// IR receiver on Switch Event handler
static void IRReceiverOnSetting_event_cb(lv_event_t * e){
  irReceiverEnabled = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
  if (irReceiverEnabled) {
    Serial.println("will turn on IR receiver");
    init_infraredReceiver();
    lv_obj_set_size(menuBoxMessages, lv_pct(100), boxHeightActivated);
    messageCount = 0;
    printReceivedMessages();
  } else {
    Serial.println("will turn off IR receiver");
    shutdown_infraredReceiver();
    printReceivedMessages(true);
    messagePos = 0;
    messageCount = 0;
    lv_obj_set_size(menuBoxMessages, lv_pct(100), boxHeightDeactivated);
  }
}

void init_gui_tab_irReceiver(lv_obj_t* tabview) {

  lv_obj_t* tab = lv_tabview_add_tab(tabview, "IR Receiver");

  // Add content to the settings tab
  // With a flex layout, setting groups/boxes will position themselves automatically
  lv_obj_set_layout(tab, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab, LV_SCROLLBAR_MODE_ACTIVE);

  menuBoxToggle = lv_obj_create(tab);
  lv_obj_set_size(menuBoxToggle, lv_pct(100), 48);
  lv_obj_set_style_bg_color(menuBoxToggle, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBoxToggle, 0, LV_PART_MAIN);

  lv_obj_t* menuLabel = lv_label_create(menuBoxToggle);
  lv_label_set_text(menuLabel, "Turn on IR receiver");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 3);
  lv_obj_t* irReceiverToggle = lv_switch_create(menuBoxToggle);
  lv_obj_set_size(irReceiverToggle, 40, 22);
  lv_obj_align(irReceiverToggle, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(irReceiverToggle, lv_color_hex(0x505050), LV_PART_MAIN);
  lv_obj_add_event_cb(irReceiverToggle, IRReceiverOnSetting_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  // lv_obj_add_state(irReceiverToggle, LV_STATE_CHECKED); // set default state


  menuBoxMessages = lv_obj_create(tab);
  lv_obj_set_size(menuBoxMessages, lv_pct(100), 77); // 125
  lv_obj_set_style_bg_color(menuBoxMessages, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBoxMessages, 0, LV_PART_MAIN);
  
  for (int i=0; i<maxCountMessages; i++) {
    irReceivedMessage[i] = lv_label_create(menuBoxMessages);
    lv_obj_set_style_text_font(irReceivedMessage[i], &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_align(irReceivedMessage[i], LV_ALIGN_TOP_LEFT, 0, 0 + i*11);
  }
  printReceivedMessages(true);
}

void init_gui_pageIndicator_irReceiver(lv_obj_t* panel) {
  // Create actual (non-clickable) buttons for every tab
  lv_obj_t* btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  lv_obj_t* label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "IR Receiver");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);
}

void register_gui_irReceiver(void){
  register_gui(& init_gui_tab_irReceiver, & init_gui_pageIndicator_irReceiver);
}
