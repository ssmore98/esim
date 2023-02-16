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

#define MAX_TASKQ 1024
#define MAX_EVENTS 1024

// random number generator

class Server;
class Generator;

typedef enum {EvTyNone, EvTyStartTask, EvTyEndTask} EventType;

class Event {
	public:
		const uint64_t t;
		const EventType type;
		Event(const uint64_t & p_t, const EventType & p_type): t(p_t), type(p_type) {
		}
		friend bool cmp(const Event * const, const Event * const);
		virtual Server * const GetServer() const = 0;
		virtual Generator * const GetGenerator() const = 0;
		virtual ~Event() {
		}
};

class Events: public std::vector<Event *> {
	public:
	       	void push_back(Event * const e) {
			assert(MAX_EVENTS > std::vector<Event *>::size());
		       	std::vector<Event *>::push_back(e);
	       	}
};

class GeneratorEvent: public Event {
	public:
		Generator * const generator;
		GeneratorEvent(const uint64_t & t, const EventType & type, Generator * const p_generator): Event(t, type), generator(p_generator) {
		}
		Generator * const GetGenerator() const {
			return generator;
		}
		Server * const GetServer() const {
			assert(0);
			return NULL;
		}
};

bool cmp(const Event * const e1, const Event * const e2)  { 
	return e1->t > e2->t;
}

class Generator {
	private:
		static uint16_t index;
	protected:
		Events & events;
	       	static std::default_random_engine generator;
	       	uint64_t ia_time_sum;
	       	uint64_t ia_time_count;
	       	uint64_t last_task_time;
	public:
		Server * const server;
		const uint16_t my_index;
		Generator(Events & p_events, Server * const p_server): events(p_events), ia_time_sum(0), ia_time_count(0), last_task_time(0),
       			server(p_server), my_index(index) {
				index++;
		}
	       	void IssueTask(const uint64_t & t) {
		       	GeneratorEvent * const e = new GeneratorEvent(t, EvTyStartTask, this);
		       	events.push_back(e);
		       	std::push_heap(events.begin(), events.end(), cmp);
	       	}
	       	virtual void StartTask(const uint64_t & t) = 0;
	       	virtual void EndTask(const uint64_t & t) = 0;
		virtual ~Generator() {
			std::cout << "GENERATOR " << my_index << std::endl;
			if (ia_time_count)
			       	std::cout << "\tavIATime " << double(ia_time_sum) / double(ia_time_count) << std::endl;
		}
};

class Task {
	public:
		const uint64_t t;
		const size_t size;
		Server * const server;
		Generator * const generator;
		Task(const uint64_t & p_t, const size_t & p_size, Server * const p_server, Generator * const p_generator):
		       	t(p_t), size(p_size), server(p_server), generator(p_generator) {
		}
};

typedef std::vector<Generator *> Generators;

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

class ServerEvent: public Event {
	public:
		Server * const server;
		ServerEvent(const uint64_t & t, const EventType & type, Server * const p_server): Event(t, type), server(p_server) {
		}
		Server * const GetServer() const {
			return server;
		}
		Generator * const GetGenerator() const {
			assert(0);
			return NULL;
		}
};

