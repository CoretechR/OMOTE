#include "OmoteSetup.hpp"

int main() {
  OMOTE::setup();
  while (true) {
    OMOTE::loop();
  }
}