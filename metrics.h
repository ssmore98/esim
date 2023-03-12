#ifndef METRICS_H
#define METRICS_H

#include <cinttypes>

#include "task.h"

class Metrics {
	protected:
	       	uint64_t n_tasks;
		uint64_t task_time;
		uint64_t qd_sum;
		uint64_t svc_sum;
		uint64_t sz_sum;
		Task    *task;
	public:
		Metrics();
		const uint64_t & N_TASKS() const;
		const uint64_t & SZ_SUM() const;
		const uint64_t & TASK_TIME() const;
		const uint64_t & SVC_SUM() const;
		const uint64_t & QD_SUM() const;
	       	void StartTask(Task * const ntask, const uint16_t & qd, const uint64_t & svc);
		void EndTask(Task * const task, const uint64_t & xt);
		friend std::ostream & operator<<(std::ostream & o, const Metrics & m);
};

#endif // METRICS_H
