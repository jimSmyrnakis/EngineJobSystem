project "FiberJobSystemPerformance"

    kind "ConsoleApp"
    language "C++"

    

    dependson {
        "FiberJobSystem"
    }

    files {
        "performance/**/*.cpp",
        "performance/*.cpp",
        "performance/**/*.c",
        "performance/*.c"
    }

    includedirs {
        "src",
        "inc",
        "../EngineSDKTools/inc"
    }

    links {
        "FiberJobSystem",
        "benchmark",
        "AOSHLL"
    }

    filter "files:**.c"
        cdialect "C17"

    filter "files:**.cpp"
        cppdialect "C++20"
    filter {}

    filter{ "system:linux"}
        targetdir "bin/perf/Linux/%{cfg.platform}/%{cfg.buildcfg}"
        objdir "obj/perf/Linux/%{cfg.platform}/%{cfg.buildcfg}"
        libdirs {
            "lib/Linux/%{cfg.platform}/%{cfg.buildcfg}",
            "../AOSHLL/lib/Linux/%{cfg.platform}/%{cfg.buildcfg}"
        }
        links {
            "pthread"
        }
        buildoptions{
            "-fvisibility=hidden"
        }

    filter{ "system:windows"}
        targetdir "bin/perf/Windows/%{cfg.platform}/%{cfg.buildcfg}"
        objdir "obj/perf/Windows/%{cfg.platform}/%{cfg.buildcfg}"
        libdirs {
            "lib/Windows/%{cfg.platform}/%{cfg.buildcfg}",
            "../AOSHLL/lib/Windows/%{cfg.platform}/%{cfg.buildcfg}"
        }

    filter {}
