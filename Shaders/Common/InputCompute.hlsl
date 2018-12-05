#ifndef INPUT_COMPUTES
# define INPUT_COMPUTES

struct	ComputeInput
{
	[[vk::location(0)]] float3 groupThreadId : SV_GroupThreadID;
	[[vk::location(1)]] float3 groupId : SV_GroupID;
	[[vk::location(2)]] float3 groupIndex : SV_GroupIndex;
};

#endif
