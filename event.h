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
class Controller;
class Task;

typedef enum {EvTyNone, EvTyRateGenNextTask, EvTyServDiskEnd, EvTyIOMEnd, EvTyPortEnd, EvTyCtrlEnd, EvTyPortFinProc, EvTyIOMFinProc} EventType;

class Event {
	public:
		const uint64_t t;
		const EventType type;
		Event(const uint64_t & p_t, const EventType & p_type);
		friend bool cmp(const Event * const, const Event * const);
		virtual Server * const GetServer() const = 0;
		virtual Generator * const GetGenerator() const = 0;
		virtual Controller * const GetController() const = 0;
		virtual ~Event();
		virtual void print(std::ostream & o) const;
};

class Events: public std::vector<Event *> {
	public:
	       	void push_back(Event * const e);
	       	friend std::ostream & operator<<(std::ostream & o, const Events & events);
};

class GeneratorEvent: public Event {
	public:
		Generator * const generator;
		GeneratorEvent(const uint64_t & t, const EventType & type, Generator * const p_generator);
		virtual Generator * const GetGenerator() const;
		virtual Server * const GetServer() const;
		virtual Controller * const GetController() const;
		virtual void print(std::ostream & o) const;
};

class ServerEvent: public Event {
	public:
		Server * const server;
		Task   * const task;
		ServerEvent(const uint64_t & t, const EventType & type, Server * const p_server, Task * const p_task);
		virtual Server * const GetServer() const;
		virtual Generator * const GetGenerator() const;
		virtual Controller * const GetController() const;
		virtual void print(std::ostream & o) const;
};

typedef std::set<ServerEvent *> ServerEvents;

bool cmp(const Event * const e1, const Event * const e2);

class ControllerEvent: public Event {
	public:
		Controller * const controller;
		Task   * const task;
		ControllerEvent(const uint64_t & t, const EventType & type, Controller * const p_Controller, Task * const p_task);
		virtual Server * const GetServer() const;
		virtual Generator * const GetGenerator() const;
		virtual Controller * const GetController() const;
		virtual void print(std::ostream & o) const;
};

typedef std::set<ServerEvent *> ServerEvents;


#endif // EVENT_H
