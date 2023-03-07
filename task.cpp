#ifndef TASK_CPP
#define TASK_CPP

#include "task.h"
#include "server.h"
#include "generator.h"

Task::Task(const uint64_t & p_t, const size_t & p_size, const bool & pis_read, const bool & pis_random, Server * const p_server, Generator * const p_generator):
       	t(p_t), size(p_size), server(p_server), generator(p_generator), is_read(pis_read), is_random(pis_random) {
}

std::ostream & operator<<(std::ostream & o, Task * const io) {
	o << (void *)io << " t=" << io->t << " size=" << io->size << " server=" << 
		(io->server ? io->server->name : std::string("N/A"))  <<
	       	" generator=" << (io->generator ? io->generator->name : std::string("N/A"));
	return o;
}

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq) {
	for (TaskQ::const_iterator i = taskq.begin(); i != taskq.end(); i++) {
		o << "(" << (*i) << ") ";
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

SubTask::SubTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random, Server * const server, Generator * const generator):
       	Task(t, size, is_read, is_random, server, generator), mtask(NULL) {
       	}

SubTask & SubTask::operator=(MasterTask * const p_mtask) {
	mtask = p_mtask;
	return *this;
}

MasterTask * SubTask::MTASK() const {
       	return mtask;
}

MasterTask::MasterTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random, Server * const server,
	       	Generator * const generator, SubTasks p_tasks): Task(t, size, is_read, is_random, server, generator), tasks(p_tasks) {
	for (SubTasks::iterator i = tasks.begin(); i != tasks.end(); i++) {
		assert(!(*i)->MTASK());
		**i = this;
	}
}

size_t MasterTask::EndTask(Task * const task, const uint64_t & t) {
	SubTasks::iterator i = std::find(tasks.begin(), tasks.end(), task);
	if (i != tasks.end()) {
		tasks.erase(i);
		return tasks.size();
	}
	assert(0);
	return 0;
}

#endif // TASK_CPP
