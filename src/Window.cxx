module;

#include <iostream> // std::cerr
#include <cstdint>  // std::uint32_t
#include <vector>   // std::vector
#include <memory>   // std::unique_ptr

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

export module Window;

// Data
VkAllocationCallbacks* g_Allocator = nullptr;
VkInstance g_Instance = VK_NULL_HANDLE;
VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
VkDevice g_Device = VK_NULL_HANDLE;
static constexpr std::uint32_t originalQueueFamily{ 4294967295 };
uint32_t g_QueueFamily = 4294967295;
VkQueue g_Queue = VK_NULL_HANDLE;
VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
ImGui_ImplVulkanH_Window g_MainWindowData;
uint32_t g_MinImageCount = 2;
bool g_SwapChainRebuild = false;

void check_vk_result(VkResult err)
{
   if (err == 0)
   {
      return;
   }

   fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);

   if (err < 0)
   {
      abort();
   }
}

bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
{
   for (const VkExtensionProperties& p : properties)
   {
      if (strcmp(p.extensionName, extension) == 0)
      {
         return true;
      }
   }
   return false;
}

VkPhysicalDevice SetupVulkan_SelectPhysicalDevice()
{
   uint32_t gpu_count{};
   VkResult err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
   check_vk_result(err);
   IM_ASSERT(gpu_count > 0);

   ImVector<VkPhysicalDevice> gpus;
   gpus.resize(int(gpu_count));
   err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.Data);
   check_vk_result(err);

   // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
   // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
   // dedicated GPUs) is out of scope of this sample.
   for (VkPhysicalDevice& device : gpus)
   {
      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(device, &properties);
      if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
         return device;
      }
   }

   // Use first GPU (Integrated) is a Discrete one is not available.
   if (gpu_count > 0)
   {
      return gpus[0];
   }
   return VK_NULL_HANDLE;
}

void SetupVulkan(ImVector<const char*> instance_extensions)
{
   VkResult err{};

   // Create Vulkan Instance
   {
      VkInstanceCreateInfo create_info = {};
      create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

      // Enumerate available extensions
      uint32_t properties_count;
      ImVector<VkExtensionProperties> properties;
      vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
      properties.resize(int(properties_count));
      err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
      check_vk_result(err);

      // Enable required extensions
      if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
         instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
      if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
      {
         instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
         create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
      }
#endif

      // Create Vulkan Instance
      create_info.enabledExtensionCount = std::uint32_t(instance_extensions.Size);
      create_info.ppEnabledExtensionNames = instance_extensions.Data;
      err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
      check_vk_result(err);
   }

   // Select Physical Device (GPU)
   g_PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

   // Select graphics queue family
   {
      uint32_t count{};
      vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, nullptr);

      std::vector<VkQueueFamilyProperties> queues{};
      queues.resize(count);

      vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues.data());
      for (uint32_t i = 0; i < count; i++)
      {
         if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
         {
            g_QueueFamily = i;
            break;
         }
      }

      IM_ASSERT(g_QueueFamily != originalQueueFamily);
   }

   // Create Logical Device (with 1 queue)
   {
      ImVector<const char*> device_extensions;
      device_extensions.push_back("VK_KHR_swapchain");

      // Enumerate physical device extension
      uint32_t properties_count{};
      ImVector<VkExtensionProperties> properties;
      vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, nullptr);
      properties.resize(int(properties_count));
      vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr, &properties_count, properties.Data);
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
      {
         device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
      }
