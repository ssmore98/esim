#ifndef GENERATOR_CPP
#define GENERATOR_CPP

#include <chrono>
#include <iostream>
#include <iomanip>

#include "server.h"
#include "generator.h"

std::default_random_engine Generator::generator(std::chrono::system_clock::now().time_since_epoch().count());
uint16_t Generator::index = 0;

Generator::Generator(const std::string & p_name, const Bytes & p_size, const uint16_t & p_percent_read,
	       	const uint16_t & p_percent_random, RAID * const p_raid):
       	rw_type_distr((double)100), loc_distr((double)100), ia_time_sum(0), ia_time_count(0), last_task_time(0),
	controller(NULL), my_index(index), name(p_name), percent_read(p_percent_read),
       	percent_random(p_percent_random), size(p_size), raid(p_raid) {
		// std::cout << percent_read << std::endl;
		index++;
}

Generator::~Generator() {
}

RateGenerator::RateGenerator(const std::string & name, const Bytes & size, const uint16_t & percent_read,
	       	const uint16_t & percent_random, const Time & ia_time, RAID * const raid):
       	Generator(name, size, percent_read, percent_random, raid), ia_time_distr(1.0 / double(ia_time)) {
	// std::cout << "CREATE RATE GENERATOR\n";
}

Tasks RateGenerator::Begin(Events & events, const Time & t) {
	// std::cout << "BEGIN RATE GENERATOR\n";
	Tasks tasks;
	Task * const task = NextTask(events, t);
	tasks.insert(task);
	pending.insert(task);
	return tasks;
}

Task * const RateGenerator::NextTask(Events & events, const Time & t) {
       	Time this_ia_time(ia_time_distr(generator));
       	ia_time_sum += t - last_task_time;
       	ia_time_count++;
       	Task * const task =  new Task(t, size, (rw_type_distr(generator) < percent_read),
		       	(loc_distr(generator) < percent_random), this);
       	last_task_time = t;
	// std::cout << t << " " << this_ia_time << std::endl;
       	GeneratorEvent * const e = new GeneratorEvent(t + this_ia_time, EvTyRateGenNextTask, this); 
	events.push_back(e);
       	std::push_heap(events.begin(), events.end(), cmp);
	pending.insert(task);
	return task;
}

Task * const RateGenerator::EndTask(Task * const task, const Time & t) {
	assert(pending.erase(task));
	return NULL;
}

QueueGenerator::QueueGenerator(const std::string & name, const size_t & size, const uint16_t & percent_read,
	       	const uint16_t & percent_random, const unsigned int & p_qdepth, RAID * const raid):
       	Generator(name, size, percent_read, percent_random, raid), qdepth(p_qdepth) {
	// std::cout << "CREATE QUEUE GENERATOR\n";
}

Tasks QueueGenerator::Begin(Events & events, const Time & t) {
	Tasks tasks;
	for (QueueDepth i; i < qdepth; i++) {
		Task * const task = CreateTask(t);
	       	tasks.insert(task);
	       	pending.insert(task);
	}
	return tasks;
}

Task * const QueueGenerator::EndTask(Task * const task, const Time & t) {
	// std::cout << "DESTROY " << task << ' ' << pending;
	assert(pending.erase(task));
	delete task;
	// std::cout << task << ' ' << pending;
	Task * const retval = CreateTask(t);
       	pending.insert(retval);
	return retval;
}

Task * const QueueGenerator::CreateTask(const Time & t) {
       	ia_time_sum += t - last_task_time;
       	ia_time_count++;
       	Task * const task =  new Task(t, size, (rw_type_distr(generator) < percent_read),
		       	(loc_distr(generator) < percent_random), this);
       	last_task_time = t;
	// std::cout << "CREATE " << task << ' ' << pending;
	return task;
}

Task * const QueueGenerator::NextTask(Events & events, const Time & t) {
	assert(0);
	return NULL;
}

Generator & Generator::operator=(Controller * const c) {
	assert(!controller);
	assert(c);
	controller = c;
	return *this;
}

#endif // GENERATOR_CPP
