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
	if (n_tasks) {
	       	o << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	o << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	o << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
}

Server::Server(const std::string & p_name, const unsigned int & service_time):
       	n_tasks(0), task_time(0), qd_sum(0), svc_sum(0),
       	sz_sum(0), my_index(index), name(p_name) {
	       	index++;
       	}

Task * const Server::Queue(Events & events, const uint64_t & t, Task * const task) {
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
	return task;
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
}

SSD_PM1733a::SSD_PM1733a(const std::string & name): Drive(name, 0),
       	read_service_time_distr(double(1)/(double)0.031), write_service_time_distr(double(1)/double(8)) {
}

uint64_t SSD_PM1733a::GetServiceTime(Task * const task) {
	if (task->is_read) {
	       	const uint64_t retval = llround(0.000000031 + ((task->size * 1000 * 1000) / double(3205.0 * 1024 * 1024)));
	       	svc_sum += retval;
		if (!retval) return 1;
	       	return retval;
	} else {
	       	const uint64_t retval = llround(write_service_time_distr(generator) + ((task->size * 1000 * 1000) / double(4688.0 * 1024 * 1024)));
	       	svc_sum += retval;
		if (!retval) return 1;
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
	assert(0);
	return;
}

size_t SSD_PM1733a::StripeSize() const {
	assert(0);
	return 0;
}

SSD_PM1733a::~SSD_PM1733a() {
	/*
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
	*/
}

void SSD_PM1733a::print(std::ostream & o, const uint64_t & current_time) {
	o << "SSD_PM1733a " << name << " (" << my_index << ")" << std::endl;
	o << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) o << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	o << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) o << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	this->Server::print(o, current_time);
}

RAID::RAID(const std::string & name, const unsigned int & service_time): Server(name, service_time) {
}

RAID_0::RAID_0(const std::string & name, Drives & p_drives, const size_t & p_stripe_width, const std::string & p_raid_level):
       	RAID(name, 0), select_server_distr(0, p_drives.size() - 1),
       	drives(p_drives), stripe_width(p_stripe_width), raid_level(p_raid_level) {
}

void RAID_0::print(std::ostream & o, const uint64_t & current_time) {
	o << "RAID-" << raid_level << " " << name << " (" << my_index << ")" << std::endl;
	o << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) o << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	o << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) o << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	this->Server::print(o, current_time);
}

size_t RAID_0::StripeSize() const {
	return stripe_width * drives.size();
}

RAID_0::~RAID_0() {
	/*
	std::cout << "RAID_0 " << name << " (" << my_index << ") " << "(servers: " << servers << ")" << std::endl;
	std::cout << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) std::cout << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	std::cout << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) std::cout << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	// std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
	*/
}

uint64_t RAID_0::GetServiceTime(Task * const task) {
	assert(0);
}

ServerEvent * const RAID_0::ScheduleTaskEnd(Task * const task, const uint64_t & t) {
	return NULL;
}

void RAID_0::UnQueue(Events & events, const uint64_t & t) {
	assert(0);
}

Task * const RAID_0::Queue(Events & events, const uint64_t & t, Task * const task) {
	SubTasks stasks;
       	Drives::iterator data_drive = drives.begin();
       	std::advance(data_drive, select_server_distr(generator));
       	SubTask * const stask = new SubTask(task->t, task->size, task->is_read, task->is_random, this, NULL);
       	stasks.push_back(stask);
       	(*data_drive)->Queue(events, t, stask);
	MasterTask * const mtask = new MasterTask(task->t, task->size, task->is_read, task->is_random, task->server, task->generator, stasks);
	delete task;
	Server::Queue(events, t, mtask);
       	// std::cout << "A " << mtask << " IOQ " << taskq;
	return mtask;
}

