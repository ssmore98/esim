#ifndef SIM_CPP
#define SIM_CPP

#include <map>
#include <regex>
#include <yaml.h>

#include "server.h"
#include "event.h"
#include "generator.h"
#include "task.h"
#include "port.h"
#include "controller.h"
#include "raid.h"

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

Controller * const ParseController(yaml_parser_t & parser, const Generators & generators, const HBAs & hbas) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "controller";
	bool expect_key = false;
	uint16_t in_mapping = 0;
	std::vector<std::string> generator_names;
	std::vector<std::string> hba_names;

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
					Generators controller_generators;
					for (std::vector<std::string>::const_iterator i = generator_names.begin();
						       	i != generator_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Generators::const_iterator j = generators.begin(); j != generators.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								controller_generators.insert(*j);
							}
						}
					}
					HBAs controller_hbas;
					for (std::vector<std::string>::const_iterator i = hba_names.begin();
						       	i != hba_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (HBAs::const_iterator j = hbas.begin();
							       	j != hbas.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								controller_hbas.insert(*j);
							}
						}
					}
				       	Controller * const retval = new Controller(name);
					for (Generators::const_iterator i = controller_generators.begin();
							i != controller_generators.end(); i++) {
						*retval = *i;
					}
					for (HBAs::const_iterator i = controller_hbas.begin();
							i != controller_hbas.end(); i++) {
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
					       	} else {
						       	assert(0);
						}
					}
				}
				break;		       	
			case YAML_SEQUENCE_START_EVENT:
			       	if (std::regex_match(key, std::regex("generators", std::regex::icase))) {
					generator_names = GetStrings(parser);
				} else if (std::regex_match(key, std::regex("hbas", std::regex::icase))) {
					hba_names = GetStrings(parser);
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

Shelf * const ParseShelf(yaml_parser_t & parser, const Drives & drives, const IOModules & ioms) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "shelf";
	bool expect_key = false;
	uint16_t in_mapping = 0;
	std::vector<std::string> drive_names;
	std::vector<std::string> iom_names;
	uint16_t slots = 48;
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
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								shelf_drives.insert(*j);
							}
						}
					}
					IOModules shelf_ioms;
					for (std::vector<std::string>::const_iterator i = iom_names.begin();
						       	i != iom_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (IOModules::const_iterator j = ioms.begin();
							       	j != ioms.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								shelf_ioms.insert(*j);
							}
						}
					}
				       	Shelf * const retval = new Shelf(name, slots);
					for (Drives::const_iterator i = shelf_drives.begin();
							i != shelf_drives.end(); i++) {
						*retval = *i;
						**i = retval;
					}
					for (IOModules::const_iterator i = shelf_ioms.begin();
							i != shelf_ioms.end(); i++) {
						*retval = *i;
						**i = retval;
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("slots", std::regex::icase))) {
						       	slots = stoul(value);
					       	} else {
						       	assert(0);
						}
					}
				}
				break;		       	
			case YAML_SEQUENCE_START_EVENT:
			       	if (std::regex_match(key, std::regex("drives", std::regex::icase))) {
					drive_names = GetStrings(parser);
				} else if (std::regex_match(key, std::regex("ioms", std::regex::icase))) {
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

void ParseDrives(yaml_parser_t & parser, Drives & drives) {
	yaml_event_t e;
	std::string key;
	std::string value;
	std::string name = "drive";
	std::string type = "drive";
	bool expect_key = false;
	uint16_t in_mapping = 0;
	uint16_t instances = 1;
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
				for (uint16_t i = 0; i < instances; i++) {
					if (std::regex_match(type, std::regex("SSD_PM1733a", std::regex::icase))) {
					       	drives.insert(new SSD_PM1733a(name + std::to_string(i + 1)));
				       	} else {
					       	assert(0);
					}
				}
				return;
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("type", std::regex::icase))) {
						       	type = value;
						} else if (std::regex_match(key, std::regex("instances", std::regex::icase))) {
						       	instances = std::stoul(value);
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
}

