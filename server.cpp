#ifndef SERVER_CPP
#define SERVER_CPP

#include <chrono>
#include <iostream>

#include "server.h"
#include "generator.h"

std::default_random_engine Server::generator(std::chrono::system_clock::now().time_since_epoch().count());
uint16_t Server::index = 0;

#if 0
uint64_t Server::GetServiceTime(Task * const task) {
       	const uint64_t retval = llround(service_time_distr(generator)) + (rate ? task->size / rate : 0);
       	svc_sum += retval;
       	return retval;
}
#endif

Server::Server(const std::string & p_name, const unsigned int & service_time, const size_t & p_rate): n_tasks(0), task_time(0), qd_sum(0), svc_sum(0),
       	sz_sum(0), rate(p_rate), my_index(index), name(p_name)	{
	       	index++;
       	}

void Server::Queue(Events & events, const uint64_t & t, Task * const task) {
	// std::cout << ">IO " << *io <<std::endl;
	// std::cout << ">IOQ " << ioq;
	assert(MAX_TASKQ > taskq.size());
	taskq.push_back(task);
	qd_sum += taskq.size();
	if (1 == taskq.size()) {
		const uint64_t service_time = GetServiceTime(task);
	       	ServerEvent * const e = new ServerEvent(t + service_time, EvTyEndTask, this);
	       	events.push_back(e);
	       	std::push_heap(events.begin(), events.end(), cmp);
	}
	// std::cout << ">>IOQ " << ioq;
}

void Server::UnQueue(Events & events, const uint64_t & t) {
	if (1 > taskq.size()) throw taskq;
	// std::cout << "<IOQ " << ioq;
	Task * const task = taskq.front();
	// std::cout << "IO start:" << task->t << " end:" << t << std::endl;
	task_time += t - task->t;
	n_tasks++;
       	task->generator->EndTask(t);
	sz_sum += task->size;
	delete task;
	taskq.pop_front();
	// std::cout << "<<IOQ " << ioq;
	if (0 < taskq.size()) {
	       	Task * const next_task = taskq.front();
		const uint64_t service_time = GetServiceTime(next_task);
	       	ServerEvent * const e = new ServerEvent(t + service_time, EvTyEndTask, this);
	       	events.push_back(e);
	       	std::push_heap(events.begin(), events.end(), cmp);
	}
}

Server::~Server() {
	std::cout << "SERVER " << name << " (" << my_index << ")" << std::endl;
	std::cout << "\tTasks " << n_tasks << std::endl;
	std::cout << "\tVolume " << sz_sum << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
}

SSD_PM1733a::SSD_PM1733a(const std::string & name, const uint64_t & p_t): Server(name, 0, 0),
       	read_service_time_distr(double(1)/(double)0.031), write_service_time_distr(double(1)/double(8)), current_time(p_t) {
}

uint64_t SSD_PM1733a::GetServiceTime(Task * const task) {
	const std::string taskname = task->Name();
	if (0 == taskname.compare(std::string("READ"))) {
	       	const uint64_t retval = llround(0.000000031 + ((task->size * 1000 * 1000) / double(3205.0 * 1024 * 1024)));
	       	svc_sum += retval;
	       	return retval;
	}
	if (0 == taskname.compare(std::string("WRITE"))) {
	       	const uint64_t retval = llround(write_service_time_distr(generator) + ((task->size * 1000 * 1000) / double(4688.0 * 1024 * 1024)));
	       	svc_sum += retval;
	       	return retval;
	}
	assert(0);
	return 0;
}

SSD_PM1733a::~SSD_PM1733a() {
	std::cout << "PM1733a " << name << " (" << my_index << ")" << std::endl;
	std::cout << "\tTotal I/Os " << n_tasks << std::endl;
	std::cout << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	std::cout << "\tTotal Bytes " << sz_sum << std::endl;
	std::cout << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
}

#endif // SERVER_CPP
