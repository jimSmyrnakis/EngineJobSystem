project "RuntimeSDK"

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
        "CoreSDK"
    }

    filter{ "system:linux"}
        
        links {
            "pthread"
        }

    filter{ "system:windows"}
        
    filter{}


    
    
filter {}