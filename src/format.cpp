#include <string>

#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) {
     long hours = seconds/3600;
     long minutes = (seconds%3600)/60;
     long remaining_seconds = seconds - hours*3600 - minutes*60;
     string shours,sminutes,sseconds;
     if (hours<10) shours = "0" + std::to_string(hours);
     else shours = std::to_string(hours);
     if (minutes<10) sminutes = "0" + std::to_string(minutes);
     else sminutes = std::to_string(minutes);
     if (remaining_seconds<10) sseconds = "0" + std::to_string(remaining_seconds);
     else sseconds = std::to_string(remaining_seconds);
     string formated_time = shours + ":" + sminutes + ":" + sseconds;
     return formated_time; 
     }