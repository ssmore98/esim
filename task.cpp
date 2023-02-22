#ifndef TASK_CPP
#define TASK_CPP

#include "task.h"
#include "server.h"
#include "generator.h"

Task::Task(const uint64_t & p_t, const size_t & p_size, const bool & pis_read, Server * const p_server, Generator * const p_generator):
       	t(p_t), size(p_size), server(p_server), generator(p_generator), is_read(pis_read) {
}

std::ostream & operator<<(std::ostream & o, Task * const io) {
	o << "t=" << io->t << " size=" << io->size << " server=" << 
		(io->server ? io->server->name : std::string("N/A"))  <<
	       	" generator=" << (io->generator ? io->generator->name : std::string("N/A")) << std::endl;
	return o;
}

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq) {
	for (TaskQ::const_iterator i = taskq.begin(); i != taskq.end(); i++) {
		o << (*i)->t << " ";
	}
	o << std::endl;
	return o;
}

#if 0
ReadTask::ReadTask(const uint64_t & t, const size_t & size, Server * const server, Generator * const generator):
       	Task(t, size, server, generator) {
}

WriteTask::WriteTask(const uint64_t & t, const size_t & size, Server * const server, Generator * const generator):
       	Task(t, size, server, generator) {
}
#endif

SubTask::SubTask(const uint64_t & t, const size_t & size, const bool & is_read, Server * const server, Generator * const generator):
       	Task(t, size, is_read, server, generator), mtask(NULL) {
       	}

MasterTask * & SubTask::MTASK() {
       	return mtask;
}

MasterTask::MasterTask(SubTasks p_tasks): Task(0, 0, false, NULL, NULL), tasks(p_tasks) {
	for (SubTasks::iterator i = tasks.begin(); i != tasks.end(); i++) {
		assert(!(*i)->MTASK());
		(*i)->MTASK() = this;
	}
}

#endif // TASK_CPP