Drive * const ParseDrive(yaml_parser_t & parser) {
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
				if (std::regex_match(key, std::regex("SSD_PM1733a", std::regex::icase))) {
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("type", std::regex::icase))) {
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

RAID * const ParseRAID(yaml_parser_t & parser, const Drives & drives) {
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
				if (std::regex_match(type, std::regex("RAID_0", std::regex::icase))) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								raid_drives.insert(*j);
							}
						}
					}
				       	return new RAID_0(name, raid_drives, stripe_width);
				}
				if (std::regex_match(type, std::regex("RAID_1", std::regex::icase))) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								raid_drives.insert(*j);
							}
						}
					}
				       	return new RAID_1(name, raid_drives);
				}
				if (std::regex_match(type, std::regex("RAID_5", std::regex::icase))) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								raid_drives.insert(*j);
							}
						}
					}
				       	return new RAID_5(name, raid_drives, stripe_width);
				}
				if (std::regex_match(type, std::regex("RAID_4", std::regex::icase))) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								raid_drives.insert(*j);
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
				if (std::regex_match(type, std::regex("RAID_DP", std::regex::icase))) {
					Drives raid_drives;
					for (std::vector<std::string>::const_iterator i = drive_names.begin(); i != drive_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Drives::const_iterator j = drives.begin(); j != drives.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
							       	// std::cout << (*j)->name << std::endl;
								raid_drives.insert(*j);
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
					// std::cout << stripe_width << std::endl;
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("type", std::regex::icase))) {
						       	type = value;
						} else if (std::regex_match(key, std::regex("stripe_width", std::regex::icase))) {
						       	stripe_width = std::stoul(value);
					       	} else {
						       	assert(0);
						}
					}
				}
				break;		       	
			case YAML_SEQUENCE_START_EVENT:
			       	if (std::regex_match(key, std::regex("drives", std::regex::icase))) {
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

Port * const ParsePort(yaml_parser_t & parser, IOModules & ioms) {
	yaml_event_t e;
	std::string key;
	std::string value;
	uint16_t in_mapping = 0;
	bool expect_key = false;
	std::string name = "port";
	std::string iom_name = "iom";
	uint64_t max_iops = 1000000;
	double mbps = 0;
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
					IOModules shelf_ioms;
					for (IOModules::const_iterator j = ioms.begin(); j != ioms.end(); j++) {
					       	if (std::regex_match((*j)->name, std::regex(iom_name, std::regex::icase))) {
						       	Port * const port = new Port(name, uint64_t(1000000 / double(max_iops)), mbps);
							*port = *j;
							return port;
					       	}
				       	}
					assert(0);
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("iom", std::regex::icase))) {
						       	iom_name = value;
						} else if (std::regex_match(key, std::regex("max_iops", std::regex::icase))) {
						       	max_iops = std::stoul(value);
						} else if (std::regex_match(key, std::regex("mbps", std::regex::icase))) {
						       	mbps = std::stod(value);
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

void ParseIOModules(yaml_parser_t & parser, IOModules & ioms) {
	yaml_event_t e;
	std::string key;
	std::string value;
	uint16_t in_mapping = 0;
	bool expect_key = false;
	std::string name = "iom";
	uint64_t max_iops = 1000000;
	double mbps = 0;
	uint16_t instances = 1;
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
					for (uint16_t i = 0; i < instances; i++) {
					       	ioms.insert(new IOModule(name + std::to_string(i + 1),
								       	uint64_t(1000000 / double(max_iops)), mbps));
					}
					return;
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("max_iops", std::regex::icase))) {
						       	max_iops = std::stoul(value);
						} else if (std::regex_match(key, std::regex("mbps", std::regex::icase))) {
						       	mbps = std::stod(value);
						} else if (std::regex_match(key, std::regex("instances", std::regex::icase))) {
						       	instances = std::stoul(value);
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
}

