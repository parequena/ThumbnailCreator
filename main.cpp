#include <iostream>

import App;
import Window;

auto main() -> int
{
   App app{ "this_is_an_app" };

   Window window{};

   std::cout << "Hello World!\n";

   return EXIT_SUCCESS;
}
