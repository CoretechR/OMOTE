#pragma once

#include <string>

static const auto HomeAssistAuthResponse = R"---(
    {
      "type": "auth",
      "access_token": ")---" + std::string(HOMEASSISTANT_API_TOKEN) +
                                           "\"}";

static const auto TestSubEvent = R"---(
    {
      "id": 5,
      "type": "subscribe_trigger",
      "trigger": {
          "platform": "state",
          "entity_id": "light.overhead_lights",
          "from": "on", 
          "to": "off"
      }
    }
    )---";

static const auto TestSubAllEvent = R"---(
    {
      "id": 1,
      "type": "subscribe_events",
      "event_type": "state_changed"
    }
    )---";

static const auto GetConfigMessage = R"---(
    {
      "id": 1,
      "type": "get_config"
    }
    )---";

static const auto ConfigAreaRegistryList = R"---(
    {
      "id": 4,
      "type": "config/area_registry/list"
    }
    )---";