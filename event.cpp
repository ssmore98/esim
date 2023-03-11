#ifndef EVENT_CPP
#define EVENT_CPP

#include "event.h"
#include "server.h"
#include "generator.h"

Event::Event(const uint64_t & p_t, const EventType & p_type): t(p_t), type(p_type) {
}

Event::~Event() {
}

void Events::push_back(Event * const e) {
	assert(MAX_EVENTS > std::vector<Event *>::size());
       	std::vector<Event *>::push_back(e);
}

GeneratorEvent::GeneratorEvent(const uint64_t & t, const EventType & type, Generator * const p_generator): Event(t, type), generator(p_generator) {
}

Generator * const GeneratorEvent::GetGenerator() const {
	return generator;
}

Server * const GeneratorEvent::GetServer() const {
	assert(0);
	return NULL;
}

bool cmp(const Event * const e1, const Event * const e2)  { 
	// std::cout << e1 << " " << e2 << std::endl;
	return e1->t > e2->t;
}

ServerEvent::ServerEvent(const uint64_t & t, const EventType & type, Server * const p_server): Event(t, type), server(p_server) {
}

Server * const ServerEvent::GetServer() const {
	return server;
}
Generator * const ServerEvent::GetGenerator() const {
	assert(0);
	return NULL;
}

std::ostream & operator<<(std::ostream & o, const EventType & type) {
	switch (type) {
	       	case EvTyNone:
			o << "None";
			break;
		case EvTyRateGenNextTask:
			o << "Rate Generator Next Task";
			break;
		case EvTyServDiskEnd:
			o << "Server Disk End";
			break;
		default:
			assert(0);
	}
	return o;
}

void Event::print(std::ostream & o) {
	o << "t=" << t << ' ' << type << std::endl;
}

void ServerEvent::print(std::ostream & o) {
	o << "Server: " << server->name << ' ';
	Event::print(o);
}

void GeneratorEvent::print(std::ostream & o) {
	o << "Generator: " << generator->name << ' ';
	Event::print(o);
}

#endif // EVENT_CPP
