#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float delta_active, delta_total;
	float total_jiffies_start, active_jiffies_start, total_jiffies_end, active_jiffies_end;
	total_jiffies_start = LinuxParser::Jiffies();
	active_jiffies_start = LinuxParser::ActiveJiffies();
	// Wait 100ms
	usleep(100000); // in microseconds

	total_jiffies_end = LinuxParser::Jiffies();
	active_jiffies_end = LinuxParser::ActiveJiffies();

	delta_total = total_jiffies_end - total_jiffies_start;
	delta_active = active_jiffies_end - active_jiffies_start;

	return delta_active / delta_total;}