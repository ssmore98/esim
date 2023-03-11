#ifndef TASK_H
#define TASK_H

#include <cstdint>
#include <deque>
#include <iostream>
#include <cassert>
#include <set>

#define MAX_TASKQ (64 * 1024)

class Server;
class Generator;

class MasterTask;

class Task {
	protected:
		std::set<Server *> servers;
	public:
		const uint64_t t;
		const size_t size;
		Generator * const generator;
		const bool is_read;
		const bool is_random;
		Task(const uint64_t & p_t, const size_t & p_size, const bool & pis_read,
			       	const bool & pis_random, Generator * const p_generator);
		Task & operator=(Server * const p_server);
		const std::set<Server *> & SERVERS() const { return servers; }
		virtual ~Task() {
		}
};

std::ostream & operator<<(std::ostream & o, Task * const io);

typedef std::deque<Task *> TaskQ;
typedef std::set<Task *> Tasks;

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq);
std::ostream & operator<<(std::ostream & o, const Tasks & tasks);

#if 0
class SubTask: public Task {
	protected:
		MasterTask * mtask;
	public:
		SubTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random,
			       	Server * const server, Generator * const generator);
		virtual MasterTask * MTASK() const;
		SubTask & operator=(MasterTask * const p_mtask);
};

typedef std::set<SubTask *> SubTasks;

class MasterTask: public Task {
	protected:
		SubTasks tasks;
	public:
	       	MasterTask(const uint64_t & t, const size_t & size, const bool & is_read, const bool & is_random,
			       	Server * const server, Generator * const generator, SubTasks p_tasks);
	       	size_t EndTask(Task * const task, const uint64_t & t);
};
#endif
#endif // TASK_H
