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
		case EvTyHBAFinProc:
			o << "HBA Finish Processing";
			break;
		case EvTyIOMFinProc:
			o << "IO Module Finish Processing";
			break;
		default:
			assert(0);
	}
	return o;
}

void Event::print(std::ostream & o)  const{
	o << "t=" << t << ' ' << type;
}

void ServerEvent::print(std::ostream & o) const {
	Event::print(o);
	o << "Server: " << server->name << ' ';
}

void GeneratorEvent::print(std::ostream & o) const {
	Event::print(o);
	o << "Generator: " << generator->name << ' ';
}

std::ostream & operator<<(std::ostream & o, const Event & event) {
       	event.print(o);
	return o;
}

std::ostream & operator<<(std::ostream & o, const Events & events) {
	o << "(";
	for (Events::const_iterator i = events.begin(); i != events.end(); i++) {
		(*i)->print(o);
	       	o << ",";
	}
	o << ")";
	return o;
}

#endif // EVENT_CPP
