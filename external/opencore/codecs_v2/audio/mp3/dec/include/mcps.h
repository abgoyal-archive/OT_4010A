
#ifndef MCPS_H
#define MCPS_H


typedef struct {
	long int count_clk1;
	int count_clk2;
	int diff_time;
	int max_diff;
	int max_Frame;
	int startTime;
	int endTime;
	int frameCount;
}MCPSinfo;



static void MCPS_Init(MCPSinfo* info)
{
	info->count_clk1 = 0;
	info->max_diff = 0;
	info->frameCount = 0;
	return;
}

static void MCPS_Start(MCPSinfo* info)
{
	info->frameCount++;
	//info->startTime = clock();
	return;
}

static void MCPS_End(MCPSinfo* info)
{

	//info->endTime = clock();
	info->diff_time = info->endTime - info->startTime;

	info->count_clk1 += info->diff_time;
	info->count_clk2 = (int)(info->count_clk1/info->frameCount);

	if(info->diff_time > info->max_diff)
	{
		info->max_Frame = info->frameCount;
		info->max_diff = info->diff_time;
	}
	return;
}

static int MCPS_Print(MCPSinfo info)
{
	printf("MAX_DIFF : 0x%x at Frame count = %d\n", info.max_diff, info.max_Frame);
	printf("Avg CYCLES : 0x%x\n", info.count_clk2);
	return(info.max_diff);
}

#endif
