#ifndef GENERATOR_CPP
#define GENERATOR_CPP

#include <chrono>
#include <iostream>

#include "server.h"
#include "generator.h"

std::default_random_engine Generator::generator(std::chrono::system_clock::now().time_since_epoch().count());
uint16_t Generator::index = 0;

Generator::Generator(const std::string & p_name, const size_t & p_size, const uint16_t & p_percent_read, const uint16_t & p_percent_random,
	       	Events & p_events, Server * const p_server):
       	events(p_events), rw_type_distr((double)100), loc_distr((double)100), ia_time_sum(0), ia_time_count(0), last_task_time(0),
	server(p_server), my_index(index), name(p_name), percent_read(p_percent_read), percent_random(p_percent_random), size(p_size) {
		index++;
}

void Generator::IssueTask(const uint64_t & t) {
       	GeneratorEvent * const e = new GeneratorEvent(t, EvTyStartTask, this);
       	events.push_back(e);
       	std::push_heap(events.begin(), events.end(), cmp);
}

Generator::~Generator() {
       	std::cout << "GENERATOR " << name << " (" << my_index << ")" << std::endl;
       	std::cout << "\tServer " << server->name << std::endl;
       	if (ia_time_count)
	       	std::cout << "\tavIATime " << double(ia_time_sum) / double(ia_time_count) << std::endl;
}

RateGenerator::RateGenerator(const std::string & name, const size_t & size, const uint16_t & percent_read, const uint16_t & percent_random,
	       	Events & events, const unsigned int & ia_time, Server * const server):
       	Generator(name, size, percent_read, percent_random, events, server), ia_time_distr(1.0 / (double)ia_time) {
	// std::cout << "CREATE RATE GENERATOR\n";
	IssueTask(0);
}
void RateGenerator::StartTask(const uint64_t & t) {
       	const uint64_t this_ia_time = llround(ia_time_distr(generator));
       	IssueTask(t + this_ia_time);
       	ia_time_sum += t - last_task_time;
       	ia_time_count++;
       	Task * const task =  new Task(t, size, (rw_type_distr(generator) < percent_read), (loc_distr(generator) < percent_random), server, this);
       	server->Queue(events, t, task);
       	last_task_time = t;
}
void RateGenerator::EndTask(const uint64_t & t) {
}

QueueGenerator::QueueGenerator(const std::string & name, const size_t & size, const uint16_t & percent_read, const uint16_t & percent_random,
	       	Events & events, const unsigned int & p_qdepth, Server * const server):
       	Generator(name, size, percent_read, percent_random, events, server), qdepth(p_qdepth) {
	// std::cout << "CREATE QUEUE GENERATOR\n";
	for (unsigned int i = 0; i < qdepth; i++) {
	       	IssueTask(0);
	}
}
void QueueGenerator::StartTask(const uint64_t & t) {
       	ia_time_sum += t - last_task_time;
       	ia_time_count++;
       	Task * const task =  new Task(t, size, (rw_type_distr(generator) < percent_read), (loc_distr(generator) < percent_random), server, this);
       	server->Queue(events, t, task);
       	last_task_time = t;
}
void QueueGenerator::EndTask(const uint64_t & t) {
	StartTask(t);
}
#endif // GENERATOR_CPP
