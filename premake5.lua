workspace "EngineSDK"
    configurations { "Debug", "Release" , "Dist" }
    platforms { "x86", "x64", "arm32", "arm64" }
    cleancommands {
        "{RMDIR} %{prj.location}/Binnaries",
        "{RMDIR} %{prj.location}/Objects"
    }




    filter "platforms:x86"
        architecture "x86"

    filter "platforms:x64"
        architecture "x64"

    filter "platforms:arm32"
        architecture "arm"

    filter "platforms:arm64"
        architecture "arm64"
    
    filter{}







    filter "configurations:Debug"

        defines {
            "_DEBUG",
            "DEBUG"
        }

        symbols "On"
        optimize "Off"

        buildoptions {
            "--coverage"
        }

        linkoptions {
            "--coverage"
        }

    filter "configurations:Release"

        defines {
            "_NDEBUG",
            "NDEBUG",
            "RELEASE"
        }

        symbols "On"
        optimize "On"

        buildoptions {
            "--coverage"
        }

        linkoptions {
            "--coverage"
        }

    filter "configurations:Dist"

        defines {
            "_NDEBUG",
            "NDEBUG",
            "DIST"
        }

        symbols "Off"
        optimize "On"

    filter{}





    filter "system:linux"
        defines {
            "SDK_OS_LINUX"
        }
        buildoptions{
            "-fvisibility=hidden"
        }
        targetdir "Binaries/Linux/%{cfg.platform}/%{cfg.buildcfg}"
        objdir "Objects/Linux/%{cfg.platform}/%{cfg.buildcfg}"
        libdirs {
            "Binaries/Linux/%{cfg.platform}/%{cfg.buildcfg}"
        }

    filter "system:windows"
        defines {
            "SDK_OS_WINDOWS"
        }
        targetdir "Binaries/Windows/%{cfg.platform}/%{cfg.buildcfg}"
        objdir "Objects/Windows/%{cfg.platform}/%{cfg.buildcfg}"
        libdirs {
            "Binaries/Windows/%{cfg.platform}/%{cfg.buildcfg}"
        }

    filter{}

    startproject "RuntimeSDK"
    
    include "CoreSDK"
    include "RuntimeSDK"
    include "TestSDK"



