#ifndef INPUT_COMPUTES
# define INPUT_COMPUTES

struct	ComputeInput
{
	[[vk::location(0)]] uint3	groupThreadId : SV_GroupThreadID;
	[[vk::location(1)]] uint3	dispatchThreadId : SV_DispatchThreadID;
	[[vk::location(2)]] uint3	groupId : SV_GroupID;
	[[vk::location(3)]] uint	groupIndex : SV_GroupIndex;
};

#endif
