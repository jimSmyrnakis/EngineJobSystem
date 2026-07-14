project "TestSDK"

    kind "ConsoleApp"
    language "C++"
    
    
    dependson {
        "CoreSDK"
    }

    files {
        "src/**.c",
        "src/**.cpp",
        "src/*.c",
        "src/*.cpp"
    }

    

    includedirs {
        "src",
        "../CoreSDK/inc" 
    }


    links {
        "CoreSDK",
        "gtest",
        "gtest_main"
    }

    filter{ "system:linux"}
        includedirs {
            "/usr/include" 
        }
        
        links {
            "pthread"
        }

    filter{ "system:windows"}
        -- change this if you installed gtest somewhere else
        includedirs {
            "C:/Libraries/googletest/include"
        }
        libdirs {
            "C:/Libraries/googletest/lib"
        }
    filter{}


    
    
filter {}