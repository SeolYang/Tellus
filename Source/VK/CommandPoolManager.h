#pragma once
#include <Core/Core.h>

namespace sy
{
	namespace vk
	{
		class VulkanContext;
		class CommandPool;
		class CommandBuffer;
		class FrameTracker;
		class CommandPoolManager final : public NonCopyable
		{
		public:
			CommandPoolManager(const VulkanContext& vulkanContext, const FrameTracker& frameTracker);
			virtual ~CommandPoolManager() override;

			[[nodiscard]] CommandPool& RequestCommandPool(EQueueType queueType);

			void BeginFrame();
			void EndFrame();

		private:
			const VulkanContext& vulkanContext;
			const FrameTracker& frameTracker;

			std::shared_mutex cmdPoolMutex;
			std::array<std::vector<std::unique_ptr<CommandPool>>, NumMaxInFlightFrames> cmdPools;

		};
	}
}