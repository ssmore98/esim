#ifndef SERVER_CPP
#define SERVER_CPP

#include <chrono>
#include <iostream>

#include "server.h"
#include "generator.h"

std::default_random_engine Server::generator(std::chrono::system_clock::now().time_since_epoch().count());
uint16_t Server::index = 0;

Server::Server(const std::string & p_name, const unsigned int & service_time): n_tasks(0), task_time(0), qd_sum(0), svc_sum(0),
       	sz_sum(0), my_index(index), name(p_name)	{
	       	index++;
       	}

void Server::Queue(Events & events, const uint64_t & t, Task * const task) {
	assert(task);
	// std::cout << ">IO " << task <<std::endl;
	// std::cout << ">IOQ " << taskq;
	assert(MAX_TASKQ > taskq.size());
	taskq.push_back(task);
	qd_sum += taskq.size();
	if (1 == taskq.size()) {
	       	ServerEvent * const e = ScheduleTaskEnd(task, t);
		if (e) {
		       	events.push_back(e);
		       	std::push_heap(events.begin(), events.end(), cmp);
		}
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
       	if (task->server) task->server->EndTask(task, t);
       	if (task->generator) task->generator->EndTask(t);
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
#if 0
	std::cout << "SERVER " << name << " (" << my_index << ")" << std::endl;
	std::cout << "\tTasks " << n_tasks << std::endl;
	std::cout << "\tVolume " << sz_sum << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
#endif
}

SSD_PM1733a::SSD_PM1733a(const std::string & name, const uint64_t & p_t): Server(name, 0),
       	read_service_time_distr(double(1)/(double)0.031), write_service_time_distr(double(1)/double(8)), current_time(p_t) {
}

uint64_t SSD_PM1733a::GetServiceTime(Task * const task) {
	if (task->is_read) {
	       	const uint64_t retval = llround(0.000000031 + ((task->size * 1000 * 1000) / double(3205.0 * 1024 * 1024)));
	       	svc_sum += retval;
	       	return retval;
	} else {
	       	const uint64_t retval = llround(write_service_time_distr(generator) + ((task->size * 1000 * 1000) / double(4688.0 * 1024 * 1024)));
	       	svc_sum += retval;
	       	return retval;
	}
	assert(0);
	return 0;
}

ServerEvent * const SSD_PM1733a::ScheduleTaskEnd(Task * const task, const uint64_t & t) {
       	const uint64_t service_time = GetServiceTime(task);
       	ServerEvent * const e = new ServerEvent(t + service_time, EvTyEndTask, this);
	return e;
}

void SSD_PM1733a::EndTask(Task * const task, const uint64_t & t) {
	return;
}

SSD_PM1733a::~SSD_PM1733a() {
	std::cout << "PM1733a " << name << " (" << my_index << ")" << std::endl;
	std::cout << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) std::cout << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	std::cout << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) std::cout << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
}

RAID_1::RAID_1(const std::string & name, Servers p_servers, const uint64_t & t): Server(name, 0), select_server_distr(0, p_servers.size() - 1),
	current_time(t), servers(p_servers) {
}

void RAID_1::Queue(Events & events, const uint64_t & t, Task * const task) {
	SubTasks tasks;
	if (task->is_read) {
		Servers::iterator i_server = servers.begin();
		std::advance(i_server, select_server_distr(generator));
		if (i_server != servers.end()) {
		       	SubTask * const stask = new SubTask(task->t, task->size, task->is_read, task->is_random, this, NULL);
		       	tasks.push_back(stask);
		       	(*i_server)->Queue(events, t, stask);
		} else {
			assert(0);
		}
	} else {
		for (Servers::iterator i_server = servers.begin(); i_server != servers.end(); i_server++) {
		       	SubTask * const stask = new SubTask(task->t, task->size, task->is_read, task->is_random, this, NULL);
		       	tasks.push_back(stask);
		       	(*i_server)->Queue(events, t, stask);
		}
	}
	MasterTask * const mtask = new MasterTask(task->t, task->size, task->is_read, task->is_random, task->server, task->generator, tasks);
	delete task;
	Server::Queue(events, t, mtask);
       	// std::cout << "A " << mtask << " IOQ " << taskq;
}

uint64_t RAID_1::GetServiceTime(Task * const task) {
	assert(0);
}

ServerEvent * const RAID_1::ScheduleTaskEnd(Task * const task, const uint64_t & t) {
	return NULL;
}

std::ostream & operator<<(std::ostream & o, const Servers & servers) {
	for (Servers::const_iterator i = servers.begin(); i != servers.end(); i++) {
		o << (*i)->name << " ";
	}
	return o;
}

void RAID_1::UnQueue(Events & events, const uint64_t & t) {
	assert(0);
}

void RAID_1::EndTask(Task * const task, const uint64_t & t) {
	// this is a subtask
	// find the master task
	MasterTask * const m_task = task->MTASK();
	// std::cout << m_task << std::endl;
	if (0 == m_task->EndTask(task, t)) {	
	       	// std::cout << "R " << m_task << " IOQ " << taskq;
		TaskQ::iterator i_task = std::find(taskq.begin(), taskq.end(), m_task);
		if (taskq.end() != i_task) {
		       	// std::cout << "IO start:" << (*i_task)->t << " end:" << t << std::endl;
			task_time += t - (*i_task)->t;
		       	n_tasks++;
			Generator * const g = (*i_task)->generator;
		       	sz_sum += (*i_task)->size;
		       	delete m_task;
		       	taskq.erase(i_task);
		       	if (g) g->EndTask(t);
		} else {
			assert(0);
		}
	}
}

