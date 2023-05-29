#ifndef TIMETRACKER_H
#define TIMETRACKER_H
#include <chrono>

class TimeTracker {
	private:
		std::chrono::time_point<std::chrono::steady_clock> rlt_start_t;
		std::chrono::time_point<std::chrono::steady_clock> rlt_end_t; 
		std::chrono::duration<double> rlt_elapsed_t;
		int rlt_int_t;

		std::chrono::time_point<std::chrono::steady_clock> cpu_start_t;
		std::chrono::time_point<std::chrono::steady_clock> cpu_end_t; 
		std::chrono::duration<double> cpu_elapsed_t;
		int cpu_int_t;

	public:
		TimeTracker();

		void rlt_start();
		void rlt_pause();
		void rlt_stop();

		void cpu_start();
		void cpu_pause();
		void cpu_stop();

};

#endif
