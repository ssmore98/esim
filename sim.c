#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <chrono>
#include <random>
#include <cassert>
#include <cstring>
#include <map>
#include <yaml.h>

// #define SERVICE_TIME 15
// #define IA_TIME 20

// random number generator
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

class Server;

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

class ClientTask: public Task {
	public:
		Server * const server;
		ClientTask(const uint64_t & t, Server * const p_server): Task(t), server(p_server) {
		}
};

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
		void Queue(Events & events, const uint64_t & t, ClientTask * const task) {
		}
};

typedef std::vector<Server *> Servers;

class Client: public Server {
	protected:
		Servers controllers;
	public:
		Client(const unsigned int & service_time, Servers & p_controllers): Server(service_time), controllers(p_controllers) {
		}
};

void IssueTask(Events & events, const uint64_t & t) {
       	Event * const e = new Event(t, EvTyIssueTask);
       	events.push_back(e);
       	std::push_heap(events.begin(), events.end(), cmp);
}

std::ostream & operator<<(std::ostream & o, const yaml_node_type_t  & node_type) {
	switch (node_type) {
	       	case YAML_NO_NODE:
			o << "YAML_NO_NODE"; break;
	       	case YAML_SCALAR_NODE:
			o << "YAML_SCALAR_NODE"; break;
	       	case YAML_SEQUENCE_NODE:
			o << "YAML_SEQUENCE_NODE"; break;
	       	case YAML_MAPPING_NODE:
			o << "YAML_MAPPING_NODE"; break;
		default:
			throw node_type;
	}
	return o;
}

#if 0
std::ostream & operator<<(std::ostream & o, const yaml_sequence_style_t & style) {
	switch (style) {
	       	case YAML_ANY_SEQUENCE_STYLE:
			o << "YAML_ANY_SEQUENCE_STYLE"; break;
	       	case YAML_BLOCK_SEQUENCE_STYLE:
			o << "YAML_BLOCK_SEQUENCE_STYLE"; break;
	       	case YAML_FLOW_SEQUENCE_STYLE:
			o << "YAML_FLOW_SEQUENCE_STYLE"; break;
		default:
			throw style;
	}
	return o;
}

std::ostream & operator<<(std::ostream & o, const yaml_mapping_style_t & style) {
	switch (style) {
	       	case YAML_ANY_MAPPING_STYLE:
			o << "YAML_ANY_MAPPING_STYLE"; break;
	       	case YAML_BLOCK_MAPPING_STYLE:
			o << "YAML_BLOCK_MAPPING_STYLE"; break;
	       	case YAML_FLOW_MAPPING_STYLE:
			o << "YAML_FLOW_MAPPING_STYLE"; break;
		default:
			throw style;
	}
	return o;
}

std::ostream & operator<<(std::ostream & o, const yaml_scalar_style_t & style) {
	switch (style) {
	       	case YAML_ANY_SCALAR_STYLE:
			o << "YAML_ANY_SCALAR_STYLE"; break;
	       	case YAML_PLAIN_SCALAR_STYLE:
			o << "YAML_PLAIN_SCALAR_STYLE"; break;
	       	case YAML_SINGLE_QUOTED_SCALAR_STYLE:
			o << "YAML_SINGLE_QUOTED_SCALAR_STYLE"; break;
	       	case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
			o << "YAML_DOUBLE_QUOTED_SCALAR_STYLE"; break;
	       	case YAML_LITERAL_SCALAR_STYLE:
			o << "YAML_LITERAL_SCALAR_STYLE"; break;
	       	case YAML_FOLDED_SCALAR_STYLE:
			o << "YAML_FOLDED_SCALAR_STYLE"; break;
		default:
			throw style;
	}
	return o;
}

std::ostream & operator<<(std::ostream & o, const yaml_mark_t & mark) {
       	o << "index=" << mark.index << " line=" << mark.line << " column=" << mark.column;;
	return o;
}

std::ostream & operator<<(std::ostream & o, const yaml_node_pair_t * const node) {
	if (node) {
	       	o << "key=" << node->key << " value=" << node->value;
	}
	return o;
}

