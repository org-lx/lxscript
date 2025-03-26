workspace "lxscript"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "lxscript"
   kind "ConsoleApp"
   language "C"
   cdialect "c99"
   targetdir "bin"
   includedirs { "include" }
   files { "src/**.c", "include/**.h" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
