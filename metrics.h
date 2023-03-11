#ifndef METRICS_H
#define METRICS_H

#include <cinttypes>

class Metrics {
	protected:
	       	uint64_t n_tasks;
		uint64_t task_time;
		uint64_t qd_sum;
		uint64_t svc_sum;
		uint64_t sz_sum;
	public:
		Metrics();
		const uint64_t & N_TASKS() const;
		const uint64_t & SZ_SUM() const;
		const uint64_t & TASK_TIME() const;
		const uint64_t & SVC_SUM() const;
		const uint64_t & QD_SUM() const;
};

#endif // METRICS_H
