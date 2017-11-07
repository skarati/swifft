#define REPEAT 1000000
#define WARMUP (REPEAT/4)

unsigned long long RDTSC_start_clk, RDTSC_end_clk;
double RDTSC_total_clk;
int RDTSC_MEASURE_ITERATOR;
int SCHED_RET_VAL;

__inline unsigned long long get_Clks(void)
{
	unsigned long long ret_val;
	__asm__ volatile
	(
		"cpuid\n\t\
		rdtsc\n\t\
		mov %%eax,(%0)\n\t\
		mov %%edx,4(%0)"::"rm"(&ret_val):"eax","edx","ebx","ecx"
	);
	return ret_val;
}
#define MEASURE(x) \
	for(RDTSC_MEASURE_ITERATOR=0; RDTSC_MEASURE_ITERATOR< WARMUP; RDTSC_MEASURE_ITERATOR++) \
	{ \
		{x}; \
	}; \
	RDTSC_start_clk = get_Clks(); \
	for (RDTSC_MEASURE_ITERATOR = 0; RDTSC_MEASURE_ITERATOR < REPEAT; RDTSC_MEASURE_ITERATOR++) \
	{ \
		{x}; \
	} \
	RDTSC_end_clk = get_Clks(); \
	RDTSC_total_clk = (double)(RDTSC_end_clk-RDTSC_start_clk)/REPEAT;

