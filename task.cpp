#ifndef TASK_CPP
#define TASK_CPP

#include "task.h"
#include "server.h"
#include "generator.h"

Task::Task(const Time & p_t, const Bytes & p_size, const bool & pis_read, const bool & pis_random, Generator * const p_generator):
       	t(p_t), size(p_size), generator(p_generator), is_read(pis_read), is_random(pis_random) {
}

static std::string concat(std::string & a, Server * b) { return a + " " + b->name; }

std::ostream & operator<<(std::ostream & o, Task * const io) {
	o << "t=" << io->t << " size=" << io->size << " servers=" << 
		std::accumulate(io->SERVERS().begin(), io->SERVERS().end(), std::string(""), concat)  <<
	       	" generator=" << (io->generator ? io->generator->name : std::string("N/A"));
	return o;
}

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq) {
	for (TaskQ::const_iterator i = taskq.begin(); i != taskq.end(); i++) {
		o << "(" << (*i).first << ") ";
	}
	o << std::endl;
	return o;
}

std::ostream & operator<<(std::ostream & o, const Tasks & tasks) {
	for (Tasks::const_iterator i = tasks.begin(); i != tasks.end(); i++) {
		o << "(" << (*i) << ") ";
	}
	o << std::endl;
	return o;
}

Task & Task::operator=(Server * const server) {
	servers.insert(server);
	return *this;
}

size_t TaskQ::size() const {
	return std::deque<std::pair<Task *, Time> >::size();
}

void TaskQ::push_back(Task * const task, const Time & t) {
	return std::deque<std::pair<Task *, Time> >::push_back(std::pair<Task *, Time>(task, t));
}

Task * const TaskQ::pop_front() {
	Task * const finished_task = std::deque<std::pair<Task *, Time> >::front().first;
	std::deque<std::pair<Task *, Time> >::pop_front();
	return finished_task;
}

std::pair<Task *, Time> TaskQ::start_task(const Time & t) {
       	// std::cout << t - front().second << std::endl;
	return std::pair<Task *, Time>(front().first, t - front().second);
}

#endif // TASK_CPP
