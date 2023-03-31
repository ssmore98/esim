#ifndef DRIVE_CPP
#define DRIVE_CPP

#include <iomanip>

#include "server.h"

Drive::Drive(const std::string & name): Server(name), shelf(NULL) {
}

Drive & Drive::operator=(Shelf * const p_shelf) {
	shelf = p_shelf;
	return *this;
}

Shelf * const Drive::SHELF() const {
	return shelf;
}

void Drive::PrintConfig(std::ostream & o, const std::string & prefix) const {
	o << prefix << "Drive " << name << std::endl;
}

SSD_PM1733a::SSD_PM1733a(const std::string & name): Drive(name),
       	read_service_time_distr(double(1)/(double)0.031), write_service_time_distr(double(1)/double(8)) {
}

Time SSD_PM1733a::GetServiceTime(Task * const task) {
	assert(task);
	Time one(1.0);
	if (task->is_read) {
	       	const Time retval = task->size / DataRate(3205);
		// std::cout << __FILE__ << ':' << __LINE__  << ' ' << task->size << ' ' << retval << std::endl;
		if (!retval) return one;
	       	return retval;
	} else {
		Time t(write_service_time_distr(generator));
	       	const Time retval = t + (task->size / DataRate(4688));
		if (!retval) return one;
	       	return retval;
	}
	assert(0);
	return Time();
}

size_t SSD_PM1733a::StripeSize() const {
	assert(0);
	return 0;
}

SSD_PM1733a::~SSD_PM1733a() {
}

void SSD_PM1733a::print(std::ostream & o, const Time & current_time) {
	// o << "SSD_PM1733a " << name << " (" << my_index << ")" << std::endl;
	if (current_time) o << metrics.N_TASKS() / current_time;
	if (current_time) o << '\t' << metrics.SZ_SUM() / current_time;
	this->Server::print(o, current_time);
}

ServerEvents SSD_PM1733a::Submit(Task * const task, const Time & t) {
	assert(task->SERVERS().end() != task->SERVERS().find(this));
	assert(MAX_TASKQ > taskq.size());
	taskq.push_back(task);
       	metrics.QueueTask(QueueDepth(taskq.size()), Bytes(task->size));
	if (1 == taskq.size()) {
	       	const Time xtime = GetServiceTime(task);
		// std::cout << __FILE__ << ':' << __LINE__ << ' ' << metrics.SVC_SUM() << std::endl;
	       	// metrics.print(std::cout);
		// std::cout << __FILE__ << ':' << __LINE__ << std::endl;
		metrics.StartTask(t - task->t, xtime);
		// std::cout << __FILE__ << ':' << __LINE__ << ' ' << metrics.SVC_SUM() << std::endl;
	       	// metrics.print(std::cout);
		// std::cout << __FILE__ << ':' << __LINE__ << std::endl;
	       	ServerEvents retval;
	       	retval.insert(new ServerEvent(t + xtime, EvTyServDiskEnd, this, task));
		return retval;
	}
	return ServerEvents();
}

ServerEvents SSD_PM1733a::Start(const Time & t) {
	assert(0);
	return ServerEvents();
}

std::pair<Task *, Event *> SSD_PM1733a::Finish(const Time & t, Task * const task) {
	assert(!task);
	assert(0 < taskq.size());
	Task * const finished_task = taskq.front();
	taskq.pop_front();
       	metrics.EndTask(t - finished_task->t);
	// std::cout << this->METRICS().QD_SUM() << std::endl;
	Event * next_event = NULL;
	if (0 < taskq.size()) {
	       	Task * const next_task = taskq.front();
	       	const Time xtime = GetServiceTime(next_task);
		metrics.StartTask(t - next_task->t, xtime);
	       	next_event = new ServerEvent(t + xtime, EvTyServDiskEnd, this, next_task);
	}
	return std::pair<Task *, Event *>(finished_task, next_event);
}

void Drives::print(std::ostream & o, const Time & current_time) const {
	o << "Drive\t\tIOPS\tTPUT\t\tIOS\t\tBYTES\tRT\tST\tQLEN\tQTIME" << std::endl;
	o << std::string(80, '=') << std::endl;
	for (Drives::const_iterator i = begin(); i != end(); i++) {
		o << std::setw(10) << std::left << (*i)->name;
		o << '\t';
		(*i)->METRICS().print(o, current_time);
		// (*i)->print(o, current_time);
		// o << '\t' << (*i)->METRICS().TASK_TIME();
		o << std::endl;
	}
	o << std::string(80, '-') << std::endl;
}

#endif // DRIVE_CPP
