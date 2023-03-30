#ifndef RAID_CPP
#define RAID_CPP

#include <chrono>

#include "raid.h"

std::default_random_engine RAID::generator(std::chrono::system_clock::now().time_since_epoch().count());

RAID::RAID(const std::string & p_name): name(p_name) {
}

RAID::~RAID() {
}

RAID_0::RAID_0(const std::string & name, Drives & p_drives, const Bytes & p_stripe_width, const std::string & p_raid_level):
       	RAID(name), select_server_distr(0, p_drives.size() - 1),
       	drives(p_drives), stripe_width(p_stripe_width), raid_level(p_raid_level) {
       	// std::cout << stripe_width << std::endl;
}

void RAID_0::print(std::ostream & o, const Time & current_time) const {
	o << "RAID-" << raid_level << " " << name << std::endl;
	if (current_time) o << "\tIOPS " << metrics.N_TASKS() / current_time << std::endl;
	if (current_time) o << "\tMBPS " << metrics.SZ_SUM() / current_time << std::endl;
	o << metrics;
}

Bytes RAID_0::StripeSize() const {
	// std::cout << stripe_width << " " << drives.size() << std::endl;
	return stripe_width * drives.size();
}

RAID_0::~RAID_0() {
}

TaskList RAID_0::Execute(Task * const task) {
	// std::cout << task << std::endl;
	tasks.insert(task);
       	metrics.StartTask(tasks.size(), Time(), task->size);
	TaskList retval;
       	Drives::iterator data_drive = drives.begin();
       	std::advance(data_drive, select_server_distr(generator));
       	Task * const ntask = new Task(task->t, task->size, task->is_read, task->is_random, task->generator);
	Tasks ntasks;
	ntasks.insert(ntask);
	retval.insert(TaskList::value_type(*data_drive, ntasks));
	assert(retval.size());
	return retval;
}

Tasks  RAID_0::Finish(const Time & current_time, Task * const task) {
	Tasks::iterator itask = tasks.find(task);
	assert(tasks.end() != itask);
	tasks.erase(itask);
       	metrics.EndTask(0);
	Tasks retval;
	retval.insert(task);
	return retval;
}

RAID_1::RAID_1(const std::string & name, Drives & drives): RAID_0(name, drives, 0, "1"),
       	select_server_distr(0, drives.size() - 1) {
}

void RAID_1::print(std::ostream & o, const Time & current_time) const {
	this->RAID_0::print(o, current_time);
}

Bytes RAID_1::StripeSize() const {
	return 0;
}

TaskList RAID_1::Execute(Task * const task) {
	tasks.insert(task);
       	metrics.StartTask(tasks.size(), Time(), task->size);
	TaskList retval;
	if (task->is_read) {
		Drives::iterator i_drive = drives.begin();
		std::advance(i_drive, select_server_distr(generator));
	       	Task * const ntask = new Task(task->t, task->size, true, task->is_random, task->generator);
	       	Tasks ntasks;
	       	ntasks.insert(ntask);
	       	retval.insert(TaskList::value_type(*i_drive, ntasks));
		return retval;
	} else {
		for (Drives::iterator i_drive = drives.begin(); i_drive != drives.end(); i_drive++) {
		       	Task * const ntask = new Task(task->t, task->size, false, task->is_random, task->generator);
		       	Tasks ntasks;
		       	ntasks.insert(ntask);
		       	retval.insert(TaskList::value_type(*i_drive, ntasks));
		}
		return retval;
	}
	assert(0);
	return retval;
}

Tasks  RAID_1::Finish(const Time & current_time, Task * const task) {
	Tasks::iterator itask = tasks.find(task);
	assert(tasks.end() != itask);
	tasks.erase(itask);
       	metrics.EndTask(0);
	Tasks retval;
	retval.insert(task);
	return retval;
}

RAID_1::~RAID_1() {
}

RAID_5::RAID_5(const std::string & name, Drives & drives, const Bytes & stripe_width):
       	RAID_0(name, drives, stripe_width, "5") {
}

void RAID_5::print(std::ostream & o, const Time & current_time) const {
	this->RAID_0::print(o, current_time);
}

Bytes RAID_5::StripeSize() const {
	return stripe_width * (drives.size() - 1);
}

RAID_5::~RAID_5() {
}

TaskList RAID_5::Execute(Task * const task) {
	if (task->is_read) return RAID_0::Execute(task);
	tasks.insert(task);
       	metrics.StartTask(tasks.size(), Time(), task->size);
	TaskList retval;
       	Drives::iterator data_drive = drives.begin();
       	std::advance(data_drive, select_server_distr(generator));
	Drives::iterator parity_drive = drives.begin();
	do {
		parity_drive = drives.begin();
		std::advance(parity_drive, select_server_distr(generator));
	} while (parity_drive == data_drive);
       	Tasks ntasks;
	Task * ntask = new Task(task->t, task->size, true, task->is_random, task->generator);
       	ntasks.insert(ntask);
	ntask = new Task(task->t, task->size, false, task->is_random, task->generator);
       	ntasks.insert(ntask);
       	retval.insert(TaskList::value_type(*data_drive, ntasks));

	ntasks.clear();
	ntask = new Task(task->t, task->size, true, task->is_random, task->generator);
       	ntasks.insert(ntask);
	ntask = new Task(task->t, task->size, false, task->is_random, task->generator);
       	ntasks.insert(ntask);
       	retval.insert(TaskList::value_type(*parity_drive, ntasks));
	return retval;
}

