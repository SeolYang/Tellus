#include <Core.h>
#include <VK/CommandPool.h>
#include <VK/CommandBuffer.h>
#include <VK/VulkanInstance.h>

namespace sy
{
	CommandPool::CommandPool(const VulkanInstance& vulkanInstance, const EQueueType queueType) :
		VulkanWrapper<VkCommandPool>("Unknown Queue", vulkanInstance, VK_DESTROY_LAMBDA_SIGNATURE(VkCommandPool)
		{
			vkDestroyCommandPool(vulkanInstance.GetLogicalDevice(), handle, nullptr);
		}),
		queueType(queueType)
	{
		switch (queueType)
		{
		case EQueueType::Graphics:
			SetName("Graphics Queue");
			break;
		case EQueueType::Compute:
			SetName("Compute Queue");
			break;
		case EQueueType::Transfer:
			SetName("Transfer Queue");
			break;
		case EQueueType::Present:
			SetName("Present Queue");
			break;
		}

		const VkCommandPoolCreateInfo cmdPoolCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = vulkanInstance.GetQueueFamilyIndex(queueType)
		};

		const size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
		spdlog::trace("Creating cmd pool for thread {}.", threadId);
		VK_ASSERT(vkCreateCommandPool(vulkanInstance.GetLogicalDevice(), &cmdPoolCreateInfo, nullptr, &handle), "Failed to create vulkan command queue from create info.");
	}

	CommandBuffer& CommandPool::RequestCommandBuffer(const std::string_view name, const Fence& renderFence)
	{
		for (const auto& cmdBufferPtr : cmdBuffers)
		{
			if (cmdBufferPtr->IsReadyToUse())
			{
				cmdBufferPtr->SetName(name);
				cmdBufferPtr->Begin(renderFence);
				return *cmdBufferPtr;
			}
		}

		const size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
		spdlog::trace("Creating cmd buffer for thread {}.", threadId);
		cmdBuffers.emplace_back(std::make_unique<CommandBuffer>(name, vulkanInstance, *this));
		const auto& newCmdBuffer = cmdBuffers.back();
		newCmdBuffer->Begin(renderFence);
		return *newCmdBuffer;
	}
}
