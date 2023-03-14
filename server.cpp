#ifndef SERVER_CPP
#define SERVER_CPP

#include <chrono>
#include <iostream>
#include <memory>

#include "server.h"
#include "generator.h"

std::default_random_engine Server::generator(std::chrono::system_clock::now().time_since_epoch().count());
uint16_t Server::index = 0;

std::ostream & operator<<(std::ostream & o, const Servers & servers) {
	for (Servers::const_iterator i = servers.begin(); i != servers.end(); i++) {
		o << (*i)->name << " ";
	}
	return o;
}

void Server::print(std::ostream & o, const uint64_t & current_time) {
	o << metrics;
}

Server::Server(const std::string & p_name): my_index(index), name(p_name) {
       	index++;
}

Server::~Server() {
}

SSD_PM1733a::SSD_PM1733a(const std::string & name): Drive(name),
       	read_service_time_distr(double(1)/(double)0.031), write_service_time_distr(double(1)/double(8)) {
}

uint64_t SSD_PM1733a::GetServiceTime(Task * const task) {
	assert(task);
	if (task->is_read) {
	       	const uint64_t retval = llround(0.000000031 + ((task->size * 1000 * 1000) / double(3205.0 * 1024 * 1024)));
		if (!retval) return 1;
	       	return retval;
	} else {
	       	const uint64_t retval = llround(write_service_time_distr(generator) + ((task->size * 1000 * 1000) / double(4688.0 * 1024 * 1024)));
		if (!retval) return 1;
	       	return retval;
	}
	assert(0);
	return 0;
}

size_t SSD_PM1733a::StripeSize() const {
	assert(0);
	return 0;
}

SSD_PM1733a::~SSD_PM1733a() {
}

void SSD_PM1733a::print(std::ostream & o, const uint64_t & current_time) {
	o << "SSD_PM1733a " << name << " (" << my_index << ")" << std::endl;
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	this->Server::print(o, current_time);
}

Drive::Drive(const std::string & name): Server(name), shelf(NULL) {
}

Drive & Drive::operator=(Shelf * const p_shelf) {
	shelf = p_shelf;
	return *this;
}

Shelf * const Drive::SHELF() const {
	return shelf;
}

Shelf::Shelf(const std::string & p_name): name(p_name) {
}

Shelf::~Shelf() {
}

static uint64_t sumtasks(uint64_t acc, IOModule * const x) { return acc + x->METRICS().N_TASKS(); }
static uint64_t sumszs(uint64_t acc, IOModule * const x) { return acc + x->METRICS().SZ_SUM(); }

void Shelf::print(std::ostream & o, const uint64_t & current_time) {
	o << "Shelf " << name << std::endl;
	/*
	for (IOModules::const_iterator i = ioms.begin(); i != ioms.end(); i++) {
	       	o << "\t IOM " << (*i)->name << std::endl;
	}
	for (Drives::const_iterator i = drives.begin(); i != drives.end(); i++) {
	       	o << "\t Drive " << (*i)->name << std::endl;
	}
	*/
	if (current_time) o << "\tIOPS " << (std::accumulate<IOModules::iterator, uint64_t>(ioms.begin(), ioms.end(), 0, sumtasks)
		       	* 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (std::accumulate<IOModules::iterator, uint64_t>(ioms.begin(), ioms.end(), 0, sumszs) * 1000 * 1000)
	       	/ (current_time * 1024 * 1024) << std::endl;
	o << "\tTotal I/Os " << std::accumulate<IOModules::iterator, uint64_t>(ioms.begin(), ioms.end(), 0, sumtasks) << std::endl;
	o << "\tTotal Bytes " << std::accumulate<IOModules::iterator, uint64_t>(ioms.begin(), ioms.end(), 0, sumszs) << std::endl;
	ioms.print(o, current_time);
}

Shelf & Shelf::operator=(IOModule * const iom) {
	ioms.insert(iom);
	return *this;
}

Shelf & Shelf::operator=(Drive * const drive) {
	drives.insert(drive);
	return *this;
}

IOModule::IOModule(const std::string & name, const uint64_t & p_service_time): Server(name), shelf(NULL), service_time(p_service_time) {
}

IOModule::~IOModule() {
}

void IOModule::print(std::ostream & o, const uint64_t & current_time) {
	o << "IOM " << name << std::endl;
	/*
	o << "\tTotal I/Os " << metrics.N_TASKS() << std::endl;
	o << "\tTotal Bytes " << metrics.SZ_SUM() << std::endl;
	*/
	if (current_time) o << "\tIOPS " << (metrics.N_TASKS() * 1000 * 1000) / current_time << std::endl;
	if (current_time) o << "\tMBPS " << (metrics.SZ_SUM() * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	this->Server::print(o, current_time);
}

IOModule & IOModule::operator=(Shelf * const p_shelf) {
	shelf = p_shelf;
	return *this;
}

static uint64_t sumtasktime(uint64_t acc, IOModule * const x) { return acc + x->METRICS().TASK_TIME(); }
static uint64_t sumsvcsum(uint64_t acc, IOModule * const x) { return acc + x->METRICS().SVC_SUM(); }
static uint64_t sumqdsum(uint64_t acc, IOModule * const x) { return acc + x->METRICS().QD_SUM(); }

void IOModules::print(std::ostream & o, const uint64_t & current_time) const {
	if (std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks)) {
	       	o << "\tavLatency " << double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasktime)) /
		       	double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks) * 1000 * 1000) << std::endl;
	       	o << "\tavServiceTime " << double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumsvcsum)) /
		       	double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks) * 1000 * 1000) << std::endl;
	       	o << "\tavQueueDepth " << double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumqdsum)) /
		       	double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks) * 1000 * 1000) << std::endl;
	}
}

