module;

#include <string> // std::string
#if defined(_WIN32)
#include <Windows.h>
#endif

#include <iostream> // std::cout

export module FileManager;

export namespace FileManager
{
using Filter_type = char const*;
auto OpenFile(Filter_type const filter = nullptr) noexcept -> std::string;
};

module :private;

auto FileManager::OpenFile(FileManager::Filter_type const filter) noexcept -> std::string
{
   std::string ret{};
#if defined(_WIN32)
   std::cout << "I'm on Windows\n";
   OPENFILENAME ofn;
   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize = sizeof(ofn);

   char szFile[260] = { 0 };
   ofn.lpstrFile = szFile;
   ofn.lpstrFile[0] = '\0';
   ofn.nMaxFile = sizeof(szFile);
   ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   // Filters.
   ofn.lpstrFilter = filter;

   if (GetOpenFileName(&ofn) == TRUE)
   {
      return ofn.lpstrFile;
   }

#else
   (void)filter;
   std::cout << "FileManager::OpenFile is not implemented on Linux\n";
   std::cout << "Please, tell us wich file do you wanna load:\n";
   std::getline(std::cin, ret);
   std::cout << "Loading: " << ret << "\n\tThis file WON'T be checked!\n";
#endif
   return ret;
}