IOModule * const ParseIOModule(yaml_parser_t & parser) {
	yaml_event_t e;
	std::string key;
	std::string value;
	uint16_t in_mapping = 0;
	bool expect_key = false;
	std::string name = "iom";
	uint64_t max_iops = 1000000;
	double mbps = 0;
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
					return new IOModule(name, uint64_t(1000000 / double(max_iops)), mbps);
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("max_iops", std::regex::icase))) {
						       	max_iops = std::stoul(value);
						} else if (std::regex_match(key, std::regex("mbps", std::regex::icase))) {
						       	mbps = std::stod(value);
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

HBA * const ParseHBA(yaml_parser_t & parser, Ports & ports) {
	yaml_event_t e;
	std::vector<std::string> port_names;
	std::string name = "hba";
	uint16_t in_mapping = 0;
	bool expect_key = false;
	std::string key;
	std::string value;
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
					       	} else {
							std::cout << key << ' ' << value << std::endl;
						       	assert(0);
						}
					}
				}
				break;
			case YAML_SEQUENCE_START_EVENT:
			       	if (std::regex_match(key, std::regex("ports", std::regex::icase))) {
				       	port_names = GetStrings(parser);
				} else {
				       	assert(0);
				}
			       	expect_key = true;
				break;
			case YAML_MAPPING_END_EVENT:
				{
				       	HBA * const hba = new HBA(name);
					for (std::vector<std::string>::const_iterator i = port_names.begin(); i != port_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Ports::const_iterator j = ports.begin(); j != ports.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								*hba = *j;
							}
						}
					}
				       	yaml_event_delete(&e);
				       	return hba;
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

