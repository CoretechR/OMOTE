#pragma once
#include "Notification.hpp"

#include <memory>
class KeyPressAbstract {
public:
  // Keys from Top Down left to right.
  enum class KeyId {
    Power,
    // Top 4 Buttons left to right
    Stop,
    Rewind,
    Play,
    FastForward,
    // Buttons around D Pad
    Menu,
    Info,
    Back,
    Source,
    // D Pad
    Up,
    Down,
    Left,
    Right,
    Center,
    // Volume Channel and 2 between
    VolUp,
    VolDown,
    Mute,
    Record,
    ChannelUp,
    ChannelDown,
    // Bottom 4 buttons left to right
    Aux1,
    Aux2,
    Aux3,
    Aux4,
    INVALID
  };

  class KeyEvent {
  public:
    enum class Type { Press, Release, INVALID };

    KeyEvent() = default;
    KeyEvent(const KeyId aId, const Type aType) : mId(aId), mType(aType) {}

    KeyId mId = KeyId::INVALID;
    Type mType = Type::INVALID;
  };

  KeyPressAbstract();

  /// @brief Register a SINGLE handler to be used for proccessing keys
  /// @param aKeyEventHandler - Callable the Handles KeyEvent
  void RegisterKeyPressHandler(std::function<bool(KeyEvent)> aKeyEventHandler);

protected:
  /// @brief Function ment to be called regularly to allow
  ///        proccesssing of key presses by calling mKeyEventHandler
  ///        best case this is done on a seprate thread/task
  ///        since it could take a while to handle a KeyPress
  virtual void HandleKeyPresses() = 0;

  /// @brief Function to queue up Key events to be handled later on by
  ///        HandleKeyPresses() hopefully on a seprate thread or task
  ///        This function should be implemented in a way that makes it ISR
  ///        safe
  /// @param aJustOccuredKeyEvent - A Key even that just occured
  virtual void QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) = 0;

  std::function<bool(KeyEvent)> mKeyEventHandler;
};