#ifndef SERVER_CPP
#define SERVER_CPP

#include <chrono>
#include <iostream>
#include <memory>
#include <iomanip>

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

void Server::print(std::ostream & o, const Time & current_time) {
	o << metrics;
}

Server::Server(const std::string & p_name): my_index(index), name(p_name) {
       	index++;
}

Server::~Server() {
}

Shelf::Shelf(const std::string & p_name, const uint16_t & p_slots): name(p_name), slots(p_slots) {
}

Shelf::~Shelf() {
}

static IOS sumtasks(IOS acc, IOModule * const x) { return acc + x->METRICS().N_TASKS(); }
static Bytes sumszs(Bytes acc, IOModule * const x) { return acc + x->METRICS().SZ_SUM(); }

void Shelf::print(std::ostream & o, const Time & current_time) {
	o << "Shelf " << name << std::endl;
	/*
	for (IOModules::const_iterator i = ioms.begin(); i != ioms.end(); i++) {
	       	o << "\t IOM " << (*i)->name << std::endl;
	}
	for (Drives::const_iterator i = drives.begin(); i != drives.end(); i++) {
	       	o << "\t Drive " << (*i)->name << std::endl;
	}
	*/
	if (current_time) o << "\tIOPS " << std::accumulate<IOModules::iterator, IOS>(ioms.begin(), ioms.end(), 0, sumtasks)
		       	 / current_time << std::endl;
	if (current_time) o << "\tMBPS " << std::accumulate<IOModules::iterator, Bytes>(ioms.begin(), ioms.end(), 0, sumszs)
	       	/ current_time << std::endl;
	o << "\tTotal I/Os " << std::accumulate<IOModules::iterator, uint64_t>(ioms.begin(), ioms.end(), 0, sumtasks) << std::endl;
	o << "\tTotal Bytes " << std::accumulate<IOModules::iterator, Bytes>(ioms.begin(), ioms.end(), 0, sumszs) << std::endl;
	ioms.cumulative_print(o, current_time);
}

Shelf & Shelf::operator=(IOModule * const iom) {
	ioms.insert(iom);
	return *this;
}

Shelf & Shelf::operator=(Drive * const drive) {
	assert(drives.size() < slots);
	drives.insert(drive);
	return *this;
}

IOModule::IOModule(const std::string & name, const Time & p_service_time, const DataRate & p_mbps):
       	Server(name), shelf(NULL), service_time(p_service_time), mbps(p_mbps) {
}

IOModule::~IOModule() {
}

void IOModule::print(std::ostream & o, const Time & current_time) {
	o << "IOM " << name << std::endl;
	/*
	o << "\tTotal I/Os " << metrics.N_TASKS() << std::endl;
	o << "\tTotal Bytes " << metrics.SZ_SUM() << std::endl;
	*/
	if (current_time) o << "\tIOPS " << metrics.N_TASKS() / current_time << std::endl;
	if (current_time) o << "\tMBPS " << metrics.SZ_SUM() / current_time << std::endl;
	this->Server::print(o, current_time);
}

IOModule & IOModule::operator=(Shelf * const p_shelf) {
	shelf = p_shelf;
	return *this;
}

static Time sumtasktime(Time acc, IOModule * const x) { return acc + x->METRICS().TASK_TIME(); }
static Time sumsvcsum(Time acc, IOModule * const x) { return acc + x->METRICS().SVC_SUM(); }
static QueueDepth sumqdsum(QueueDepth acc, IOModule * const x) { return acc + x->METRICS().QD_SUM(); }

void IOModules::cumulative_print(std::ostream & o, const Time & current_time) const {
	if (std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks)) {
	       	o << "\tavLatency " << double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasktime)) /
		       	double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks) * 1000 * 1000) << std::endl;
	       	o << "\tavServiceTime " << double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumsvcsum)) /
		       	double(std::accumulate<IOModules::iterator, uint64_t>(begin(), end(), 0, sumtasks) * 1000 * 1000) << std::endl;
	       	o << "\tavQueueDepth " << std::accumulate<IOModules::iterator, QueueDepth>(begin(), end(), 0, sumqdsum) /
		       	std::accumulate<IOModules::iterator, IOS>(begin(), end(), 0, sumtasks) << std::endl;
	}
}

