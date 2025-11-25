#ifndef SERIAL_PARSER_H
#define SERIAL_PARSER_H

  #include <Arduino.h>

  extern String line;

  void print_help();
  void handle_command(const String& cmd_in);
  void plot_line(float ref_rpm, float speed_rpm, float pos_deg);
  String up(String s);

#endif//SERIAL_PARSER_H
