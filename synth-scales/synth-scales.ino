#include "M5Cardputer.h"
#include "tones.h"
#include "utils.h"

const char keys[OCT] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const String key_names[11] = {"C", "C#", "D", "D#", "E", "F", "G", "G#", "A", "A#", "B"};
const int scale_major[OCT] = {0, 2, 2, 1, 2, 2, 2, 1};
const int scale_minor[OCT] = {0, 2, 1, 2, 2, 1, 2, 2};
const int* current_scale = scale_major;
int key = 0; // C4

void draw_title() {
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.drawString("Scales", M5Cardputer.Display.width() / 2, M5Cardputer.Display.height() / 2);
}

void draw_key() {
    M5Cardputer.Display.setTextColor(YELLOW);
    M5Cardputer.Display.setTextDatum(top_left);
    M5Cardputer.Display.drawString(key_names[wrapi(key, 0, sizeof(key_names)/sizeof(*key_names))]+(current_scale == scale_minor ? "m" : ""), 5, 5);
}

void set_key(int const k) {
    key = k;
    M5Cardputer.Display.clear();
    draw_title();
    draw_key();
    M5Cardputer.Speaker.tone(660, 50, 1);
    delay(150);
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Speaker.setVolume(255);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setFont(&fonts::Orbitron_Light_32);
    M5Cardputer.Display.setTextSize(1);
    draw_title();
    draw_key();
}

void loop() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            M5Cardputer.Speaker.tone(660, 50, 1);
            M5Cardputer.Speaker.setVolume(min(M5Cardputer.Speaker.getVolume()+10, 255));
        }
        else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            M5Cardputer.Speaker.tone(550, 50, 1);
            M5Cardputer.Speaker.setVolume(max(M5Cardputer.Speaker.getVolume()-10, 0));
        }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            set_key(key+1);
        }
        else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            set_key(key-1);
        }
        else if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            set_key(0);
        }
        else if (M5Cardputer.Keyboard.isKeyPressed('m')) {
            if (current_scale == scale_major) {
              current_scale = scale_minor;
            } else {
              current_scale = scale_major;
            }
            set_key(key);
        }
      
        bool key_pressed = false;
        for (int i=0, sum=0; i < OCT; ++i) {
            sum += current_scale[i];
            if (M5Cardputer.Keyboard.isKeyPressed(keys[i])) {
                M5Cardputer.Speaker.tone(semitone(sum+key), 100);
                key_pressed = true;
            }
        }
        if (key_pressed) {
          delay(150);
        }
    }
}
