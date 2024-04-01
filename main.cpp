#include <iostream> // std::cout

import App;

int main()
{
   App app{ "this_is_an_app" };

   std::cout << "Hello : " << app.Str() << '\n';
   return EXIT_SUCCESS;
}
