import Window;

auto main() -> int
{
   Window window{};
   while (window.Play())
   {
      window.Update();
      window.Render();
   }

   return 0;
}
