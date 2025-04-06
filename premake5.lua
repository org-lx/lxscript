workspace "lxscript"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "liblxscript"
	kind "SharedLib"
	targetprefix ""
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

project "lxscript"
   kind "ConsoleApp"
   language "C"
   cdialect "c99"
   targetdir "bin"
   includedirs { "include" }
   files { "cli/**.c", "cli/**.h" }

	links { "liblxscript" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
