#pragma once
#include <Core.h>

namespace sy
{
	class VulkanInstance;
	class CommandPool;
	class Fence;
	class CommandBuffer : public VulkanWrapper<VkCommandBuffer>
	{
	public:
		CommandBuffer(std::string_view name, const VulkanInstance& vulkanInstance, const CommandPool& cmdPool);
		virtual ~CommandBuffer() override = default;

		[[nodiscard]] EQueueType GetQueueType() const { return queueType; }

		void Reset() const;
		void Begin() const;
		void End() const;

		void BeginRendering(const VkRenderingInfo& renderingInfo) const;
		void EndRendering() const;

		void ChangeImageLayout(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t mipLevelCount = 1, uint32_t baseMipLevel = 0, uint32_t arrayLayerCount = 1, uint32_t baseArrayLayer = 0) const;
		void PipelineBarrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, std::span<VkMemoryBarrier> memoryBarriers, std::span<VkBufferMemoryBarrier> bufferMemoryBarriers, std::span<VkImageMemoryBarrier> imageMemoryBarriers) const;

	public:
		static std::pair<VkAccessFlags, VkAccessFlags> QueryOptimalAccessFlagFromImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

	private:
		const EQueueType queueType;

	};

}
