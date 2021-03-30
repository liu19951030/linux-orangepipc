#undef TRACE_SYSTEM
#define TRACE_SYSTEM autohotplug

#if !defined(_TRACE_AUTOHOTPLUG_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_AUTOHOTPLUG_H
#include <linux/tracepoint.h>

/*
 * Tracepoint for cpu_autohotplug.
 */
TRACE_EVENT(autohotplug_operate,

	TP_PROTO(int cpu, char *str),
	TP_ARGS(cpu, str),

	TP_STRUCT__entry(
		__field(int, cpu)
		__array(char, str, 10)
	),

	TP_fast_assign(
		__entry->cpu = cpu;
		strncpy(__entry->str, str, 10);
	),

	TP_printk("cpu%d %s success", __entry->cpu, __entry->str)
);

TRACE_EVENT(autohotplug_under,

	TP_PROTO(int load_level, int count),
	TP_ARGS(load_level, count),

	TP_STRUCT__entry(
		__field(int, load_level)
		__field(int, count)
	),

	TP_fast_assign(
		__entry->load_level = load_level;
		__entry->count  = count;
	),

	TP_printk("under load %d%% count is %d", __entry->load_level, __entry->count)
);

TRACE_EVENT(autohotplug_stable,

	TP_PROTO(int success),
	TP_ARGS(success),

	TP_STRUCT__entry(
		__field(int, success)
	),

	TP_fast_assign(
		__entry->success = success;
	),

	TP_printk("load %s stable", __entry->success == 1 ? "is" : "is not")
);

TRACE_EVENT(autohotplug_try,

	TP_PROTO(char *str),
	TP_ARGS(str),

	TP_STRUCT__entry(
		__array(char, str, 10)
	),

	TP_fast_assign(
		strncpy(__entry->str, str, 10);
	),

	TP_printk("try cpu %s", __entry->str)
);

TRACE_EVENT(autohotplug_notyet,

	TP_PROTO(char *str, int cpu),
	TP_ARGS(str, cpu),

	TP_STRUCT__entry(
		__array(char, str, 10)
		__field(int, cpu)
	),

	TP_fast_assign(
		strncpy(__entry->str, str, 10);
		__entry->cpu = cpu;
	),

	TP_printk("%s cpu%d impossible", __entry->str, __entry->cpu)
);

#ifdef CONFIG_CPU_AUTOHOTPLUG_ROOMAGE
TRACE_EVENT(autohotplug_roomage,

	TP_PROTO(int cluster, char *str, int cur_online, int limit_online),
	TP_ARGS(cluster, str, cur_online, limit_online),

	TP_STRUCT__entry(
		__field(int, cluster)
		__array(char, str, 10)
		__field(int, cur_online)
		__field(int, limit_online)
	),

	TP_fast_assign(
		__entry->cluster = cluster;
		strncpy(__entry->str, str, 10);
		__entry->cur_online = cur_online;
		__entry->limit_online = limit_online;
	),

	TP_printk("cluser%d current online is %d, %s limit is %d", __entry->cluster,
				__entry->cur_online, __entry->str, __entry->limit_online)
);
#endif

#endif /* _TRACE_AUTOHOTPLUG_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
