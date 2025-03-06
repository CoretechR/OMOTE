#pragma once

#include <string>

namespace HomeAssist {

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
          "attribute": "brightness"
      }
    }
    )---";

static const auto GetEntityMessage = R"---(
    {
      "id": 0,
      "type": "config/entity_registry/list_for_display"
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

}  // namespace HomeAssist