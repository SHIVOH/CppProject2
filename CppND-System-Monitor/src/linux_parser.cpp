#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key;
  float MemT, MemF;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key=="MemT:") MemT=value;
      else if(key=="MemF:") MemF=value;
    }
    return float((MemT-MemF)/MemT);
  }
  return 0;  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long jiffies = ActiveJiffies() + IdleJiffies();
  return jiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid){
   string pidstr = to_string(pid);
  string value; 
  vector<string> jiffies{};
  long active_jiffies=0;
  string line, key;
  std::ifstream filstream(kProcDirectory + pidstr + kStatFilename);
  if (filstream.is_open())
  {
    getline(filstream,line);
    std::istringstream linestream(line);
    for (int i = 1; i <= kStart; i++) 
    {
      linestream >> value;
      if (i == kU || i == kS || i == kCu || i == kCs ) 
      {
      // read in clock ticks and convert to seconds by dividing by clock ticks per second
        active_jiffies += std::stof(value); 
      }
       }
    
  
  }
  return active_jiffies; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long juser, jnice, jsystem, jidle, jiowait, jirq, jsoftirq, jsteal, jguest, jguestnice;
  string line;
  string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> juser >> jnice >> jsystem >> jidle >> jiowait >> jirq >> jsoftirq >> jsteal >> jguest >> jguestnice;
      if (key=="cpu"){
        return (juser+jnice+jsystem+jirq+jsoftirq+jsteal);
      }
    }
  }
  return 0;}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, cpu_string;
  long value, idle_jiffies{0};
  vector<int> cpu_data{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
   std::getline (filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu_string;
    while (linestream >> value) {
     cpu_data.push_back(value); 
    }
  }
  idle_jiffies = cpu_data.at(CPUStates::kIdle_) + cpu_data.at(CPUStates::kIOwait_);
  
  return idle_jiffies;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuValues{};
  string line;
  string key;
  string vuser, vnice, vsystem, vidle, viowait, virq, vsoftirq, vsteal, vguest, vguest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
  	while(std::getline(filestream, line)){
    	std::istringstream linestream(line);
      	while(linestream >> key >> vuser >> vnice >> vsystem >> vidle >> viowait >> virq >> vsoftirq >> vsteal >> vguest >> vguest_nice){
        	if(key == "cpu"){
            	cpuValues.push_back(vuser);
              	cpuValues.push_back(vnice);
              	cpuValues.push_back(vsystem);
              	cpuValues.push_back(vidle);
              	cpuValues.push_back(viowait);
              	cpuValues.push_back(virq);
              	cpuValues.push_back(vsoftirq);
              	cpuValues.push_back(vsteal);
              	cpuValues.push_back(vguest);
              	cpuValues.push_back(vguest_nice);
              	return cpuValues;
            }
        }
    }
  }
  return {};
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
        break;
    }
  }
  return value;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int processes_running;
  string line, key;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> processes_running;
      if(key=="procs_running"){
        return processes_running;
      }
      else
        continue;
    }
    return 0;
  }
  else
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string pidstr= to_string(pid);
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + pidstr + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line; 
    }
   return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, vmSize;
  long int value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          vmSize = std::to_string(value / 1000);
        }
      }
    }
  }
  return vmSize;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
  	while(std::getline(filestream, line)){
    	std::replace(line.begin(), line.end(), ':',' ');
      	std::istringstream linestream(line);
      	while(linestream >> key >> value){
        	if(key == "Uid"){
            	return value;
            }
        }
    }
  }
  
  
  return value; 
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, data, user;
  string uid = LinuxParser::Uid(pid);
  if(uid=="") return "None";
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      int i=0;
      while(std::getline(linestream, data, ':')){
        if(i==0) {
          user=data;
          i++;
          continue;
        }
        if(uid==data){
          return user;
          }
          else if(i==2){
            break;
          }
          else {
            i++;
          }
      }
    }
    return "None";
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  int upTime{0}, processStartTime;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream >> value;
    }
    linestream >> processStartTime;
    upTime = LinuxParser::UpTime() - processStartTime / sysconf(_SC_CLK_TCK);
    return upTime;
  }
  return upTime;

 }
