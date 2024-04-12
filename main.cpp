import Window;
import FileManager;

#include <iostream>
#include <unordered_map>

auto main() -> int
{
   FileManager::Filter_type filters{ "PNG\0*.png\0JPG\0*.jpg\0" };
   std::cout << "File: " << FileManager::OpenFile(filters);

   Window window{};
   while (window.Play())
   {
      window.Update();
      window.Render();
   }

   return 0;
}
