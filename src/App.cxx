module;

#include <string>      // std::string
#include <string_view> // std::string_view

export module App;

export struct App
{
   explicit App(std::string_view const str)
       : str_{ str.data() }
   {
   }

   [[nodiscard]] constexpr std::string Str() const noexcept { return str_; }

private:
   std::string str_{};
};