#endif
      const float queue_priority[] = { 1.0f };
      VkDeviceQueueCreateInfo queue_info[1] = {};
      queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_info[0].queueFamilyIndex = g_QueueFamily;
      queue_info[0].queueCount = 1;
      queue_info[0].pQueuePriorities = queue_priority;
      VkDeviceCreateInfo create_info = {};
      create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
      create_info.pQueueCreateInfos = queue_info;
      create_info.enabledExtensionCount = uint32_t(device_extensions.Size);
      create_info.ppEnabledExtensionNames = device_extensions.Data;
      err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
      check_vk_result(err);
      vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
   }

   // Create Descriptor Pool
   // The example only requires a single combined image sampler descriptor for the font image and only uses one
   // descriptor set (for that) If you wish to load e.g. additional textures you may need to alter pools sizes.
   {
      VkDescriptorPoolSize pool_sizes[] = {
         { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
      };
      VkDescriptorPoolCreateInfo pool_info = {};
      pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
      pool_info.maxSets = 1;
      pool_info.poolSizeCount = uint32_t(IM_ARRAYSIZE(pool_sizes));
      pool_info.pPoolSizes = pool_sizes;
      err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
      check_vk_result(err);
   }
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
{
   wd->Surface = surface;

   // Check for WSI support
   VkBool32 res{};
   vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
   if (res != VK_TRUE)
   {
      fprintf(stderr, "Error no WSI support on physical device 0\n");
      exit(-1);
   }

   // Select Surface Format
   const VkFormat requestSurfaceImageFormat[]
     = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
   const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
   wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat,
     size_t(IM_ARRAYSIZE(requestSurfaceImageFormat)), requestSurfaceColorSpace);

   // Select Present Mode
#ifdef APP_USE_UNLIMITED_FRAME_RATE
   VkPresentModeKHR present_modes[]
     = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
   VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
   wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
     g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
   // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

   // Create SwapChain, RenderPass, Framebuffer, etc.
   IM_ASSERT(g_MinImageCount >= 2);
   ImGui_ImplVulkanH_CreateOrResizeWindow(
     g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
}

void CleanupVulkan()
{
   vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);
   vkDestroyDevice(g_Device, g_Allocator);
   vkDestroyInstance(g_Instance, g_Allocator);
}

void CleanupVulkanWindow() { ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator); }