void RAID_0::EndTask(Task * const task, const uint64_t & t) {
	// this is a subtask
	// find the master task
	MasterTask * const m_task = ((SubTask *)task)->MTASK();
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

RAID_1::RAID_1(const std::string & name, Drives & drives): RAID_0(name, drives, 0, "1"),
       	select_server_distr(0, drives.size() - 1) {
}

void RAID_1::print(std::ostream & o, const uint64_t & current_time) {
	this->RAID_0::print(o, current_time);
}

size_t RAID_1::StripeSize() const {
	return 0;
}

Task * const RAID_1::Queue(Events & events, const uint64_t & t, Task * const task) {
	SubTasks stasks;
	if (task->is_read) {
		Drives::iterator i_drive = drives.begin();
		std::advance(i_drive, select_server_distr(generator));
		if (i_drive != drives.end()) {
		       	SubTask * const stask = new SubTask(task->t, task->size, task->is_read, task->is_random, this, NULL);
		       	stasks.push_back(stask);
		       	(*i_drive)->Queue(events, t, stask);
		} else {
			assert(0);
		}
	} else {
		for (Drives::iterator i_drive = drives.begin(); i_drive != drives.end(); i_drive++) {
		       	SubTask * const stask = new SubTask(task->t, task->size, task->is_read, task->is_random, this, NULL);
		       	stasks.push_back(stask);
		       	(*i_drive)->Queue(events, t, stask);
		}
	}
	MasterTask * const mtask = new MasterTask(task->t, task->size, task->is_read, task->is_random, task->server, task->generator, stasks);
	delete task;
	Server::Queue(events, t, mtask);
       	// std::cout << "A " << mtask << " IOQ " << taskq;
	return mtask;
}

uint64_t RAID_1::GetServiceTime(Task * const task) {
	assert(0);
}

ServerEvent * const RAID_1::ScheduleTaskEnd(Task * const task, const uint64_t & t) {
	return NULL;
}

void RAID_1::UnQueue(Events & events, const uint64_t & t) {
	assert(0);
}

RAID_1::~RAID_1() {
	/*
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
	*/
}

RAID_5::RAID_5(const std::string & name, Drives & drives, const size_t & stripe_width):
       	RAID_0(name, drives, stripe_width, "5") {
}

void RAID_5::print(std::ostream & o, const uint64_t & current_time) {
	this->RAID_0::print(o, current_time);
}

size_t RAID_5::StripeSize() const {
	return stripe_width * (drives.size() - 1);
}

RAID_5::~RAID_5() {
	/*
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
	*/
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

Task * const RAID_5::Queue(Events & events, const uint64_t & t, Task * const task) {
	if (task->is_read) return RAID_0::Queue(events, t, task);
	SubTasks stasks;
       	Drives::iterator data_drive = drives.begin();
       	std::advance(data_drive, select_server_distr(generator));
	Drives::iterator parity_drive = drives.begin();
	do {
		parity_drive = drives.begin();
		std::advance(parity_drive, select_server_distr(generator));
	} while (parity_drive == data_drive);
	SubTask * stask = new SubTask(task->t, task->size, true, task->is_random, this, NULL);
	stasks.push_back(stask);
	(*data_drive)->Queue(events, t, stask);
	stask = new SubTask(task->t, task->size, true, task->is_random, this, NULL);
	stasks.push_back(stask);
	(*parity_drive)->Queue(events, t, stask);

	stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
	stasks.push_back(stask);
	(*data_drive)->Queue(events, t, stask);
	stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
	stasks.push_back(stask);
	(*parity_drive)->Queue(events, t, stask);
	MasterTask * const mtask = new MasterTask(task->t, task->size, task->is_read, task->is_random, task->server, task->generator, stasks);
	delete task;
	Server::Queue(events, t, mtask);
       	// std::cout << "A " << mtask << " IOQ " << taskq;
	return mtask;
}

RAID_4::RAID_4(const std::string & name, Drives & data_drives, Drives & p_parity_drives, const size_t & p_stripe_width,
		const std::string & raid_level): RAID_0(name, data_drives, stripe_width, raid_level),
       	select_parity_distr(0, p_parity_drives.size() - 1),
       	parity_drives(p_parity_drives), stripe_width(p_stripe_width) {
}

size_t RAID_4::StripeSize() const {
	return RAID_0::StripeSize();
}

RAID_4::~RAID_4() {
	/*
	std::cout << "RAID_4 " << name << " (" << my_index << ") " << "(data: " << " parity: " << parity_servers << ")" << std::endl;
	std::cout << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) std::cout << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	std::cout << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) std::cout << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	if (n_tasks) {
	       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
	       	// std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
	       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
	}
	*/
}

void RAID_4::print(std::ostream & o, const uint64_t & current_time) {
	this->RAID_0::print(o, current_time);
}

Task * const RAID_4::Queue(Events & events, const uint64_t & t, Task * const task) {
	if (task->is_read) return RAID_0::Queue(events, t, task);
	SubTasks stasks;
       	Drives::iterator data_server = drives.begin();
       	std::advance(data_server, select_server_distr(generator));
	SubTask * stask = new SubTask(task->t, task->size, true, task->is_random, this, NULL);
	stasks.push_back(stask);
	(*data_server)->Queue(events, t, stask);
	stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
	stasks.push_back(stask);
	(*data_server)->Queue(events, t, stask);

	for (Drives::iterator i_drive = parity_drives.begin(); i_drive != parity_drives.end(); i_drive++) {
	       	stask = new SubTask(task->t, task->size, true, task->is_random, this, NULL);
	       	stasks.push_back(stask);
	       	(*i_drive)->Queue(events, t, stask);
	       	stask = new SubTask(task->t, task->size, false, task->is_random, this, NULL);
	       	stasks.push_back(stask);
	       	(*i_drive)->Queue(events, t, stask);
	}

	MasterTask * const mtask = new MasterTask(task->t, task->size, task->is_read, task->is_random, task->server, task->generator, stasks);
	delete task;
	Server::Queue(events, t, mtask);
       	// std::cout << "A " << mtask << " IOQ " << taskq;
	return mtask;
}

void RAID_4::UnQueue(Events & events, const uint64_t & t) {
	assert(0);
}

uint64_t RAID_4::GetServiceTime(Task * const task) {
	assert(0);
}

ServerEvent * const RAID_4::ScheduleTaskEnd(Task * const task, const uint64_t & t) {
	return NULL;
}

RAID_DP::RAID_DP(const std::string & name, Drives & data_drives, Drives & parity_drives,
	       	const size_t & stripe_width): RAID_4(name, data_drives, parity_drives, stripe_width, "DP") {
}

RAID_DP::~RAID_DP() {
	/*
	std::cout << "RAID_DP " << name << " (" << my_index << ") " << "(data: " << " parity: " << ")" << std::endl;
	*/
}

size_t RAID_DP::GetAccWriteSize() const {
	size_t retval = 0;
	for (auto & n : write_tasks) retval += n->size;
	return retval;
}

Task * const RAID_DP::Queue(Events & events, const uint64_t & t, Task * const task) {
	if (task->is_read) return RAID_0::Queue(events, t, task);
	write_tasks.push_back(task);
	if (StripeSize() <= GetAccWriteSize()) {
	       	SubTasks stasks;
		for (Drives::iterator i_drive = drives.begin(); i_drive != drives.end(); i_drive++) {
		       	SubTask * const stask = new SubTask(t, stripe_width, false, false, this, NULL);
		       	stasks.push_back(stask);
		       	(*i_drive)->Queue(events, t, stask);
		}
	       	for (Drives::iterator i_drive = parity_drives.begin(); i_drive != parity_drives.end(); i_drive++) {
		       	SubTask * const stask = new SubTask(t, stripe_width, false, false, this, NULL);
		       	stasks.push_back(stask);
		       	(*i_drive)->Queue(events, t, stask);
		}
	       	MasterTask * const mtask = new MasterTask(t, StripeSize() + stripe_width * parity_drives.size(),
			       	false, false, this, NULL, stasks);
	       	Server::Queue(events, t, mtask);
		cps.insert(ConsistencyPoint(mtask, write_tasks));
		write_tasks.clear();
		// std::cout << mtask << std::endl;
	       	return mtask;
	}
	return NULL;
}

void RAID_DP::EndTask(Task * const task, const uint64_t & t) {
	if (task->is_read) {
		RAID_0::EndTask(task, t);
		return;
	}
	// this is a subtask
	// find the master task
	MasterTask * const m_task = ((SubTask *)task)->MTASK();
	assert(m_task);
       	// std::cout << "R " << m_task << " IOQ " << taskq;
	if (0 == m_task->EndTask(task, t)) {	
		// std::cout << cps;
	       	// std::cout << "R " << m_task << " IOQ " << taskq;
		TaskQ::iterator i_task = std::find(taskq.begin(), taskq.end(), m_task);
		if (taskq.end() != i_task) {
			assert(*i_task == m_task);
			if ((*i_task)->is_read) assert(0);
			ConsistencyPoints::iterator i_cp = cps.find(m_task);
			assert(i_cp != cps.end());
			Generators gs;
			for (Tasks::iterator j_task = i_cp->second.begin(); j_task != i_cp->second.end(); j_task++) {
			       	task_time += t - (*j_task)->t;
			       	n_tasks++;
			       	sz_sum += (*j_task)->size;
			       	Generator * const g = (*j_task)->generator;
			       	if (g) gs.push_back(g);
				delete *j_task;
			}
			cps.erase(i_cp);
			{
			       	Generator * const g = (*i_task)->generator;
			       	if (g) gs.push_back(g);
			}
		       	taskq.erase(i_task);
		       	delete m_task;
			for (Generators::iterator i_g = gs.begin(); i_g != gs.end(); i_g++) {
			       	(*i_g)->EndTask(t);
			}
		} else {
		       	std::cout << "R " << m_task << " IOQ " << taskq;
			assert(0);
		}
	}
}

void RAID_DP::print(std::ostream & o, const uint64_t & current_time) {
	this->RAID_4::print(o, current_time);
}

std::ostream & operator<<(std::ostream & o, const ConsistencyPoint & cp) {
	o << cp.first << " -> " << cp.second;
	return o;
}

std::ostream & operator<<(std::ostream & o, const ConsistencyPoints & cps) {
	o << "START CPLIST\n";
	for (ConsistencyPoints::const_iterator i = cps.begin(); i != cps.end(); i++) {
		o << "\t" << *i;
	}
	o << "END CPLIST\n";
	return o;
}

Drive::Drive(const std::string & name, const unsigned int & service_time): Server(name, service_time), shelf(NULL) {
}

Drive & Drive::operator=(Shelf * const p_shelf) {
	shelf = p_shelf;
	return *this;
}

Shelf::Shelf(const std::string & name, Drives p_drives): Server(name, 0), drives(p_drives) {
	for (Drives::iterator i = drives.begin(); i != drives.end(); i++) {
		**i = this;
	}
}

Shelf::~Shelf() {
}

void Shelf::print(std::ostream & o, const uint64_t & current_time) {
	o << "Shelf " /*<< name << " (" << my_index << ")"*/ << std::endl;
	/*
	o << "SSD_PM1733a " << name << " (" << my_index << ")" << std::endl;
	o << "\tTotal I/Os " << n_tasks << std::endl;
	if (current_time) o << "\tIOPS " << (n_tasks * 1000 * 1000) / current_time << std::endl;
	o << "\tTotal Bytes " << sz_sum << std::endl;
	if (current_time) o << "\tMBPS " << (sz_sum * 1000 * 1000) / (current_time * 1024 * 1024) << std::endl;
	this->Server::print(o, current_time);
	*/
}

#endif // SERVER_CPP