Tasks  RAID_5::Finish(const Time & current_time, Task * const task) {
	Tasks::iterator itask = tasks.find(task);
	assert(tasks.end() != itask);
	tasks.erase(itask);
       	metrics.EndTask(0);
	Tasks retval;
	retval.insert(task);
	return retval;
}

RAID_4::RAID_4(const std::string & name, Drives & data_drives, Drives & p_parity_drives, const Bytes & stripe_width,
		const std::string & raid_level): RAID_0(name, data_drives, stripe_width, raid_level),
       	select_parity_distr(0, p_parity_drives.size() - 1),
       	parity_drives(p_parity_drives) {
}

Bytes RAID_4::StripeSize() const {
	return RAID_0::StripeSize();
}

RAID_4::~RAID_4() {
}

void RAID_4::print(std::ostream & o, const Time & current_time) {
	this->RAID_0::print(o, current_time);
}

TaskList RAID_4::Execute(Task * const task) {
	if (task->is_read) return RAID_0::Execute(task);
	tasks.insert(task);
       	metrics.StartTask(tasks.size(), Time(), task->size);
	TaskList retval;
       	Drives::iterator data_drive = drives.begin();
       	std::advance(data_drive, select_server_distr(generator));
       	Tasks ntasks;
	Task * ntask = new Task(task->t, task->size, true, task->is_random, task->generator);
       	ntasks.insert(ntask);
	ntask = new Task(task->t, task->size, false, task->is_random, task->generator);
       	ntasks.insert(ntask);
       	retval.insert(TaskList::value_type(*data_drive, ntasks));

	for (Drives::iterator i_drive = parity_drives.begin(); i_drive != parity_drives.end(); i_drive++) {
	       	Tasks ntasks;
	       	ntask = new Task(task->t, task->size, true, task->is_random, task->generator);
	       	ntasks.insert(ntask);
	       	ntask = new Task(task->t, task->size, false, task->is_random, task->generator);
	       	ntasks.insert(ntask);
	       	retval.insert(TaskList::value_type(*i_drive, ntasks));
	}
	return retval;
}

Tasks  RAID_4::Finish(const Time & current_time, Task * const task) {
	Tasks::iterator itask = tasks.find(task);
	assert(tasks.end() != itask);
	tasks.erase(itask);
       	metrics.EndTask(0);
	Tasks retval;
	retval.insert(task);
	return retval;
}

RAID_DP::RAID_DP(const std::string & name, Drives & data_drives, Drives & parity_drives,
	       	const Bytes & stripe_width): RAID_4(name, data_drives, parity_drives, stripe_width, "DP") {
}

RAID_DP::~RAID_DP() {
}

Bytes RAID_DP::GetAccWriteSize() const {
	Bytes retval = 0;
	for (auto & n : write_tasks) retval += n->size;
	return retval;
}

TaskList RAID_DP::Execute(Task * const task) {
	// std::cout << task << std::endl;
	if (task->is_read) return RAID_0::Execute(task);
       	metrics.StartTask(tasks.size(), Time(), task->size);
	// std::cout << StripeSize() << " " << GetAccWriteSize() << std::endl;
	// std::cout << task << " " << write_tasks;
	assert(write_tasks.end() == write_tasks.find(task));
	write_tasks.insert(task);
	// std::cout << task << " " << write_tasks;
	// std::cout << StripeSize() << " " << GetAccWriteSize() << std::endl;
	if (StripeSize() <= GetAccWriteSize()) {
	       	tasks.insert(task);
	       	TaskList retval;
		for (Drives::iterator i_drive = drives.begin(); i_drive != drives.end(); i_drive++) {
		       	Task * const ntask = new Task(task->t, stripe_width, false, false, task->generator);
		       	Tasks ntasks;
		       	ntasks.insert(ntask);
		       	retval.insert(TaskList::value_type(*i_drive, ntasks));
		}
	       	for (Drives::iterator i_drive = parity_drives.begin(); i_drive != parity_drives.end(); i_drive++) {
		       	Task * const ntask = new Task(task->t, stripe_width, false, false, task->generator);
		       	Tasks ntasks;
		       	ntasks.insert(ntask);
		       	retval.insert(TaskList::value_type(*i_drive, ntasks));
		}
		cps.insert(ConsistencyPoint(task, write_tasks));
		write_tasks.clear();
	       	return retval;
	}
	return TaskList();
}

Tasks RAID_DP::Finish(const Time & current_time, Task * const task) {
	if (task->is_read) {
		return RAID_0::Finish(current_time, task);
	}
	Tasks::iterator itask = tasks.find(task);
	assert(tasks.end() != itask);
	tasks.erase(itask);
       	ConsistencyPoints::iterator i_cp = cps.find(task);
       	assert(i_cp != cps.end());
	Tasks retval;
	retval.insert(task);
       	metrics.EndTask(current_time - task->t);
	for (Tasks::iterator i = i_cp->second.begin(); i != i_cp->second.end(); i++) {
	       	retval.insert(*i);
	       	metrics.EndTask(current_time - (*i)->t);
	}
	// std::cout << *i_cp << std::endl;
       	cps.erase(i_cp);
	return retval;
}

void RAID_DP::print(std::ostream & o, const Time & current_time) {
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

#endif // RAID_CPP
