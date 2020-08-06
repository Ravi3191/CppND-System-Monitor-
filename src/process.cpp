#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


int Process::Pid() { return pid; }


float Process::CpuUtilization() { return LinuxParser::Ram(pid); }


string Process::Command() { return LinuxParser::Command(pid); }


string Process::Ram() { return LinuxParser::Memory(pid); }


string Process::User() { return LinuxParser::User(pid); }


long int Process::UpTime() { return LinuxParser::UpTime(pid); }


bool Process::operator<(Process const& a) const {
    
     return ( util < a.util);
     }