class Server {
	private:
		static uint16_t index;
	protected:
	       	static std::default_random_engine generator;
		TaskQ taskq;
		uint64_t n_tasks;
		uint64_t task_time;
		uint64_t qd_sum;
		uint64_t svc_sum;
		uint64_t GetServiceTime(Task * const task) {
		       	const uint64_t retval = llround(service_time_distr(generator)) + (rate ? task->size / rate : 0);
		       	svc_sum += retval;
			return retval;
		}
	public:
	       	std::exponential_distribution<double> service_time_distr;
		const size_t rate;
		const uint16_t my_index;
		Server(const unsigned int & service_time, const size_t & p_rate): n_tasks(0), task_time(0), qd_sum(0), svc_sum(0),
	       		service_time_distr(1.0 / (double)service_time), rate(p_rate), my_index(index)	{
				index++;
		}
		void Queue(Events & events, const uint64_t & t, Task * const task) {
			// std::cout << ">IO " << *io <<std::endl;
			// std::cout << ">IOQ " << ioq;
			assert(MAX_TASKQ > taskq.size());
			taskq.push_back(task);
			qd_sum += taskq.size();
			if (1 == taskq.size()) {
				const uint64_t service_time = GetServiceTime(task);
			       	ServerEvent * const e = new ServerEvent(t + service_time, EvTyEndTask, this);
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
		       	task->generator->EndTask(t);
			delete task;
			taskq.pop_front();
			// std::cout << "<<IOQ " << ioq;
			if (0 < taskq.size()) {
				const uint64_t service_time = GetServiceTime(task);
			       	ServerEvent * const e = new ServerEvent(t + service_time, EvTyEndTask, this);
			       	events.push_back(e);
			       	std::push_heap(events.begin(), events.end(), cmp);
			}
		}
		virtual ~Server() {
			std::cout << "SERVER " << my_index << std::endl;
			std::cout << "\tTasks " << n_tasks << std::endl;
			if (n_tasks) {
			       	std::cout << "\tavLatency " << double(task_time) / double(n_tasks) << std::endl;
			       	std::cout << "\tavServiceTime " << double(svc_sum) / double(n_tasks) << std::endl;
			       	std::cout << "\tavQueueDepth " << double(qd_sum) / double(n_tasks) << std::endl;
			}
		}
};

std::default_random_engine Server::generator(std::chrono::system_clock::now().time_since_epoch().count());

uint16_t Server::index = 0;

typedef std::vector<Server *> Servers;

std::default_random_engine Generator::generator(std::chrono::system_clock::now().time_since_epoch().count());
uint16_t Generator::index = 0;

class RateGenerator : public Generator {
	protected:
	       	std::exponential_distribution<double> ia_time_distr;
	public:
		RateGenerator(Events & events, const unsigned int & ia_time, Server * const server): Generator(events, server),
	       		ia_time_distr(1.0 / (double)ia_time) {
			// std::cout << "CREATE RATE GENERATOR\n";
			IssueTask(0);
		}
	       	void StartTask(const uint64_t & t) {
		       	const uint64_t this_ia_time = llround(ia_time_distr(generator));
		       	IssueTask(t + this_ia_time);
		       	ia_time_sum += t - last_task_time;
		       	ia_time_count++;
		       	Task * const task = new Task(t, 0, server, this);
		       	server->Queue(events, t, task);
		       	last_task_time = t;
		}
	       	void EndTask(const uint64_t & t) {
		}
};

class QueueGenerator : public Generator {
	public:
	       	const unsigned int qdepth;
		QueueGenerator(Events & events, const unsigned int & p_qdepth, Server * const server): Generator(events, server), qdepth(p_qdepth) {
			// std::cout << "CREATE QUEUE GENERATOR\n";
			for (unsigned int i = 0; i < qdepth; i++) {
			       	IssueTask(0);
			}
		}
	       	void StartTask(const uint64_t & t) {
		       	ia_time_sum += t - last_task_time;
		       	ia_time_count++;
		       	Task * const task = new Task(t, 0, server, this);
		       	server->Queue(events, t, task);
		       	last_task_time = t;
		}
	       	void EndTask(const uint64_t & t) {
			StartTask(t);
		}
};

std::ostream & operator<<(std::ostream & o, const yaml_event_type_t  & e) {
	switch (e) {
	       	case YAML_NO_EVENT:
			o << "YAML_NO_EVENT"; break;
	       	case YAML_SCALAR_EVENT:
			o << "YAML_SCALAR_EVENT"; break;
	       	case YAML_SEQUENCE_START_EVENT:
			o << "YAML_SEQUENCE_START_EVENT"; break;
	       	case YAML_SEQUENCE_END_EVENT:
			o << "YAML_SEQUENCE_END_EVENT"; break;
	       	case YAML_MAPPING_START_EVENT:
			o << "YAML_MAPPING_START_EVENT"; break;
	       	case YAML_MAPPING_END_EVENT:
			o << "YAML_MAPPING_END_EVENT"; break;
	       	case YAML_DOCUMENT_START_EVENT:
			o << "YAML_DOCUMENT_START_EVENT"; break;
	       	case YAML_DOCUMENT_END_EVENT:
			o << "YAML_DOCUMENT_END_EVENT"; break;
	       	case YAML_STREAM_START_EVENT:
			o << "YAML_STREAM_START_EVENT"; break;
	       	case YAML_STREAM_END_EVENT:
			o << "YAML_STREAM_END_EVENT"; break;
		default:
			o << int(e) << " xxx\n";
			throw e;
	}
	return o;
}

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

#if 0
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

KeyVal parse(yaml_document_t & document, const yaml_node_t * const node, const uint16_t level = 0) {
	if (node) {
		switch (node->type) {
		       	case YAML_SCALAR_NODE:
				{
					KeyVal retval;
				       	retval.insert(KeyVal::value_type(std::string((const char *)(node->data.scalar.value)), std::string("")));
					std::cout << level << " YAML_SCALAR_NODE: " << node->data.scalar.value << std::endl;
					return retval;
				}
				break;
		       	case YAML_MAPPING_NODE:
				{
					KeyVal retval;
					for (int k = 0; k < (node->data.mapping.pairs.top - node->data.mapping.pairs.start); k++) {
					       	const yaml_node_t * const k_node = yaml_document_get_node(&document, node->data.mapping.pairs.start[k].key);
						std::string key = parse(document, k_node, level + 1).begin()->first;
					       	const yaml_node_t * const v_node = yaml_document_get_node(&document, node->data.mapping.pairs.start[k].value);
						std::string value = parse(document, v_node, level + 1).begin()->first;
					       	retval.insert(KeyVal::value_type(key, value));
					       	std::cout << level << " YAML_MAPPING_NODE: " << key << " " << value << std::endl;
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

Server * const ParseServer(yaml_parser_t & parser, Events & events) {
	yaml_event_t e;
	std::string key;
	std::string value;
	bool expect_key = false;
	unsigned int service_time = 0;
	size_t rate = 0;
	uint16_t in_mapping = 0;
	while (true) {
		if (!yaml_parser_parse(&parser, &e)) {
		       	assert(0);
	       	}
	       	switch (e.type) {
			case YAML_MAPPING_START_EVENT:
				expect_key = true;
				in_mapping += 1;
				// std::cout << in_mapping << std::endl;
				break;
			case YAML_MAPPING_END_EVENT:
				assert(in_mapping);
				in_mapping--;
				// std::cout << in_mapping << std::endl;
			       	if (1 > service_time) {
				       	std::cerr << "Service time should be greater than zero." << std::endl;
				       	exit(1);
			       	}
			       	yaml_event_delete(&e);
				return new Server(service_time, rate);
			case YAML_SCALAR_EVENT:
				if (e.data.scalar.value) {
				       	// std::cout << e.data.scalar.value << " " << e.data.scalar.style << std::endl;
				        if (expect_key) {
						key = (char *)e.data.scalar.value;
						// std::cout << "KEY " << key <<std::endl;
						expect_key = false;
					} else {
						value = (char *)e.data.scalar.value;
						// std::cout << "VALUE " << value <<std::endl;
						expect_key = true;
					       	if (!strcasecmp("service_time", key.c_str())) {
						       	service_time = std::stoul(value);
					       	} else if (!strcasecmp("rate", key.c_str())) {
						       	rate = std::stoul(value);
					       	} else {
						       	assert(0);
						}
					}
				}
				break;		       	
			default:
			       	std::cout << e.type <<std::endl;
			       	assert(0);
	       	}
		yaml_event_delete(&e);
	}
}

Generator * const ParseGenerator(yaml_parser_t & parser, Events & events, Server * const server) {
	yaml_event_t e;
	std::string key;
	std::string value;
	bool expect_key = false;
	unsigned int qdepth = 0;
	bool b2b = false;
	unsigned int ia_time = 0;
	uint16_t in_mapping = 0;
	while (true) {
		if (!yaml_parser_parse(&parser, &e)) {
		       	assert(0);
	       	}
	       	switch (e.type) {
			case YAML_MAPPING_START_EVENT:
				// std::cout << e.data.mapping_start.style << std::endl;
				expect_key = true;
				in_mapping += 1;
				// std::cout << in_mapping << std::endl;
				break;
			case YAML_MAPPING_END_EVENT:
				assert(in_mapping);
				in_mapping--;
				// std::cout << in_mapping << std::endl;
				if (!in_mapping) {
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
				       	yaml_event_delete(&e);
				       	if (b2b) 
						return new QueueGenerator(events, qdepth, server);
				       	else 
						return new RateGenerator(events, ia_time, server);
			       	}
				break;
			case YAML_SCALAR_EVENT:
				if (e.data.scalar.value) {
				       	// std::cout << e.data.scalar.value << " " << e.data.scalar.style << std::endl;
				        if (expect_key) {
						key = (char *)e.data.scalar.value;
						// std::cout << "KEY " << key <<std::endl;
						expect_key = false;
					} else {
						value = (char *)e.data.scalar.value;
						// std::cout << "VALUE " << value <<std::endl;
						expect_key = true;
					       	if (!strcasecmp("queue_depth", key.c_str())) {
						       	b2b = true;
						       	qdepth = std::stoul(value);
					       	} else if (!strcasecmp("ia_time", key.c_str())) {
						       	b2b = false;
						       	ia_time = std::stoul(value);
					       	} else {
						       	assert(0);
						}
					}
				}
				break;
		       	default:
			       	std::cout << e.type <<std::endl;
			       	assert(0);
	       	}
		yaml_event_delete(&e);
       	}
}

int main(int argc, char **argv) {

	Events events;
	Servers servers;
	Generators generators;
	uint64_t simulation_time = 0;

	FILE * const config_fp = fopen("config.yaml", "r");
	assert(config_fp);
	yaml_parser_t parser;
	yaml_parser_initialize(&parser);
	yaml_parser_set_input_file(&parser, config_fp);
	yaml_event_t e;
	std::string key;
	bool expect_value = false;
	while (true) {
		if (!yaml_parser_parse(&parser, &e)) {
		       	assert(0);
	       	}
		if (e.type == YAML_STREAM_END_EVENT) break;
		switch (e.type) {
			case YAML_SCALAR_EVENT:
				if (e.data.scalar.value) {
				       	// std::cout << e.data.scalar.value << " " << e.data.scalar.style << std::endl;
				       	if (!strcasecmp("generator", (char *)e.data.scalar.value)) {
						if (servers.size()) generators.push_back(ParseGenerator(parser, events, *(servers.begin())));
						else        assert(0);
				       	} else if (!strcasecmp("server", (char *)e.data.scalar.value)) {
					       	servers.push_back(ParseServer(parser, events));
				       	} else if (expect_value) {
						if (!strcasecmp("simulation_time", key.c_str())) {
						       	simulation_time = std::stoull((char *)e.data.scalar.value);
						} else {
						       	assert(0);
						}
						expect_value = false;
					} else {
					       	key = (char *)e.data.scalar.value;
						expect_value = true;
					}

				}
				break;
			case YAML_MAPPING_START_EVENT:
			case YAML_MAPPING_END_EVENT:
			case YAML_DOCUMENT_START_EVENT:
			case YAML_DOCUMENT_END_EVENT:
			case YAML_STREAM_START_EVENT:
				break;
		       	case YAML_NO_EVENT:
				assert(0);
			default:
			       	std::cout << e.type <<std::endl;
				assert(0);
		}
		yaml_event_delete(&e);
	}
	yaml_parser_delete(&parser);
	fclose(config_fp);

       	if (1 > simulation_time) {
	       	std::cerr << "Simulation time should be greater than zero." << std::endl;
	       	exit(1);
       	}

	uint64_t t = 0;
	while (t < simulation_time) {
		if (0 == events.size()) {
			std::cout << "Empty event heap.\n";
			break;
		}
		Event * const e = events.front();
		std::pop_heap(events.begin(), events.end(), cmp);
	       	events.pop_back();
		t = e->t;
		switch (e->type) {
			case EvTyStartTask:
				// std::cout << "StartTask " << t << std::endl;
				e->GetGenerator()->StartTask(t);
				break;
			case EvTyEndTask:
				// std::cout << "EndTask " << t << std::endl;
			       	e->GetServer()->UnQueue(events, t);
				break;
			default:
				throw e;
		}
		delete e;
	}
	for (Generators::const_iterator generator = generators.begin(); generator != generators.end(); generator++) {
	       	delete *generator;
	}
	for (Servers::const_iterator server = servers.begin(); server != servers.end(); server++) {
	       	delete *server;
	}
	return 0;
}
