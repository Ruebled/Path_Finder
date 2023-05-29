#include "TimeTracker.h"

// Global variables performance report
// variable cpu_time 
float cpu_time = 0;

// cpu_time variable text
float real_time = 0;

TimeTracker::TimeTracker(){

}

void TimeTracker::rlt_start(){
	rlt_start_t = std::chrono::steady_clock::now();
	return;
}

void TimeTracker::rlt_pause(){
	rlt_end_t = std::chrono::steady_clock::now();
	rlt_elapsed_t += rlt_end_t - rlt_start_t;
}

void TimeTracker::rlt_stop(){
	rlt_int_t = std::chrono::floor<std::chrono::milliseconds>(rlt_elapsed_t).count();
	real_time = (rlt_int_t)/1000 + ((float) (rlt_int_t%1000))/1000;
}

void TimeTracker::cpu_start(){
	cpu_start_t = std::chrono::steady_clock::now();
	return;
}

void TimeTracker::cpu_pause(){
	cpu_end_t = std::chrono::steady_clock::now();
	cpu_elapsed_t += cpu_end_t - cpu_start_t;
}

void TimeTracker::cpu_stop(){
	cpu_int_t = std::chrono::floor<std::chrono::nanoseconds>(cpu_elapsed_t).count();
	cpu_time = (cpu_int_t)/1000000 + ((float) (cpu_int_t%1000000))/1000000;
}