void IOModules::print(std::ostream & o, const Time & current_time) const {
	o << "IOM\t\tIOPS\tTPUT\t\tIOS\t\tBYTES\tRT\tST\tQLEN\tQTIME" << std::endl;
	o << std::string(80, '=') << std::endl;
	for (IOModules::const_iterator i = begin(); i != end(); i++) {
	       	o << std::setw(10) << std::left  << (*i)->name;
		o << '\t';
		(*i)->METRICS().print(o, current_time);
		o << std::endl;
	}
	o << std::string(80, '-') << std::endl;
}

Time IOModule::GetServiceTime(Task * const task) {
	const Time xtime = task->size / mbps;
	// std::cout << __FILE__ << ':' << __LINE__ << ' '  << (xtime > service_time ? xtime : service_time) << std::endl;
	return (xtime > service_time ? xtime : service_time);
}

ServerEvents IOModule::Submit(Task * const task, const Time & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	taskq.push_back(task);
       	metrics.QueueTask(taskq.size(), task->size);
	if (1 == taskq.size()) {
		ServerEvents retval;
	       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << t << '+' << GetServiceTime(task) <<
		       	// '=' << t + GetServiceTime(task) <<
		       	// std::endl;
	       	// std::cout << __FILE__ << ':' << __LINE__ << '(' << t + GetServiceTime(task) << ')' << std::endl;
		metrics.StartTask(t - task->t, GetServiceTime(task));
		const Time nt(t + GetServiceTime(task));
	       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << nt << std::endl;
		ServerEvent * const se = new ServerEvent(nt, EvTyIOMFinProc, this, task);
	       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << se->t << std::endl;
	       	retval.insert(se);
	       	// std::cout << __FILE__ << ':' << __LINE__ << ' ' << retval << std::endl;
		return retval;
	}
	return ServerEvents();
}

ServerEvents IOModule::Start(const Time & t) {
	assert(0 < taskq.size());
	Task * const finished_task = taskq.front();
	taskq.pop_front();
       	ServerEvents retval;
	if (0 < taskq.size()) {
		metrics.StartTask(t - taskq.front()->t, GetServiceTime(taskq.front()));
	       	retval.insert(new ServerEvent(t + GetServiceTime(taskq.front()), EvTyIOMFinProc, this, taskq.front()));
	}
	for (Drives::iterator drive = shelf->DRIVES().begin(); drive != shelf->DRIVES().end(); drive++) {
		if (finished_task->SERVERS().end() != finished_task->SERVERS().find(*drive)) {
		       	pending_tasks.insert(finished_task);
			// std::cout << (*drive)->name << std::endl;
			ServerEvents sretval = (*drive)->Submit(finished_task, t);
			// std::cout << "sretval " << sretval.size() <<std::endl;
			retval.insert(sretval.begin(), sretval.end());
			return retval;
		}
	}
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> IOModule::Finish(const Time & t, Task * const task) {
	assert(task);
	Tasks::iterator itask = pending_tasks.find(task);
	assert(pending_tasks.end() != itask);
       	pending_tasks.erase(itask);
       	metrics.EndTask(t - task->t);
       	return std::pair<Task *, Event *>(task, NULL);
}

void IOModule::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "IO Module " << name << std::endl;
}

void Shelf::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "Shelf " << name << std::endl;
	for (IOModules::const_iterator i = ioms.begin(); i != ioms.end(); i++) {
		(*i)->PrintConfig(o, prefix + "\t");
	}
	for (Drives::const_iterator i = drives.begin(); i != drives.end(); i++) {
		(*i)->PrintConfig(o, prefix + "\t\t");
	}
}
#endif // SERVER_CPP
