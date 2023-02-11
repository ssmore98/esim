#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <chrono>
#include <random>

// #define SERVICE_TIME 15
// #define IA_TIME 20
#define SIMULATION_TIME 100000000

// random number generator
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

class Task {
	public:
		const uint64_t t;
		Task(const uint64_t & p_t): t(p_t) {
		}
};

std::ostream & operator<<(std::ostream & o, const Task & io) {
	o << io.t;
	return o;
}

typedef std::deque<Task *> TaskQ;

std::ostream & operator<<(std::ostream & o, const TaskQ & taskq) {
	for (TaskQ::const_iterator i = taskq.begin(); i != taskq.end(); i++) {
		o << (*i)->t << " ";
	}
	o << std::endl;
	return o;
}

typedef enum {EvTyNone, EvTyIssueTask, EvTyFinishTask} EventType;

class Event {
	public:
		const uint64_t t;
		const EventType type;
		Event(const uint64_t & p_t, const EventType & p_type): t(p_t), type(p_type) {
		}
		friend bool cmp(const Event * const, const Event * const);
};

typedef std::vector<Event *> Events;

bool cmp(const Event * const e1, const Event * const e2)  { 
	return e1->t > e2->t;
}

class Server {
	protected:
		TaskQ taskq;
		uint64_t n_tasks;
		uint64_t task_time;
		uint64_t qd_sum;
		uint64_t svc_sum;
	       	std::exponential_distribution<double> service_time_distr;
	public:
		Server(const unsigned int & service_time): n_tasks(0), task_time(0), qd_sum(0), svc_sum(0), service_time_distr(1.0 / (double)service_time) {
		}
		void Queue(Events & events, const uint64_t & t, Task * const task) {
			// std::cout << ">IO " << *io <<std::endl;
			// std::cout << ">IOQ " << ioq;
			taskq.push_back(task);
			qd_sum += taskq.size();
			if (1 == taskq.size()) {
				const uint64_t service_time = service_time_distr(generator);
				svc_sum += service_time;
			       	Event * const e = new Event(t + service_time, EvTyFinishTask);
			       	events.push_back(e);
			       	std::push_heap(events.begin(), events.end(), cmp);
			}
			// std::cout << ">>IOQ " << ioq;
		}
		void UnQueue(Events & events, const uint64_t & t) {
			if (1 > taskq.size()) throw taskq;
			// std::cout << "<IOQ " << ioq;
			Task * const task = taskq.front();
			// std::cout << "IO start:" << task->t << " end:" << t << std::endl;
			task_time += t - task->t;
			n_tasks++;
			delete task;
			taskq.pop_front();
			// std::cout << "<<IOQ " << ioq;
			if (0 < taskq.size()) {
				const uint64_t service_time = service_time_distr(generator);
				svc_sum += service_time;
			       	Event * const e = new Event(t + service_time, EvTyFinishTask);
			       	events.push_back(e);
			       	std::push_heap(events.begin(), events.end(), cmp);
			}
		}
		~Server() {
			std::cout << "Tasks " << n_tasks << std::endl;
			std::cout << "avLatency " << double(task_time) / double(n_tasks) << std::endl;
			std::cout << "avServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
			std::cout << "avQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
		}
};

void IssueTask(Events & events, const uint64_t & t) {
       	Event * const e = new Event(t, EvTyIssueTask);
       	events.push_back(e);
       	std::push_heap(events.begin(), events.end(), cmp);
}

void usage(const std::string & s) {
       	fprintf(stderr, "Usage: %s [-q <queue depth>]\n", s.c_str());
       	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

	int opt;
	unsigned int qdepth = 0;
	bool b2b = false;
	unsigned int ia_time = 0;
	unsigned int service_time = 0;
	while ((opt = getopt(argc, argv, "q:i:s:")) != -1) {
	       	switch (opt) {
		       	case 'q':
			       	b2b = true;
			       	qdepth = atoi(optarg);
			       	break;
		       	case 'i':
			       	b2b = false;
			       	ia_time = atoi(optarg);
			       	break;
		       	case 's':
			       	service_time = atoi(optarg);
			       	break;
		       	default: /* '?' */
				usage(argv[0]);
	       	}
       	}

	if (b2b) {
	       	if (1 > qdepth) {
		       	usage(argv[0]);
	       	}
	} else {
	       	if (1 > ia_time) {
		       	usage(argv[0]);
	       	}
	}
       	if (1 > service_time) {
	       	usage(argv[0]);
       	}

       	std::exponential_distribution<double> ia_time_distr(1.0 / (double)ia_time);

	uint64_t t = 0;
	Events events;
	Server server(service_time);
	if (b2b) {
		for (unsigned int i = 0; i < qdepth; i++) {
		       	IssueTask(events, t);
		}
	} else {
	       	IssueTask(events, t);
	}
	uint64_t last_task_time = 0;
	uint64_t ia_time_sum = 0;
	uint64_t ia_time_count = 0;
	while (t < SIMULATION_TIME) {
		if (0 == events.size()) {
			break;
		}
		Event * const e = events.front();
		std::pop_heap(events.begin(), events.end(), cmp);
	       	events.pop_back();
		t = e->t;
		switch (e->type) {
			case EvTyIssueTask:
				// std::cout << "IssueTask " << t << std::endl;
				if (!b2b) {
				       	const uint64_t ia_time = ia_time_distr(generator);
				       	IssueTask(events, t + ia_time);
				}
			       	ia_time_sum += t - last_task_time;
			       	ia_time_count++;
				{
				       	Task * const task = new Task(t);
					server.Queue(events, t, task);
				}
				last_task_time = t;
				break;
			case EvTyFinishTask:
				// std::cout << "FinishIO " << t << std::endl;
			       	server.UnQueue(events, t);
				if (b2b) {
				       	IssueTask(events, t);
				}
				break;
			default:
				throw e;
		}
		delete e;
	}
       	std::cout << "avIATime " << double(ia_time_sum) / double(ia_time_count) << std::endl;
	return 0;
}
