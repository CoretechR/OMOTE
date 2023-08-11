
#include "display.hpp"

Display* Display::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Display();
    }

    return instance;
}

void Display::setup(int backlight_pin, int enable_pin)
{
    this->enable_pin = enable_pin;
    this->backlight_pin = backlight_pin;
    pinMode(this->enable_pin, OUTPUT);
    digitalWrite(this->enable_pin, HIGH);
    pinMode(this->backlight_pin, OUTPUT);
    digitalWrite(this->backlight_pin, HIGH);

    this->tft = TFT_eSPI();

    ledcSetup(LCD_BACKLIGHT_LEDC_CHANNEL, LCD_BACKLIGHT_LEDC_FREQUENCY, LCD_BACKLIGHT_LEDC_BIT_RESOLUTION);
    ledcAttachPin(this->backlight_pin, LCD_BACKLIGHT_LEDC_CHANNEL);
    ledcWrite(LCD_BACKLIGHT_LEDC_CHANNEL, 0);

    this->tft.init();
    this->tft.initDMA();
    this->tft.setRotation(0);
    this->tft.fillScreen(TFT_BLACK);
    this->tft.setSwapBytes(true);

    // Slowly charge the VSW voltage to prevent a brownout
    // Workaround for hardware rev 1!
    for(int i = 0; i < 100; i++){
        digitalWrite(this->enable_pin, HIGH);  // LCD Logic off
        delayMicroseconds(1);
        digitalWrite(this->enable_pin, LOW);  // LCD Logic on
    }  

}

void Display::pushPixel(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t* pixel_values)
{
    this->tft.startWrite();
    this->tft.setAddrWindow( x, y, w, h );
    this->tft.pushPixelsDMA(pixel_values, w * h);
    this->tft.endWrite();    
}

void Display::setBrightness(uint8_t brigthness)
{
    ledcWrite(LCD_BACKLIGHT_LEDC_CHANNEL, brigthness);
}

void Display::turnOff()
{
    digitalWrite(this->backlight_pin, HIGH);
    digitalWrite(this->enable_pin, HIGH);
    pinMode(this->backlight_pin, INPUT);
    pinMode(this->enable_pin, INPUT);
    gpio_hold_en((gpio_num_t) this->backlight_pin);
    gpio_hold_en((gpio_num_t) this->enable_pin);
}