RAID_1::~RAID_1() {
	std::cout << "RAID_1 " << name << " (" << my_index << ") " << "(servers: " << servers << ")" << std::endl;
	std::cout << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) std::cout << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	std::cout << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) std::cout << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	// std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
}

RAID_5::RAID_5(const std::string & name, Servers p_servers, const size_t & p_stripe_width, const uint64_t & t): Server(name, 0),
       	select_server_distr(0, p_servers.size() - 1), current_time(t), current_stripe_fill(p_stripe_width * (p_servers.size() - 1)),
       	servers(p_servers), stripe_width(p_stripe_width) {
}

RAID_5::~RAID_5() {
	std::cout << "RAID_5 " << name << " (" << my_index << ") " << "(servers: " << servers << ")" << std::endl;
	std::cout << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) std::cout << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	std::cout << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) std::cout << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	// std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
}

void RAID_5::EndTask(Task * const task, const uint64_t & t) {
	// this is a subtask
	// find the master task
	MasterTask * const m_task = task->MTASK();
	// std::cout << m_task << std::endl;
	if (0 == m_task->EndTask(task, t)) {	
	       	// std::cout << "R " << m_task << " IOQ " << taskq;
		TaskQ::iterator i_task = std::find(taskq.begin(), taskq.end(), m_task);
		if (taskq.end() != i_task) {
		       	// std::cout << "IO start:" << (*i_task)->t << " end:" << t << std::endl;
			task_time += t - (*i_task)->t;
		       	n_tasks++;
			Generator * const g = (*i_task)->generator;
		       	sz_sum += (*i_task)->size;
		       	delete m_task;
		       	taskq.erase(i_task);
		       	if (g) g->EndTask(t);
		} else {
			assert(0);
		}
	}
}

uint64_t RAID_5::GetServiceTime(Task * const task) {
	assert(0);
}

ServerEvent * const RAID_5::ScheduleTaskEnd(Task * const task, const uint64_t & t) {
	return NULL;
}

void RAID_5::UnQueue(Events & events, const uint64_t & t) {
	assert(0);
}

void RAID_5::Queue(Events & events, const uint64_t & t, Task * const task) {
	SubTasks tasks;
	if (task->is_read) {
		Servers::iterator i_server = servers.begin();
		std::advance(i_server, select_server_distr(generator));
		if (i_server != servers.end()) {
		       	SubTask * const stask = new SubTask(task->t, task->size, task->is_read, task->is_random, this, NULL);
		       	tasks.push_back(stask);
		       	(*i_server)->Queue(events, t, stask);
		} else {
			assert(0);
		}
	} else {
		if (task->is_random) {
			Servers::iterator i_server = servers.begin();
			std::advance(i_server, select_server_distr(generator));
			if (i_server != servers.end()) {
				{
				       	SubTask * const stask = new SubTask(task->t, task->size, true, task->is_random, this, NULL);
				       	
					tasks.push_back(stask); (*i_server)->Queue(events, t, stask);
				}
				{
				       	SubTask * const stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
				       	tasks.push_back(stask);
				       	(*i_server)->Queue(events, t, stask);
				}
			} else {
				assert(0);
			}
			i_server = servers.begin();
			std::advance(i_server, select_server_distr(generator));
			if (i_server != servers.end()) {
				{
				       	SubTask * const stask = new SubTask(task->t, task->size, true, task->is_random, this, NULL);
				       	tasks.push_back(stask);
				       	(*i_server)->Queue(events, t, stask);
				}
				{
				       	SubTask * const stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
				       	tasks.push_back(stask);
				       	(*i_server)->Queue(events, t, stask);
				}
			} else {
				assert(0);
			}
		} else {
			Servers::iterator i_server = servers.begin();
			std::advance(i_server, select_server_distr(generator));
			if (i_server != servers.end()) {
				{
				       	SubTask * const stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
				       	tasks.push_back(stask);
				       	(*i_server)->Queue(events, t, stask);
				}
				if (current_stripe_fill > task->size) {
					current_stripe_fill -= task->size;
				} else {
				       	i_server = servers.begin();
				       	std::advance(i_server, select_server_distr(generator));
				       	if (i_server != servers.end()) {
					       	SubTask * const stask = new SubTask(task->t, stripe_width,
							       	false, task->is_random, this, NULL);
					       	tasks.push_back(stask);
					       	(*i_server)->Queue(events, t, stask);
					} else {
						assert(0);
					}
					current_stripe_fill = stripe_width * (servers.size() - 1) - task->size + current_stripe_fill;
				}
			} else {
				assert(0);
			}
		}
	}
	MasterTask * const mtask = new MasterTask(task->t, task->size, task->is_read, task->is_random, task->server, task->generator, tasks);
	delete task;
	Server::Queue(events, t, mtask);
       	// std::cout << "A " << mtask << " IOQ " << taskq;
}

#endif // SERVER_CPP
