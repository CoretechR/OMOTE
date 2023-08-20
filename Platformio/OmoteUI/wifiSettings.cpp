#include "OmoteUI.hpp"

#define WIFI_SUBPAGE_SIZE 3
static char* ssid;

lv_obj_t* OmoteUI::create_wifi_selection_page(lv_obj_t* menu)
{
  /* Create sub page for wifi*/
  lv_obj_t* subpage = lv_menu_page_create(menu, NULL);
  this->wifi_setting_cont = lv_menu_cont_create(subpage);
  lv_obj_set_layout(this->wifi_setting_cont, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(this->wifi_setting_cont, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(this->wifi_setting_cont, LV_SCROLLBAR_MODE_ACTIVE);

  lv_obj_t* menuLabel = lv_label_create(this->wifi_setting_cont);
  lv_label_set_text(menuLabel, "Searching for wifi networks");


  return subpage;
}

/**
 * @brief Callback function for the show password checkbox. Checking the box will show the password while unchecked the
 * password will be shown as dots.
 * 
 * @param e Pointer to event object for the event where this callback is called
 */
static void show_password_cb(lv_event_t* e)
{
  lv_obj_t* password_field = (lv_obj_t*) e->user_data;
  if (lv_obj_has_state(e->target, LV_STATE_CHECKED)){
    lv_textarea_set_password_mode(password_field, false);
  }
  else{
    lv_textarea_set_password_mode(password_field, true);
  }

}

/**
 * @brief Textarea callback function for the password field. In case the enter key is pressed in the text area, the 
 * function will try to connect to the network with the provided password. 
 * 
 * @param e Pointer to event object for the event where this callback is called
 */
void OmoteUI::password_field_event_cb(lv_event_t* e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  lv_obj_t * kb = (lv_obj_t*) lv_event_get_user_data(e);

  const char* password = lv_textarea_get_text(ta);
  switch(code){
    case LV_EVENT_READY:
      this->mHardware->wifi_connect.notify(std::make_shared<std::string>(std::string(ssid)), std::make_shared<std::string>(std::string(password)));
      lv_obj_clear_state(ta, LV_STATE_FOCUSED);
      this->hide_keyboard();
      this->reset_settings_menu();
      /* Fall through on purpose. Pressing enter should disable the keyboard as well*/
    default:
      break;

  }
}

/**
 * @brief Callback which is triggered when clicking the connect button. It triggers the wifi connection.
 * 
 * @param event Pointer to event object for the event where this callback is called
 */
void OmoteUI::connect_btn_cb(lv_event_t* event)
{
  lv_obj_t* ta = (lv_obj_t*) event->user_data;
  const char* password = lv_textarea_get_text(ta);

  this->mHardware->wifi_connect.notify(std::make_shared<std::string>(std::string(ssid)), std::make_shared<std::string>(std::string(password)));
//Trigger wifi connection here
  //wifihandler.connect(ssid, password);
  lv_obj_clear_state(ta, LV_STATE_FOCUSED);
  this->hide_keyboard(); 
  this->reset_settings_menu();
  
}

void OmoteUI::create_wifi_main_page(lv_obj_t* parent)
{
  lv_obj_t* menuLabel = lv_label_create(parent);
  lv_label_set_text(menuLabel, "Wi-Fi");
  this->wifiOverview = lv_obj_create(parent);
  lv_obj_set_size(this->wifiOverview, lv_pct(100), 80);
  lv_obj_set_style_bg_color(this->wifiOverview, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(this->wifiOverview, 0, LV_PART_MAIN);
  menuLabel = lv_label_create(this->wifiOverview);

  lv_obj_t* arrow = lv_label_create(this->wifiOverview);
  lv_label_set_text(arrow, LV_SYMBOL_RIGHT);
  lv_obj_align(arrow, LV_ALIGN_TOP_RIGHT, 0, 0);

  lv_obj_t* ip_label = lv_label_create(this->wifiOverview);
  lv_label_set_text(ip_label, "IP:");
  lv_obj_align(ip_label, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  
  lv_obj_t* ip = lv_label_create(this->wifiOverview);
  lv_obj_align(ip, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

  lv_label_set_text(menuLabel, "Disconnected"); 
  lv_label_set_text(ip, "-");

  lv_menu_set_load_page_event(this->settingsMenu, this->wifiOverview, this->wifi_selection_page);
  lv_obj_add_event_cb(this->wifiOverview, [] (lv_event_t* e) {mInstance->wifi_settings_cb(e);}, LV_EVENT_CLICKED, this->wifi_setting_cont);
}

void OmoteUI::wifi_scan_done(std::shared_ptr<std::vector<WifiInfo>> info)
{
  unsigned int size = info->size();
  this->no_subpages = (size + WIFI_SUBPAGE_SIZE - 1)/WIFI_SUBPAGE_SIZE;
  this->no_wifi_networks = size;
  this->found_wifi_networks = info;

  if (size == 0)
  {
    lv_obj_t* menuBox = lv_obj_create(this->wifi_setting_cont);
    lv_obj_set_size(menuBox, lv_pct(100), 45);
    lv_obj_set_scrollbar_mode(menuBox, LV_SCROLLBAR_MODE_OFF);
    lv_obj_t* menuLabel = lv_label_create(menuBox);
    lv_label_set_text(menuLabel, "no networks found");
  }
  else
  {
    this->update_wifi_selection_subpage(0);
  }
}
void OmoteUI::next_wifi_selection_subpage(lv_event_t* e)
{
  int subpage = (int) lv_event_get_user_data(e);
  this->update_wifi_selection_subpage(subpage);
}


/**
 * @brief Callback function in case a wifi is selected. This callback function will change the label of the wifi password
 * sub page to the selected wifi network. 
 * 
 * @param e Pointer to event object for the event where this callback is called
 */
static void wifi_selected_cb(lv_event_t* e)
{
  lv_obj_t* label = lv_obj_get_child(e->target, 0);
  lv_label_set_text((lv_obj_t*) e->user_data, lv_label_get_text(label));
  ssid = lv_label_get_text(label);
}


void OmoteUI::update_wifi_selection_subpage(int page)
{
  if (page < this->no_subpages)
  {
    lv_obj_clean(this->wifi_setting_cont);

    lv_obj_t* pageLabel = lv_label_create(this->wifi_setting_cont);
    lv_label_set_text_fmt(pageLabel, "Page %d/%d", page + 1, this->no_subpages);
    if (page > 0)
    {
      lv_obj_t* menuBox = lv_obj_create(this->wifi_setting_cont);
      lv_obj_set_size(menuBox, lv_pct(100), 45);
      lv_obj_set_scrollbar_mode(menuBox, LV_SCROLLBAR_MODE_OFF);

      lv_obj_t* menuLabel = lv_label_create(menuBox);
      lv_label_set_text(menuLabel, "Previous");
      lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
      lv_obj_add_event_cb(menuBox, [](lv_event_t* e) {mInstance->next_wifi_selection_subpage(e);},LV_EVENT_CLICKED, (void*)(page - 1));
      lv_obj_t* arrow = lv_label_create(menuBox);
      lv_label_set_text(arrow, LV_SYMBOL_LEFT);
      lv_obj_align(arrow, LV_ALIGN_TOP_LEFT, 0, 0);
    }

    for (int i = 0; i < WIFI_SUBPAGE_SIZE && (page*WIFI_SUBPAGE_SIZE + i) < this->no_wifi_networks; i++)
    {
      lv_obj_t* menuBox = lv_obj_create(this->wifi_setting_cont);
      lv_obj_set_size(menuBox, lv_pct(100), 45);
      lv_obj_set_scrollbar_mode(menuBox, LV_SCROLLBAR_MODE_OFF);

      lv_obj_add_flag(menuBox, LV_OBJ_FLAG_EVENT_BUBBLE);

      lv_obj_t* menuLabel = lv_label_create(menuBox);
      lv_label_set_text(menuLabel, this->found_wifi_networks->at(page*WIFI_SUBPAGE_SIZE + i).ssid.c_str());
      lv_obj_t* wifi_image;

      int RSSI = this->found_wifi_networks->at(page*WIFI_SUBPAGE_SIZE + i).rssi;

      if (RSSI > -50)
      {
        wifi_image = imgs.addWifiHighSignal(menuBox);
      }
      else if (RSSI > -60)
      {
        wifi_image = imgs.addWifiMidSignal(menuBox);
      }
      else if (RSSI > -70)
      {
        wifi_image = imgs.addWifiLowSignal(menuBox);
      }
      else
      {
        wifi_image = imgs.addWifiLowSignal(menuBox);
      }
      lv_obj_align(wifi_image, LV_ALIGN_TOP_RIGHT, 0, 0);
      lv_menu_set_load_page_event(this->settingsMenu, menuBox, this->wifi_password_page);
      lv_obj_add_event_cb(menuBox, wifi_selected_cb, LV_EVENT_CLICKED, this->wifi_password_label);
      }

    if ((page + 1) < this->no_subpages)
    {
      lv_obj_t* menuBox = lv_obj_create(this->wifi_setting_cont);
      lv_obj_set_size(menuBox, lv_pct(100), 45);
      lv_obj_set_scrollbar_mode(menuBox, LV_SCROLLBAR_MODE_OFF);

      lv_obj_t* menuLabel = lv_label_create(menuBox);
      lv_label_set_text(menuLabel, "Next");
      lv_obj_add_event_cb(menuBox, [](lv_event_t* e) {mInstance->next_wifi_selection_subpage(e);}, LV_EVENT_CLICKED, (void*)(page + 1));

      lv_obj_t* arrow = lv_label_create(menuBox);
      lv_label_set_text(arrow, LV_SYMBOL_RIGHT);
      lv_obj_align(arrow, LV_ALIGN_TOP_RIGHT, 0, 0);

    }
    lv_obj_scroll_to_y(this->wifi_setting_cont, 0, LV_ANIM_OFF);
  }
}

void OmoteUI::create_wifi_settings(lv_obj_t* menu, lv_obj_t* parent)
{
  this->wifi_selection_page = this->create_wifi_selection_page(menu);
  this->wifi_password_page = this->create_wifi_password_page(this->settingsMenu);
  this->create_wifi_main_page(parent);
  this->mHardware->wifi_scan_done.onNotify([this] (std::shared_ptr<std::vector<WifiInfo>> info) {this->wifi_scan_done(info);});
  this->mHardware->wifi_status_update.onNotify([this] (std::shared_ptr<wifiStatus> status) {this->wifi_status(status);});
}

void OmoteUI::wifi_status(std::shared_ptr<wifiStatus> status)
{
  this->mHardware->debugPrint("connected %d\n", status->isConnected);
  this->mHardware->debugPrint("IP %s\n", status->IP);
  this->mHardware->debugPrint("SSID %s\n", status->ssid);

  lv_obj_t* ip_label = lv_obj_get_child(this->wifiOverview, 3);
  lv_obj_t* ssid_label = lv_obj_get_child(this->wifiOverview, 0);

  if (status->isConnected)
  {
    lv_label_set_text(this->WifiLabel, LV_SYMBOL_WIFI);
    lv_label_set_text(ssid_label, status->ssid.c_str());
    lv_label_set_text(ip_label, status->IP.c_str());
  }
  else
  {
    lv_label_set_text(this->WifiLabel, "");
    lv_label_set_text(ssid_label, "Disconnected");
    lv_label_set_text(ip_label, "-");
  }
}

lv_obj_t* OmoteUI::create_wifi_password_page(lv_obj_t* menu)
{
  lv_obj_t* ret_val = lv_menu_page_create(menu, NULL);
  lv_obj_t* cont = lv_menu_cont_create(ret_val);
  lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_ACTIVE);

  this->wifi_password_label = lv_label_create(cont);
  lv_label_set_text(this->wifi_password_label, "Password");
  lv_obj_t* password_input = lv_textarea_create(cont);
  lv_obj_set_width(password_input, lv_pct(100));
  lv_textarea_set_password_mode(password_input, true);
  lv_textarea_set_one_line(password_input, true);
  lv_textarea_set_placeholder_text(password_input, "Password");
  lv_obj_add_event_cb(password_input, [] (lv_event_t* e) {mInstance->password_field_event_cb(e);} , LV_EVENT_READY, NULL );
  this->attach_keyboard(password_input);

  lv_obj_t* show_password = lv_checkbox_create(cont);
  lv_checkbox_set_text(show_password, "Show password");
  lv_obj_add_event_cb(show_password, show_password_cb, LV_EVENT_VALUE_CHANGED, password_input);

  lv_obj_t* connect_button = lv_btn_create(cont);
  lv_obj_t* label = lv_label_create(connect_button);
  lv_label_set_text(label, "Connect");
  lv_obj_add_event_cb(connect_button,[] (lv_event_t* e) { mInstance->connect_btn_cb(e);}, LV_EVENT_CLICKED, password_input);

  return ret_val;
}

/**
 * @brief Callback which is triggered when the wifi settings are opened (the wifi settings are pressed in the settings
 * main page). This function will trigger the asynchronous scan for wifi networks and update the label of the wifi 
 * selection page to indicate that wifi networks are being searched for. The wifi event callback function then has to 
 * call the API function to fill the page with the found networks.
 * 
 * @param event Pointer to event object for the event where this callback is called
 */
void OmoteUI::wifi_settings_cb(lv_event_t* event)
{
  lv_obj_t* cont = (lv_obj_t*) lv_event_get_user_data(event);
  lv_obj_clean(cont);
  lv_obj_t* label = lv_label_create(cont);
  lv_label_set_text(label, "Searching for wifi networks");
  mHardware->debugPrint("Wifi settings cb called\n");
  mHardware->wifi_scan_start.notify();
  //This will trigger an asynchronouse network scan
  // We need to trigger wifi search via HAL
  //wifihandler.scan();
}