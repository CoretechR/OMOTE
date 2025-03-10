# HomeAssistant Integration

This folder contains the integration layer between OMOTE and Home Assistant, providing both REST API and WebSocket API support.

## Directory Structure

```
HomeAssistant/
├── Api/
│    ├── Rest/           # REST API implementation  
│    ├── WebSocket/      # WebSocket API implementation
└── RapidJsonUtility.cpp # JSON parsing utilities
```

## Key Components

### REST API
- `IHomeAssistApi`: Interface defining core API operations
- `Light`: Entity implementation for controlling HA light devices
- `HomeAssistApiParser`: JSON response parser using RapidJSON

### WebSocket API
- Provides real-time communication with Home Assistant
- Supports chunked message processing
- Handles device state updates and queries

## Configuration

Home Assistant connection settings are defined in `platformio.ini`:
- `HOMEASSISTANT_API_TOKEN`
- `HOMEASSISTANT_IP_ADDRESS`
- `HOMEASSISTANT_PORT`
- Optional: `HOMEASSISTANT_URL`

## Usage

The integration supports:
- Entity state queries and updates
- Real-time device state monitoring
- Light control (brightness, on/off)
- Device discovery and enumeration

Example WebSocket usage:
```cpp
auto api = std::make_unique<HomeAssist::WebSocket::Api>();
auto messageHandler = std::make_shared<HomeAssist::WebSocket::MessageHandle>();
```