void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
   VkResult err;

   VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
   VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
   err = vkAcquireNextImageKHR(
     g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
   if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
   {
      g_SwapChainRebuild = true;
      return;
   }
   check_vk_result(err);

   ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
   {
      err = vkWaitForFences(
        g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
      check_vk_result(err);

      err = vkResetFences(g_Device, 1, &fd->Fence);
      check_vk_result(err);
   }
   {
      err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
      check_vk_result(err);
      VkCommandBufferBeginInfo info = {};
      info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
      err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
      check_vk_result(err);
   }
   {
      VkRenderPassBeginInfo info = {};
      info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      info.renderPass = wd->RenderPass;
      info.framebuffer = fd->Framebuffer;
      info.renderArea.extent.width = uint32_t(wd->Width);
      info.renderArea.extent.height = uint32_t(wd->Height);
      info.clearValueCount = 1;
      info.pClearValues = &wd->ClearValue;
      vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
   }

   // Record dear imgui primitives into command buffer
   ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

   // Submit command buffer
   vkCmdEndRenderPass(fd->CommandBuffer);
   {
      VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      VkSubmitInfo info = {};
      info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      info.waitSemaphoreCount = 1;
      info.pWaitSemaphores = &image_acquired_semaphore;
      info.pWaitDstStageMask = &wait_stage;
      info.commandBufferCount = 1;
      info.pCommandBuffers = &fd->CommandBuffer;
      info.signalSemaphoreCount = 1;
      info.pSignalSemaphores = &render_complete_semaphore;

      err = vkEndCommandBuffer(fd->CommandBuffer);
      check_vk_result(err);
      err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
      check_vk_result(err);
   }
}

void FramePresent(ImGui_ImplVulkanH_Window* wd)
{
   if (g_SwapChainRebuild)
   {
      return;
   }
   VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
   VkPresentInfoKHR info = {};
   info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
   info.waitSemaphoreCount = 1;
   info.pWaitSemaphores = &render_complete_semaphore;
   info.swapchainCount = 1;
   info.pSwapchains = &wd->Swapchain;
   info.pImageIndices = &wd->FrameIndex;
   VkResult err = vkQueuePresentKHR(g_Queue, &info);
   if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
   {
      g_SwapChainRebuild = true;
      return;
   }
   check_vk_result(err);
   wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
}

void glfw_error_callback(int error, const char* description)
{
   std::cerr << "GLFW Error " << error << " : " << description << '\n';
}

export struct Window
{
   explicit Window()
   {
      glfwSetErrorCallback(glfw_error_callback);
      if (!glfwInit())
      {
         throw std::runtime_error("GLFW Can't be initialized!\n");
      }

      // Create window with Vulkan context
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

      window_ = glfwCreateWindow(1280, 720, "ThumbnailCreator", nullptr, nullptr);

      if (!glfwVulkanSupported())
      {
         throw std::runtime_error("GLFW: Vulkan Not Supported\n");
      }

      ImVector<const char*> extensions;
      uint32_t extensions_count = 0;
      const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
      for (uint32_t i = 0; i < extensions_count; i++)
      {

         extensions.push_back(glfw_extensions[i]);
      }
      SetupVulkan(extensions);

      // Create Window Surface
      VkSurfaceKHR surface;
      err_ = glfwCreateWindowSurface(g_Instance, window_, g_Allocator, &surface);
      check_vk_result(err_);

      // Create Framebuffers
      int w{}, h{};
      glfwGetFramebufferSize(window_, &w, &h);
      wd_ = &g_MainWindowData;
      SetupVulkanWindow(wd_, surface, w, h);

      // Setup Dear ImGui context
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

      // Setup Dear ImGui style
      ImGui::StyleColorsDark();
      // ImGui::StyleColorsLight();

      // Setup Platform/Renderer backends
      ImGui_ImplGlfw_InitForVulkan(window_, true);
      ImGui_ImplVulkan_InitInfo init_info = {};
      init_info.Instance = g_Instance;
      init_info.PhysicalDevice = g_PhysicalDevice;
      init_info.Device = g_Device;
      init_info.QueueFamily = g_QueueFamily;
      init_info.Queue = g_Queue;
      init_info.PipelineCache = g_PipelineCache;
      init_info.DescriptorPool = g_DescriptorPool;
      init_info.Subpass = 0;
      init_info.MinImageCount = g_MinImageCount;
      init_info.ImageCount = wd_->ImageCount;
      init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
      init_info.Allocator = g_Allocator;
      init_info.CheckVkResultFn = check_vk_result;
      ImGui_ImplVulkan_Init(&init_info, wd_->RenderPass);

      // Main loop
      while (!glfwWindowShouldClose(window_))
      {
         glfwPollEvents();

         // Resize swap chain?
         if (g_SwapChainRebuild)
         {
            int width, height;
            glfwGetFramebufferSize(window_, &width, &height);
            if (width > 0 && height > 0)
            {
               ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
               ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
                 g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
               g_MainWindowData.FrameIndex = 0;
               g_SwapChainRebuild = false;
            }
         }

         // Start the Dear ImGui frame
         ImGui_ImplVulkan_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();

         ImGui::SetNextWindowPos(ImVec2{ 0.0f, 0.0f});
         ImGui::SetNextWindowSize(ImVec2{ windowWidth_, windowHeight_ });

         ImGuiWindowFlags flags{ ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration };

         ImGui::Begin("ThumbnailCreator", nullptr, flags);

         ImGui::End();

         // Rendering
         Render();
      }
   }

   void Render() const noexcept
   {
      ImGui::Render();
      ImDrawData* draw_data = ImGui::GetDrawData();
      bool const is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
      if (is_minimized)
      {
         return;
      }

      wd_->ClearValue.color.float32[0] = clear_color_.x * clear_color_.w;
      wd_->ClearValue.color.float32[1] = clear_color_.y * clear_color_.w;
      wd_->ClearValue.color.float32[2] = clear_color_.z * clear_color_.w;
      wd_->ClearValue.color.float32[3] = clear_color_.w;
      FrameRender(wd_, draw_data);
      FramePresent(wd_);
   }

   ~Window() noexcept
   {
      // Cleanup
      err_ = vkDeviceWaitIdle(g_Device);
      check_vk_result(err_);
      ImGui_ImplVulkan_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();

      CleanupVulkanWindow();
      CleanupVulkan();

      glfwDestroyWindow(window_);
      glfwTerminate();
   }

private:
   static constexpr std::uint32_t windowWidth_{ 1280 };
   static constexpr std::uint32_t windowHeight_{ 720 };
   static constexpr ImVec4 clear_color_{ 0.0f, 0.0f, 0.0f, 1.00f };
   GLFWwindow* window_{};
   VkResult err_{};
   ImGui_ImplVulkanH_Window* wd_{};
};
