#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
using namespace LinuxParser;

using std::stof;
using std::string;
using std::to_string;
using std::vector;

]
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

]
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}


vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() {
    string line,name,memtype;
    float util,mem_free{0.0},mem_total{0.0};
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()){
      while(std::getline(stream,line)){
        std::replace(line.begin(),line.end(),':',' ');
        std::istringstream linestream(line);
        linestream >> name >> util >> memtype; 
        if(name == "MemTotal") mem_total = util/1000;
        if(name == "MemFree")  mem_free = util/1000;
      }
    }
   float mem_util = (mem_total-mem_free)/mem_total ;
   return mem_util;
}


long LinuxParser::UpTime() { 
  long uptime,idletime;
  string line,name;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  
  return uptime/sysconf(_SC_CLK_TCK);
   }
  
  
  long LinuxParser::Jiffies() { 
  long jiffies{0},temp;
  string line,name;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
        std::istringstream linestream(line);
        linestream >> name ;
        if(name == "cpu") {
          while(linestream){
            linestream >> temp;
            jiffies += temp; 
          }
           break;
        }
    }

  }
  return jiffies;
  }




long LinuxParser::ActiveJiffies(int pid) {
   long jiffies{0},temp, number;
   string a,b;
   int iter{0};
   string line, name;
   std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
   if(stream.is_open()){
     std::getline(stream,line);
     std::istringstream linestream(line);

     linestream >> number >> a >> b;
     while(linestream){
       linestream >> temp;
        
       if(iter == 10 ||iter == 11 ||iter == 12 ||iter == 13){
          jiffies += temp;
       }
        iter++;
     }
   }
   return jiffies;
}


long LinuxParser::ActiveJiffies() {
     long jiffies{0},jiffies1,jiffies2,jiffies3;
  string line,name;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
        std::istringstream linestream(line);
        linestream >> name >> jiffies1 >> jiffies2 >>jiffies3;
        if(name == "cpu") {
          jiffies = jiffies1 + jiffies2 + jiffies3; 
           break;
        }
    }
  }
  return jiffies;
}




long LinuxParser::IdleJiffies() {
  long jiffies1,jiffies2,jiffies3, jiffies4;
  string line,name;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
        std::istringstream linestream(line);
        linestream >> name >> jiffies1 >> jiffies2 >>jiffies3 >> jiffies4;
        if(name == "cpu") {
          return jiffies4; 
          break;
        }
    }

  }
  return jiffies4; 
}




vector<long> LinuxParser::CpuUtilization() {
  vector<long> Utilization{};
  string line,name;
  long temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> name;
    while(linestream){
         linestream >> temp;
         Utilization.push_back(temp);
    }
  }
  Utilization.pop_back();
  return Utilization; 
  }

int LinuxParser::TotalProcesses() {
  int processes{0};
  string line,name;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream >> name;
      if(name == "processes"){
        linestream >> processes;
        return processes;
      }
    }
  }
   return processes;
}


int LinuxParser::RunningProcesses() { 
  int procs_running{0};
  string line,name;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream >> name;
      if(name == "procs_running"){
        linestream >> procs_running;
        return procs_running;
      }
    }
  }
   return procs_running;
  }


string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream,line);
  }
  return line;
}


string LinuxParser::Memory(int pid) {
  long number;
   string line, name;
   std::ifstream stream(kProcDirectory + to_string(pid) +kStatusFilename);
   if(stream.is_open()){
     while(std::getline(stream,line)){
       std::istringstream linestream(line);
       linestream >> name >> number;
       if(name == "VmSize:"){
         return to_string(float(float(number)/1000));
       }
     }
   }
    return to_string(0.0);
   }

float LinuxParser::Ram(int pid) {
   long totaltime = LinuxParser::ActiveJiffies(pid);
   long seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
   float num = float(float(totaltime)/sysconf(_SC_CLK_TCK));
   float cpu_usage = num/float(seconds);
   return cpu_usage/100.0;
   }


string LinuxParser::Uid(int pid) { 
  string line,name;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatusFilename);
  if(stream.is_open()){
        while(std::getline(stream,line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> name;
        if(name == "Uid") {
          linestream >> name;
          return name;
        }
    }
  }

  return name; 
  }


string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line,name,password,id;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
        while(std::getline(stream,line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> name >> password >> id;
        if(uid == id){
            return name;
        }  
    }
  }
  return name; 
  }


long LinuxParser::UpTime(int pid) {
   long temp{0}, number;
   int iter{0};
   string line, name, a,b;
   std::ifstream stream(kProcDirectory + to_string(pid) +kStatFilename);
   if(stream.is_open()){
     std::getline(stream,line);
     std::istringstream linestream(line);

     linestream >> number >> a >> b;
     while(linestream){
       linestream >> temp;
       if(iter == 18){
          return temp/sysconf(_SC_CLK_TCK);
       }
        iter++;
     }
   }
   return temp/sysconf(_SC_CLK_TCK);
}