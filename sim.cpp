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

std::vector<std::string> GetStrings(yaml_parser_t & parser) {
	std::vector<std::string> retval;
	yaml_event_t e;
	while (true) {
		if (!yaml_parser_parse(&parser, &e)) {
		       	assert(0);
	       	}
	       	switch (e.type) {
			case YAML_SCALAR_EVENT:
				if (e.data.scalar.value) {
						retval.push_back((char *)e.data.scalar.value);
				}
				break;
			case YAML_SEQUENCE_END_EVENT:
			       	yaml_event_delete(&e);
				return retval;
			default:
			       	std::cout << e.type <<std::endl;
			       	assert(0);
		}
		yaml_event_delete(&e);
	}
	assert(0);
	return retval;
}

Shelf * const ParseShelf(yaml_parser_t & parser, Events & events, const Drives & drives, const IOModules & ioms) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "shelf";
	bool expect_key = false;
	uint16_t in_mapping = 0;
	std::vector<std::string> drive_names;
	std::vector<std::string> iom_names;
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
				{
					Drives shelf_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								shelf_drives.insert(*j);
								break;
							}
						}
					}
					IOModules shelf_ioms;
					for (std::vector<std::string>::const_iterator i = iom_names.begin();
						       	i != iom_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (IOModules::const_iterator j = ioms.begin();
							       	j != ioms.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								shelf_ioms.insert(*j);
								break;
							}
						}
					}
				       	Shelf * const retval = new Shelf(name);
					for (Drives::const_iterator i = shelf_drives.begin();
							i != shelf_drives.end(); i++) {
						*retval = *i;
					}
					for (IOModules::const_iterator i = shelf_ioms.begin();
							i != shelf_ioms.end(); i++) {
						*retval = *i;
					}
					return retval;
				}
				break;
			case YAML_SCALAR_EVENT:
				if (e.data.scalar.value) {
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
			case YAML_SEQUENCE_START_EVENT:
			       	if (!strcasecmp("drives", key.c_str())) {
					drive_names = GetStrings(parser);
				} else if (!strcasecmp("ioms", key.c_str())) {
					iom_names = GetStrings(parser);
				} else {
				       	assert(0);
				}
			       	expect_key = true;
				break;
			default:
			       	std::cout << e.type << expect_key << std::endl;
			       	assert(0);
	       	}
		yaml_event_delete(&e);
	}
	assert(0);
	return NULL;
}

Drive * const ParseDrive(yaml_parser_t & parser, Events & events) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "drive";
	std::string type = "drive";
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
				if (!strcasecmp("SSD_PM1733a", type.c_str())) {
				       	return new SSD_PM1733a(name);
				}
				assert(0);
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
						} else if (!strcasecmp("type", key.c_str())) {
						       	type = value;
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
	assert(0);
	return NULL;
}

RAID * const ParseRAID(yaml_parser_t & parser, Events & events, const Drives & drives) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "server";
	std::string type = "server";
	std::vector<std::string> drive_names;
	bool expect_key = false;
	uint16_t in_mapping = 0;
	size_t stripe_width = 64 * 1024;
	// size_t n_parity_servers = 1;
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
				if (!strcasecmp("RAID_0", type.c_str())) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								raid_drives.insert(*j);
								break;
							}
						}
					}
				       	return new RAID_0(name, raid_drives, stripe_width);
				}
				if (!strcasecmp("RAID_1", type.c_str())) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								raid_drives.insert(*j);
								break;
							}
						}
					}
				       	return new RAID_1(name, raid_drives);
				}
				if (!strcasecmp("RAID_5", type.c_str())) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								raid_drives.insert(*j);
								break;
							}
						}
					}
				       	return new RAID_5(name, raid_drives, stripe_width);
				}
				if (!strcasecmp("RAID_4", type.c_str())) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								raid_drives.insert(*j);
								break;
							}
						}
					}
					Drives data_drives;
					Drives parity_drives;
					size_t count = 0;
					for (Drives::iterator i = raid_drives.begin(); i != raid_drives.end(); i++) {
						if (count < 1) {
						       	parity_drives.insert(*i);
						} else {
						       	data_drives.insert(*i);
						}
						count++;
					}
				       	return new RAID_4(name, data_drives, parity_drives, stripe_width);
				}
				if (!strcasecmp("RAID_DP", type.c_str())) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (!strcasecmp((*j)->name.c_str(), (*i).c_str())) {
								raid_drives.insert(*j);
								break;
							}
						}
					}
					Drives data_drives;
					Drives parity_drives;
					size_t count = 0;
					for (Drives::iterator i = raid_drives.begin(); i != raid_drives.end(); i++) {
						if (count < 2) {
						       	parity_drives.insert(*i);
						} else {
						       	data_drives.insert(*i);
						}
						count++;
					}
				       	return new RAID_DP(name, data_drives, parity_drives, stripe_width);
				}
				assert(0);
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
						} else if (!strcasecmp("type", key.c_str())) {
						       	type = value;
						} else if (!strcasecmp("stripe_width", key.c_str())) {
						       	stripe_width = std::stoul(value);
						// } else if (!strcasecmp("parity_servers", key.c_str())) {
						       	// n_parity_servers = std::stoul(value);
					       	} else {
						       	assert(0);
						}
					}
				}
				break;		       	
			case YAML_SEQUENCE_START_EVENT:
			       	if (!strcasecmp("drives", key.c_str())) {
					drive_names = GetStrings(parser);
				} else {
				       	assert(0);
				}
			       	expect_key = true;
				break;
			default:
			       	std::cout << e.type <<std::endl;
			       	assert(0);
	       	}
		yaml_event_delete(&e);
	}
	assert(0);
	return NULL;
}

