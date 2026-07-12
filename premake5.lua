workspace "EngineSDK"
    configurations { "Debug", "Release" , "Dist" }
    platforms { "x86", "x64", "arm32", "arm64" }
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
            "NDEBUG"
        }


        symbols "Off"
        optimize "On"

    filter{}
    include "../FiberJobSystem"
    include "../AOSHLL"