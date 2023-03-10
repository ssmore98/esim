#ifndef EVENT_H
#define EVENT_H

#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>

#define MAX_EVENTS (64 * 1024)

class Server;
class Generator;

typedef enum {EvTyNone, EvTyRateGenNextTask, EvTyServDiskEnd, EvTyHBAFinProc, EvTyIOMFinProc} EventType;

class Event {
	public:
		const uint64_t t;
		const EventType type;
		Event(const uint64_t & p_t, const EventType & p_type);
		friend bool cmp(const Event * const, const Event * const);
		virtual Server * const GetServer() const = 0;
		virtual Generator * const GetGenerator() const = 0;
		virtual ~Event();
		virtual void print(std::ostream & o);
};

class Events: public std::vector<Event *> {
	public:
	       	void push_back(Event * const e);
};

class GeneratorEvent: public Event {
	public:
		Generator * const generator;
		GeneratorEvent(const uint64_t & t, const EventType & type, Generator * const p_generator);
		Generator * const GetGenerator() const;
		Server * const GetServer() const;
		virtual void print(std::ostream & o);
};

class ServerEvent: public Event {
	public:
		Server * const server;
		ServerEvent(const uint64_t & t, const EventType & type, Server * const p_server);
		Server * const GetServer() const;
		Generator * const GetGenerator() const;
		virtual void print(std::ostream & o);
};

typedef std::set<ServerEvent *> ServerEvents;

bool cmp(const Event * const e1, const Event * const e2);

#endif // EVENT_H
