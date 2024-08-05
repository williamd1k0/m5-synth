#include <set>
#include <map>
#include <BLEMidi.h>
#include "M5Cardputer.h"
#include "tones.h"

std::map<uint8_t, int> keys;
std::set<int> channels = { 1, 2, 3, 4, 5, 6, 7, 8 };
const uint32_t release = 0;
const uint8_t* tone_wave = sine_wave;
M5Canvas cv(&M5Cardputer.Display);

void on_note_on(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  if (channels.empty()) return;
  auto it = channels.begin();
  int chan = *it;
  channels.erase(it);
  keys[note] = chan;
  draw_keys();
}

void on_note_off(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  if (keys[note] == 0) return;
  channels.insert(keys[note]);
  if (release < 100) {
      M5Cardputer.Speaker.stop(keys[note]-1);
  } else {
      M5Cardputer.Speaker.tone(semitone_midi(note), release, keys[note]-1);
  }
  keys[note] = 0;
  draw_keys();
}

void draw_keys() {
    cv.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height() / 2 + 6);
    cv.fillSprite(0x7BEF);
    const int wH = M5Cardputer.Display.height() / 2;
    const int bH = M5Cardputer.Display.height() / 3;
    const int s = 1;
    const int w = 10;
    const int b = 5;
    const int o = (M5Cardputer.Display.width() - (1+(s+w)*7)*3) / 2 + 1;
    int oct = 1+(s+w)*7;
    for (int i=0; i < 3; ++i) {
        cv.fillRect(oct*i+o+(s+w)*0, 3, w, wH, keys[48+12*i] == 0 ? WHITE : RED);
        cv.fillRect(oct*i+o+(s+w)*1, 3, w, wH, keys[50+12*i] == 0 ? WHITE : YELLOW);
        cv.fillRect(oct*i+o+(s+w)*2, 3, w, wH, keys[52+12*i] == 0 ? WHITE : GREEN);
        cv.fillRect(oct*i+o+(s+w)*3, 3, w, wH, keys[53+12*i] == 0 ? WHITE : 0x07F9);
        cv.fillRect(oct*i+o+(s+w)*4, 3, w, wH, keys[55+12*i] == 0 ? WHITE : BLUE);
        cv.fillRect(oct*i+o+(s+w)*5, 3, w, wH, keys[57+12*i] == 0 ? WHITE : VIOLET);
        cv.fillRect(oct*i+o+(s+w)*6, 3, w, wH, keys[59+12*i] == 0 ? WHITE : MAGENTA);
        cv.fillRect(oct*i+o+w*1-b/2, 3, b, bH, keys[49+12*i] == 0 ? BLACK : ORANGE);
        cv.fillRect(oct*i+o+w*2+s*1-b/2, 3, b, bH, keys[51+12*i] == 0 ? BLACK : GREENYELLOW);
        cv.fillRect(oct*i+o+w*4+s*3-b/2, 3, b, bH, keys[54+12*i] == 0 ? BLACK : CYAN);
        cv.fillRect(oct*i+o+w*5+s*4-b/2, 3, b, bH, keys[56+12*i] == 0 ? BLACK : PURPLE);
        cv.fillRect(oct*i+o+w*6+s*5-b/2, 3, b, bH, keys[58+12*i] == 0 ? BLACK : PINK);
    }
    M5Cardputer.Display.startWrite(); 
    cv.pushSprite(0, M5Cardputer.Display.height() / 2 - 6);
    M5Cardputer.Display.endWrite();
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, false);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setFont(&fonts::Orbitron_Light_32);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.drawString("Midi", M5Cardputer.Display.width() / 2, M5Cardputer.Display.height() / 4);
    M5Cardputer.Speaker.setVolume(255);
    Serial.begin(115200);
    BLEMidiClient.begin("Cardputer");
    draw_keys();
}

void loop() {
    M5Cardputer.update();
    if (M5Cardputer.BtnA.wasPressed()) {
        if (tone_wave == sine_wave) {
          tone_wave = square_wave;
        } else {
          tone_wave = sine_wave;
        }
    }
    if(!BLEMidiClient.isConnected()) {
        int nDevices = BLEMidiClient.scan();
        if(nDevices > 0) {
            if(BLEMidiClient.connect(0)) {
                BLEMidiClient.setNoteOnCallback(on_note_on);
                BLEMidiClient.setNoteOffCallback(on_note_off);
            }
            else {
                Serial.println("Connection failed");
                delay(3000);
            }
        }
    } else {
        for (const auto& keyval : keys) {
            if (keyval.second != 0) {
                if (M5Cardputer.Speaker.isPlaying(keyval.second-1)) continue;
                M5Cardputer.Speaker.tone(semitone_midi(keyval.first), UINT32_MAX, keyval.second-1, true, tone_wave, 16);
            }
        }
    }
}