std::ostream & operator<<(std::ostream & o, const yaml_node_t * const node) {
	if (node) {
	       	// o << node->start_mark << std::endl;
	       	// o << node->type << std::endl;
	       	switch (node->type) {
		       	case YAML_SCALAR_NODE:
			       	// o << node->data.scalar.style << std::endl;
				// o << "value=" << node->data.scalar.value << " length=" <<  node->data.scalar.length; 
				o << node->data.scalar.value;
				break;
		       	case YAML_MAPPING_NODE:
			       	// o << node->data.mapping.style << std::endl;
				{
					for (int k = 0; k < (node->data.mapping.pairs.top - node->data.mapping.pairs.start); k++) {
						{
						       	const yaml_node_t * const c_node = yaml_document_get_node(&document, node->data.mapping.pairs.start[k].key);
						       	o << "KEY " << c_node << std::endl;
						}
						{
						       	const yaml_node_t * const c_node = yaml_document_get_node(&document, node->data.mapping.pairs.start[k].value);
						       	o << "VALUE " << c_node << std::endl;
						}
					}
				}
			       	break;
			case YAML_SEQUENCE_NODE:
			       	// o << node->data.sequence.style << std::endl;
				for (int k = 0; k < (node->data.sequence.items.top - node->data.sequence.items.start); k++) {
				       	const yaml_node_t * const c_node = yaml_document_get_node(&document, node->data.sequence.items.start[k]);
				       	// o << "SEQ " << c_node << std::endl;
				       	o << c_node << " ";
				}
				break;
		       	default:
			       	throw node->type;
		}
	       	// o << node->end_mark << std::endl;
	}
	return o;
}
#endif

typedef std::map<std::string, std::string> KeyVal;

std::ostream & operator<<(std::ostream & o, const KeyVal & node) {
	for (KeyVal::const_iterator i = node.begin(); i != node.end(); i++) {
		o << i->first << " " << i->second << std::endl;
	}
	return o;
}

KeyVal parse(yaml_document_t & document, const yaml_node_t * const node) {
	if (node) {
		switch (node->type) {
		       	case YAML_SCALAR_NODE:
				{
					KeyVal retval;
				       	retval.insert(KeyVal::value_type(std::string((const char *)(node->data.scalar.value)), std::string("")));
					return retval;
				}
				break;
		       	case YAML_MAPPING_NODE:
				{
					KeyVal retval;
					for (int k = 0; k < (node->data.mapping.pairs.top - node->data.mapping.pairs.start); k++) {
					       	const yaml_node_t * const k_node = yaml_document_get_node(&document, node->data.mapping.pairs.start[k].key);
						std::string key = parse(document, k_node).begin()->first;
					       	const yaml_node_t * const v_node = yaml_document_get_node(&document, node->data.mapping.pairs.start[k].value);
						std::string value = parse(document, v_node).begin()->first;
					       	retval.insert(KeyVal::value_type(key, value));
					}
					return retval;
				}
			       	break;
			case YAML_SEQUENCE_NODE:
				break;
		       	default:
			       	throw node->type;
		}
	}
	return KeyVal();
}

KeyVal parse(yaml_document_t & document) {
       	yaml_node_t * const root = yaml_document_get_root_node(&document);
	return parse(document, root);
}

int main(int argc, char **argv) {

	FILE * const config_fp = fopen("config.yaml", "r");
	assert(config_fp);
	yaml_parser_t parser;
	yaml_parser_initialize(&parser);
	yaml_parser_set_input_file(&parser, config_fp);
       	yaml_document_t document;
	yaml_parser_load(&parser, &document);
	const KeyVal config = parse(document);
	yaml_document_delete(&document);
	yaml_parser_delete(&parser);
	fclose(config_fp);

	int opt;
	unsigned int qdepth = 0;
	bool b2b = false;
	unsigned int ia_time = 0;
	unsigned int service_time = 0;
	uint64_t simulation_time = 0;
	for (KeyVal::const_iterator i = config.begin(); i != config.end(); i++) {
		if (!strcasecmp("queue_depth", i->first.c_str())) {
		       	b2b = true;
		       	qdepth = std::stoul(i->second);
			continue;
	       	}
		if (!strcasecmp("ia_time", i->first.c_str())) {
		       	b2b = false;
		       	ia_time = std::stoul(i->second);
			continue;
	       	}
		if (!strcasecmp("service_time", i->first.c_str())) {
		       	service_time = std::stoul(i->second);
			continue;
	       	}
		if (!strcasecmp("simulation_time", i->first.c_str())) {
		       	simulation_time = std::stoull(i->second);
			continue;
	       	}
	}

	if (b2b) {
	       	if (1 > qdepth) {
			std::cerr << "Queue depth should be greater than zero." << std::endl;
			exit(1);
	       	}
	} else {
	       	if (1 > ia_time) {
			std::cerr << "Inter-arrival time should be greater than zero." << std::endl;
			exit(1);
	       	}
	}
       	if (1 > service_time) {
	       	std::cerr << "Service time should be greater than zero." << std::endl;
	       	exit(1);
       	}
       	if (1 > simulation_time) {
	       	std::cerr << "Simulation time should be greater than zero." << std::endl;
	       	exit(1);
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
	while (t < simulation_time) {
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
