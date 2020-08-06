#include "processor.h"
#include "linux_parser.h"
#include <chrono>
#include <thread>
#include <vector>
using namespace std::this_thread;
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.


float Processor::Utilization() { 
    std::vector<long> process_times_prev = LinuxParser::CpuUtilization();
    sleep_for(100ms);
    std::vector<long> process_times_curr = LinuxParser::CpuUtilization();
    long PrevIdle = process_times_prev[3] + process_times_prev[4]; 
    long Idle = process_times_curr[3] + process_times_curr[4];
    long PrevNonIdle = process_times_prev[0] + process_times_prev[1] + process_times_prev[2] + process_times_prev[5] + process_times_prev[6] + process_times_prev[7];
    long NonIdle = process_times_curr[0] + process_times_curr[1] + process_times_curr[2] + process_times_curr[5] + process_times_curr[6] + process_times_curr[7];
    long total = PrevIdle + PrevNonIdle;
    long prev_total = Idle + NonIdle;
    long totald = total - prev_total;
    long idled = Idle - PrevIdle;
    float percentage = (float(totald - idled)/float(totald))/100.0 ;
    return percentage; 
    }