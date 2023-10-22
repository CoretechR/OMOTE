#include "KeyPressAbstract.hpp"
#include "SDL2/SDL.h"

#include <map>
#include <mutex>
#include <queue>
#include <thread>

class KeyPressSim : public KeyPressAbstract {
public:
  static constexpr auto MaxQueueableKeyEvents = 3;

  KeyPressSim();

  static int GrabKeyImpl(void *aSelf, SDL_Event *aEvent);
  void GrabKeys(SDL_Event *aEvent);

  void HandleKeyPresses() override;
  void QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) override;

private:
  std::thread mKeyGrabberThread;
  std::thread mKeyHandlerThread;
  std::queue<KeyEvent> mKeyEventQueue;
  std::mutex mQueueGaurd;

  using Key = KeyPressAbstract::KeyId;
  static inline const std::map<SDL_Keycode, KeyPressAbstract::KeyId> KeyMap{
      {SDLK_p, Key::Power},
      // Video Control row
      {SDLK_5, Key::Stop},
      {SDLK_6, Key::Rewind},
      {SDLK_7, Key::Play},
      {SDLK_8, Key::FastForward},
      // D-Pad
      {SDLK_UP, Key::Up},
      {SDLK_DOWN, Key::Down},
      {SDLK_LEFT, Key::Left},
      {SDLK_RIGHT, Key::Right},
      {SDLK_SPACE, Key::Center},
      // General Keys
      {SDLK_h, Key::Menu},
      {SDLK_j, Key::Info},
      {SDLK_l, Key::Back},
      {SDLK_k, Key::Source},
      // Volume/ Channel UP and down
      {SDLK_w, Key::VolUp},
      {SDLK_s, Key::VolDown},
      {SDLK_e, Key::ChannelUp},
      {SDLK_d, Key::ChannelDown},
      // Mute & record
      {SDLK_x, Key::Mute},
      {SDLK_c, Key::Record},
      // Aux Keys
      {SDLK_1, Key::Aux1},
      {SDLK_2, Key::Aux2},
      {SDLK_3, Key::Aux3},
      {SDLK_4, Key::Aux4}};
};