IOModule * const ParseIOModule(yaml_parser_t & parser, Events & events) {
	yaml_event_t e;
	std::string key;
	std::string value;
	uint16_t in_mapping = 0;
	bool expect_key = false;
	std::string name = "iom";
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
					return new IOModule(name);
				}
				break;
			case YAML_SCALAR_EVENT:
				if (e.data.scalar.value) {
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
	assert(0);
	return NULL;
}

Generator * const ParseGenerator(yaml_parser_t & parser, Events & events, RAIDs & raids) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "generator";
	std::string raid_name = "raid";
	bool expect_key = false;
	unsigned int qdepth = 0;
	bool b2b = false;
	unsigned int ia_time = 0;
	uint16_t in_mapping = 0;
	uint16_t percent_read = 0;
	uint16_t percent_random = 0;
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
					for (RAIDs::const_iterator raid = raids.begin(); raid != raids.end(); raid++) {
						if (!strcasecmp(raid_name.c_str(), (*raid)->name.c_str())) {
						       	if (b2b) 
								return new QueueGenerator(name, size, percent_read, percent_random, events, qdepth, *raid);
						       	else 
								return new RateGenerator(name, size, percent_read, percent_random, events, ia_time, *raid);
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
					       	} else if (!strcasecmp("percent_random", key.c_str())) {
						       	percent_random = std::stoul(value);
					       	} else if (!strcasecmp("size", key.c_str())) {
						       	size = std::stoul(value);
					       	} else if (!strcasecmp("name", key.c_str())) {
						       	name = value;
					       	} else if (!strcasecmp("raid", key.c_str())) {
						       	raid_name = value;
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
	RAIDs raids;
	Drives drives;
	IOModules ioms;
	Generators generators;
	Shelves shelves;
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
						generators.push_back(ParseGenerator(parser, events, raids));
				       	} else if (!strcasecmp("raid", (char *)e.data.scalar.value)) {
					       	raids.insert(ParseRAID(parser, events, drives));
				       	} else if (!strcasecmp("drive", (char *)e.data.scalar.value)) {
					       	drives.insert(ParseDrive(parser, events));
				       	} else if (!strcasecmp("shelf", (char *)e.data.scalar.value)) {
					       	shelves.insert(ParseShelf(parser, events, drives, ioms));
				       	} else if (!strcasecmp("iom", (char *)e.data.scalar.value)) {
					       	ioms.insert(ParseIOModule(parser, events));
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
	for (RAIDs::const_iterator raid = raids.begin(); raid != raids.end(); raid++) {
		(*raid)->print(std::cout, t);
	       	delete *raid;
	}
	for (Shelves::const_iterator shelf = shelves.begin(); shelf != shelves.end(); shelf++) {
		(*shelf)->print(std::cout, t);
	       	delete *shelf;
	}
	for (Drives::const_iterator drive = drives.begin(); drive != drives.end(); drive++) {
		(*drive)->print(std::cout, t);
	       	delete *drive;
	}
	for (IOModules::const_iterator iom = ioms.begin(); iom != ioms.end(); iom++) {
		(*iom)->print(std::cout, t);
	       	delete *iom;
	}
	return 0;
}

#endif // SIM_CPP
