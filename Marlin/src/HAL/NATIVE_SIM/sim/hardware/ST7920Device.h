#pragma once

#include <SDL2/SDL.h>

#include <list>
#include <deque>
#include "Gpio.h"

class ST7920Device: public Peripheral {
public:
  enum KeyName {
    ARROW_UP, ARROW_DOWN, COUNT
  };

  struct Command {
    Command(uint8_t rw, uint8_t rs, uint8_t data) : rw(rw), rs(rs), data(data){};
    uint8_t rw = 0;
    uint8_t rs = 0;
    uint8_t data = 0;
  };

  ST7920Device(pin_type clk, pin_type mosi, pin_type cs, pin_type beeper, pin_type enc1, pin_type enc2, pin_type enc_but, pin_type kill);
  virtual ~ST7920Device();
  void process_command(Command cmd);
  void update();
  void interrupt(GpioEvent ev);

  pin_type clk_pin, mosi_pin, cs_pin, beeper_pin, enc1_pin, enc2_pin, enc_but_pin, kill_pin;

  bool extended_instruction_set = false;

  uint8_t incomming_byte = 0;
  uint8_t incomming_bit_count = 0;
  uint8_t incomming_byte_count = 0;
  uint8_t incomming_cmd[3] = {};
  std::deque<Command> cmd_in;

  static constexpr uint8_t display_ram_size = 64 * (16 / 8);
  uint8_t display_ram[display_ram_size] = {};  // 64 x 16bit
  uint16_t display_ram_index = 0;

  static constexpr uint16_t graphic_ram_size = 64 * (256 / 8);
  uint8_t graphic_ram[graphic_ram_size] = {}; // 64 x 256bit
  uint16_t graphic_ram_index = 0;
  uint16_t graphic_ram_index_x = 0, graphic_ram_index_y = 0;

  uint32_t address_counter = 0;
  int8_t address_increment = 1;

  uint8_t coordinate[2] = {};
  uint8_t coordinate_index = 0;


  /*** SDL ****/
  void window_create(float scaler);
  void process_event(SDL_Event& e);
  void window_destroy();
  void encoder_rotate_cw();
  void encoder_rotate_ccw();

  bool key_pressed[KeyName::COUNT] = {};

  bool close_request = false;
  bool dirty = true;
  std::chrono::high_resolution_clock clock;
  std::chrono::high_resolution_clock::time_point last_update;
  uint8_t encoder_position = 0;
  float scaler;

  SDL_Window* window = nullptr;
  SDL_Surface* screenSurface = nullptr;
};