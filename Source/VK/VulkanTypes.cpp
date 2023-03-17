#include <PCH.h>
#include <VK/VulkanTypes.h>
#include <VK/TextureView.h>

namespace sy
{
	namespace vk
	{
		VkRenderingAttachmentInfo DepthAttachmentInfo(const TextureView& depthStencilView, const float depthClearValue,
			const uint8_t stencilClearValue)
		{
			return {
				.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.pNext = nullptr,
				.imageView = depthStencilView.GetNativeHandle(),
				.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue = VkClearValue{
					.depthStencil = VkClearDepthStencilValue{ depthClearValue, stencilClearValue } }
			};
		}
	} // namespace vk
} // namespace sy