ServerEvents IOModule::Submit(Task * const task, const uint64_t & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	taskq.push_back(task);
       	metrics.StartTask(taskq.size(), service_time, task->size);
	if (1 == taskq.size()) {
		ServerEvents retval;
	       	retval.insert(new ServerEvent(t + service_time, EvTyIOMFinProc, this));
		return retval;
	}
	return ServerEvents();
}

ServerEvents IOModule::Start(const uint64_t & t) {
	assert(0 < taskq.size());
	Task * const finished_task = taskq.front();
	taskq.pop_front();
       	metrics.EndTask(t - finished_task->t);
       	ServerEvents retval;
	if (0 < taskq.size()) {
	       	retval.insert(new ServerEvent(t + service_time, EvTyIOMFinProc, this));
	}
	for (Drives::iterator drive = shelf->DRIVES().begin(); drive != shelf->DRIVES().end(); drive++) {
		if (finished_task->SERVERS().end() != finished_task->SERVERS().find(*drive)) {
		       	pending_tasks.insert(finished_task);
			ServerEvents sretval = (*drive)->Submit(finished_task, t);
			retval.insert(sretval.begin(), sretval.end());
			return retval;
		}
	}
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> IOModule::Finish(const uint64_t & t, Task * const task) {
	assert(task);
	Tasks::iterator itask = pending_tasks.find(task);
	assert(pending_tasks.end() != itask);
       	pending_tasks.erase(itask);
       	return std::pair<Task *, Event *>(task, NULL);
}

ServerEvents SSD_PM1733a::Submit(Task * const task, const uint64_t & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	assert(MAX_TASKQ > taskq.size());
	taskq.push_back(task);
	if (1 == taskq.size()) {
	       	const uint64_t xtime = GetServiceTime(task);
		metrics.StartTask(1, xtime, task->size);
	       	ServerEvents retval;
	       	retval.insert(new ServerEvent(t + xtime, EvTyServDiskEnd, this));
		return retval;
	}
	return ServerEvents();
}

ServerEvents SSD_PM1733a::Start(const uint64_t & t) {
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> SSD_PM1733a::Finish(const uint64_t & t, Task * const task) {
	assert(!task);
	assert(0 < taskq.size());
	Task * const finished_task = taskq.front();
	taskq.pop_front();
       	metrics.EndTask(t - finished_task->t);
	Event * next_event = NULL;
	if (0 < taskq.size()) {
	       	Task * const next_task = taskq.front();
	       	const uint64_t xtime = GetServiceTime(next_task);
		metrics.StartTask(taskq.size(), xtime, next_task->size);
	       	next_event = new ServerEvent(t + xtime, EvTyServDiskEnd, this);
	}
	return std::pair<Task *, Event *>(finished_task, next_event);
}

#endif // SERVER_CPP