Filer * const ParseFiler(yaml_parser_t & parser, Controllers & controllers) {
	yaml_event_t e;
	std::vector<std::string> ctrl_names;
	std::string name = "filer";
	uint16_t in_mapping = 0;
	bool expect_key = false;
	std::string key;
	std::string value;
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
						if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
					       	} else {
						       	assert(0);
						}
					}
				}
				break;
			case YAML_SEQUENCE_START_EVENT:
			       	if (std::regex_match(key, std::regex("controllers", std::regex::icase))) {
				       	ctrl_names = GetStrings(parser);
				} else {
				       	assert(0);
				}
			       	expect_key = true;
				break;
			case YAML_MAPPING_END_EVENT:
				{
				       	Filer * const filer = new Filer(name);
					for (std::vector<std::string>::const_iterator i = ctrl_names.begin(); i != ctrl_names.end(); i++) {
						// std::cout << *i << std::endl;
						for (Controllers::const_iterator j = controllers.begin(); j != controllers.end(); j++) {
							if (std::regex_match((*j)->name, std::regex(*i, std::regex::icase))) {
								*filer = *j;
							}
						}
					}
				       	yaml_event_delete(&e);
				       	return filer;
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

Generator * const ParseGenerator(yaml_parser_t & parser, RAIDs & raids) {
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
						if (std::regex_match((*raid)->name, std::regex(raid_name, std::regex::icase))) {
						       	if (b2b) 
								return new QueueGenerator(name, size, percent_read, percent_random,
											       	qdepth, *raid);
						       	else 
								return new RateGenerator(name, size,
									       	percent_read, percent_random,
									       	ia_time, *raid);
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
						if (std::regex_match(key, std::regex("queue_depth", std::regex::icase))) {
						       	b2b = true;
						       	qdepth = std::stoul(value);
						} else if (std::regex_match(key, std::regex("ia_time", std::regex::icase))) {
						       	b2b = false;
						       	ia_time = std::stoul(value);
						} else if (std::regex_match(key, std::regex("percent_read", std::regex::icase))) {
						       	percent_read = std::stoul(value);
						} else if (std::regex_match(key, std::regex("percent_random", std::regex::icase))) {
						       	percent_random = std::stoul(value);
						} else if (std::regex_match(key, std::regex("size", std::regex::icase))) {
						       	size = std::stoul(value);
						} else if (std::regex_match(key, std::regex("name", std::regex::icase))) {
						       	name = value;
						} else if (std::regex_match(key, std::regex("raid", std::regex::icase))) {
						       	raid_name = value;
						} else if (std::regex_match(key, std::regex("iops", std::regex::icase))) {
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
	assert(0);
	return NULL;
}

int main(int argc, char **argv) {

	std::string config_filename("config.yaml");

	if (1 < argc) {
		config_filename = argv[1];
	}

	uint64_t t = 0;
	Events events;
	RAIDs raids;
	Drives drives;
	IOModules ioms;
	Ports ports;
	Controllers controllers;
	Generators generators;
	Shelves shelves;
	Filers filers;
	HBAs hbas;
	uint64_t simulation_time = 0;

	FILE * const config_fp = fopen(config_filename.c_str(), "r");
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
						generators.insert(ParseGenerator(parser, raids));
				       	} else if (!strcasecmp("raid", (char *)e.data.scalar.value)) {
					       	raids.insert(ParseRAID(parser, drives));
				       	} else if (!strcasecmp("drive", (char *)e.data.scalar.value)) {
					       	drives.insert(ParseDrive(parser));
				       	} else if (!strcasecmp("drives", (char *)e.data.scalar.value)) {
					       	ParseDrives(parser, drives);
				       	} else if (!strcasecmp("shelf", (char *)e.data.scalar.value)) {
					       	shelves.insert(ParseShelf(parser, drives, ioms));
				       	} else if (!strcasecmp("iom", (char *)e.data.scalar.value)) {
					       	ioms.insert(ParseIOModule(parser));
				       	} else if (!strcasecmp("ioms", (char *)e.data.scalar.value)) {
					       	ParseIOModules(parser, ioms);
				       	} else if (!strcasecmp("port", (char *)e.data.scalar.value)) {
					       	ports.insert(ParsePort(parser, ioms));
				       	} else if (!strcasecmp("controller", (char *)e.data.scalar.value)) {
					       	controllers.insert(ParseController(parser, generators, hbas));
				       	} else if (!strcasecmp("filer", (char *)e.data.scalar.value)) {
					       	filers.insert(ParseFiler(parser, controllers));
				       	} else if (!strcasecmp("hba", (char *)e.data.scalar.value)) {
					       	hbas.insert(ParseHBA(parser, ports));
				       	} else if (expect_value) {
						if (!strcasecmp("simulation_time", key.c_str())) {
						       	simulation_time = std::stoull((char *)e.data.scalar.value);
						} else {
							std::cout << e.data.scalar.value << std::endl;
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

	/*
	for (Filers::const_iterator filer = filers.begin(); filer != filers.end(); filer++) {
		(*filer)->PrintConfig(std::cout, "");
	}
	for (Shelves::const_iterator shelf = shelves.begin(); shelf != shelves.end(); shelf++) {
		(*shelf)->PrintConfig(std::cout, "");
	}
	*/

       	if (1 > simulation_time) {
	       	std::cerr << "Simulation time should be greater than zero." << std::endl;
	       	exit(1);
       	}

	for (Controllers::iterator i = controllers.begin(); i != controllers.end(); i++) {
		(*i)->Begin(events, t);
	}

	while (t < simulation_time) {
		if (0 == events.size()) {
			std::cout << "Empty event heap.\n";
			exit(0);
			break;
		}
		Event * const e = events.front();
		std::pop_heap(events.begin(), events.end(), cmp);
	       	events.pop_back();
		t = e->t;
		// e->print(std::cout);
		// std::cout << " " << events << std::endl;
		switch (e->type) {
		       	case EvTyIOMFinProc:
				{
				       	ServerEvents ses = e->GetServer()->Start(t);
					// std::cout << e->GetServer()->name << std::endl;
					for (ServerEvents::iterator i = ses.begin(); i != ses.end(); i++) {
					       	events.push_back(*i);
					       	std::push_heap(events.begin(), events.end(), cmp);
					}
				}
				break;
		       	case EvTyPortFinProc:
				{
				       	ServerEvents ses = e->GetServer()->Start(t);
					for (ServerEvents::iterator i = ses.begin(); i != ses.end(); i++) {
					       	events.push_back(*i);
					       	std::push_heap(events.begin(), events.end(), cmp);
					}
				}
				break;
		       	case EvTyRateGenNextTask:
				{
				       	Task * const task = e->GetGenerator()->NextTask(events, t);
					assert(task);
					e->GetGenerator()->GetController()->ScheduleTask(e->GetGenerator()->raid, task, events, t);
				}
				break;
		       	case EvTyServDiskEnd:
				{
				       	std::pair<Task *, Event *> finish_data = e->GetServer()->Finish(t, NULL);
					Task * const task = finish_data.first;
					assert(task);
					if (finish_data.second) {
					       	events.push_back(finish_data.second);
					       	std::push_heap(events.begin(), events.end(), cmp);
					}
					for (IOModules::iterator iom = ioms.begin(); iom != ioms.end(); iom++) {
						if (task->SERVERS().end() != task->SERVERS().find(*iom)) {
						       	ServerEvent * new_event = new ServerEvent(t, EvTyIOMEnd, *iom, task);
						       	events.push_back(new_event);
						       	std::push_heap(events.begin(), events.end(), cmp);
							break;
						}
					}
				}
				break;
		       	case EvTyIOMEnd:
				{
					std::pair<Task *, Event *> finish_data = e->GetServer()->Finish(t, ((ServerEvent *)e)->task);
					Task * const task = finish_data.first;
					assert(task);
					if (finish_data.second) {
					       	events.push_back(finish_data.second);
					       	std::push_heap(events.begin(), events.end(), cmp);
					}
					for (Ports::iterator port = ports.begin(); port != ports.end(); port++) {
					       	if (task->SERVERS().end() != task->SERVERS().find(*port)) {
						       	ServerEvent * new_event = new ServerEvent(t, EvTyPortEnd, *port, task);
						       	events.push_back(new_event);
						       	std::push_heap(events.begin(), events.end(), cmp);
							break;
						}
					}
				}
				break;
		       	case EvTyPortEnd:
				{
					std::pair<Task *, Event *> finish_data = e->GetServer()->Finish(t, ((ServerEvent *)e)->task);
					Task * const task = finish_data.first;
					assert(task);
					if (finish_data.second) {
					       	events.push_back(finish_data.second);
					       	std::push_heap(events.begin(), events.end(), cmp);
					}
					bool done = false;
					for (Controllers::iterator ctrl = controllers.begin();
							ctrl != controllers.end(); ctrl++) {
					       	for (HBAs::iterator hba = (*ctrl)->HBAS().begin();
							       	hba != (*ctrl)->HBAS().end(); hba++) {
						       	for (Ports::iterator port = (*hba)->PORTS().begin(); port != (*hba)->PORTS().end(); port++) {
								if (task->SERVERS().end() != task->SERVERS().find(*port)) {
								       	ControllerEvent * new_event = new ControllerEvent(t, EvTyCtrlEnd, *ctrl, task);
								       	events.push_back(new_event);
								       	std::push_heap(events.begin(), events.end(), cmp);
								       	done = true;
								       	break;
								}
							}
							if (done) break;
						}
					       	if (done) break;
					}
				}
				break;
		       	case EvTyCtrlEnd:
				{
				       	Task *task = e->GetController()->EndTask(t, ((ControllerEvent *)e)->task);
				       	if (task) {
					       	// std::cout << task << std::endl;
						Generator * const generator = task->generator;
					       	RAID * const raid = generator->raid;
					       	Tasks tasks = raid->Finish(t, task);
					       	// std::cout << tasks << std::endl;
						for (Tasks::iterator i = tasks.begin(); i != tasks.end(); i++) {
						       	Task * const gtask = (*i)->generator->EndTask(*i, t);
						       	if (gtask) {
							       	e->GetController()->ScheduleTask(gtask->generator->raid,
									       	gtask, events, t);
						       	}
					       	}
				       	}
			       	}
				break;
			default:
				throw e;
		}
		delete e;
	}
       	drives.print(std::cout, t);
       	ioms.print(std::cout, t);
       	ports.print(std::cout, t);
       	controllers.print(std::cout, t);
       	filers.print(std::cout, t);
	exit(0);
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
	for (HBAs::const_iterator hba = hbas.begin(); hba != hbas.end(); hba++) {
		(*hba)->print(std::cout, t);
	       	delete *hba;
	}
	for (Drives::const_iterator drive = drives.begin(); drive != drives.end(); drive++) {
		(*drive)->print(std::cout, t);
	       	// delete *drive;
	}
	for (IOModules::const_iterator iom = ioms.begin(); iom != ioms.end(); iom++) {
		// (*iom)->print(std::cout, t);
	       	delete *iom;
	}
	for (Ports::const_iterator port = ports.begin(); port != ports.end(); port++) {
		// (*port)->print(std::cout, t);
	       	delete *port;
	}
	for (Controllers::const_iterator cntlr = controllers.begin(); cntlr != controllers.end(); cntlr++) {
		// (*cntlr)->print(std::cout, t);
	       	delete *cntlr;
	}
	for (Filers::const_iterator filer = filers.begin(); filer != filers.end(); filer++) {
		// (*filer)->print(std::cout, t);
	       	delete *filer;
	}
	return 0;
}

#endif // SIM_CPP
