#ifndef SIM_CPP
#define SIM_CPP

#include <map>
#include <yaml.h>

#include "server.h"
#include "event.h"
#include "generator.h"
#include "task.h"

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

Server * const ParseServer(yaml_parser_t & parser, Events & events, const uint64_t & t) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "server";
	bool expect_key = false;
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
			       	yaml_event_delete(&e);
				return new SSD_PM1733a(name, t);
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
					       	if (!strcasecmp("name", key.c_str())) {
						       	name = value;
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

Generator * const ParseGenerator(yaml_parser_t & parser, Events & events, Servers & servers) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "generator";
	std::string server_name = "server";
	bool expect_key = false;
	unsigned int qdepth = 0;
	bool b2b = false;
	unsigned int ia_time = 0;
	uint16_t in_mapping = 0;
	uint16_t percent_read = 0;
	size_t size = 0;
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
					if (1 > size) {
					       	std::cerr << "Size should be greater than zero." << std::endl;
					       	exit(1);
					}
				       	yaml_event_delete(&e);
					for (Servers::const_iterator server = servers.begin(); server != servers.end(); server++) {
						if (!strcasecmp(server_name.c_str(), (*server)->name.c_str())) {
						       	if (b2b) 
								return new QueueGenerator(name, size, percent_read, events, qdepth, *server);
						       	else 
								return new RateGenerator(name, size, percent_read, events, ia_time, *server);
						}
					}
					assert(0);
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
					       	} else if (!strcasecmp("percent_read", key.c_str())) {
						       	percent_read = std::stoul(value);
					       	} else if (!strcasecmp("size", key.c_str())) {
						       	size = std::stoul(value);
					       	} else if (!strcasecmp("name", key.c_str())) {
						       	name = value;
					       	} else if (!strcasecmp("server", key.c_str())) {
						       	server_name = value;
					       	} else if (!strcasecmp("iops", key.c_str())) {
						       	b2b = false;
						       	ia_time = (unsigned int)(1000000.0 / std::stod(value));
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

	uint64_t t = 0;
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
						if (servers.size()) generators.push_back(ParseGenerator(parser, events, servers));
						else        assert(0);
				       	} else if (!strcasecmp("server", (char *)e.data.scalar.value)) {
					       	servers.push_back(ParseServer(parser, events, t));
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

#endif // SIM_CPP
