
project "CoreSDK"
    kind "SharedLib"
    language "C++"
    

    includedirs { 
        "src" , 
        "inc" 
    }
    
    
    files{
        "./src/**.c",
        "./src/**.cpp",
        "./src/*.c",
        "./src/*.cpp"
        
    }




    -- C files use C17
    filter "files:**.c"
        cdialect "C17"

    -- C++ files use C++20
    filter "files:**.cpp"
        cppdialect "C++20"

    filter{}

        





    filter {"system:linux"}
        removefiles { "src/Windows/**" , "src/**/Windows/**"  }
        defines{
            "SDK_BUILD"
        }

    filter {"system:windows"}
        removefiles { "src/Linux/**" , "src/**/Linux/**"  }
        defines{
            "_SDK_BUILD"
        }
    filter{}





    filter { "system:linux", "platforms:x86" }
        files{"src/Linux/x86/*.asm" , "src/**/Linux/x86/*.asm"}

    filter { "system:linux", "platforms:x64" }
        files{"src/Linux/x64/*.asm" , "src/**/Linux/x64/*.asm"}

    filter { "system:linux", "platforms:arm32" }
        files{"src/Linux/ARM32/*.s" , "src/**/Linux/ARM32/*.s"}

    filter { "system:linux", "platforms:arm64" }
        files{"src/Linux/ARM64/*.s" , "src/**/Linux/ARM64/*.s"}

    filter{}







    filter { "system:windows", "platforms:x86" }
        files{"src/Windows/x86/*.asm" , "src/**/Windows/x86/*.asm"}

    filter { "system:windows", "platforms:x64" }
        files{"src/Windows/x64/*.asm" , "src/**/Windows/x64/*.asm"}

    filter { "system:windows", "platforms:arm32" }
        files{"src/Windows/ARM32/*.asm" , "src/**/Windows/ARM32/*.asm"}

    filter { "system:windows", "platforms:arm64" }
        files{"src/Windows/ARM64/*.asm" , "src/**/Windows/ARM64/*.asm"}
        
    
    filter{}








        
        -- x86 Linux NASM
    filter { "system:linux", "platforms:x86", "files:src/**/*.asm" }
        buildcommands {
            "nasm -f elf32 %{file.relpath} -o %{cfg.objdir}/%{file.basename}.o"
        }
        
        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.o"
        }


    -- x64 Linux NASM
    filter { "system:linux", "platforms:x64", "files:src/**/*.asm" }
        
        buildcommands {
            "nasm -f elf64 %{file.relpath} -o %{cfg.objdir}/%{file.basename}.o"
        }

        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.o"
        }
        
        
    -- Linux ARM32 GNU assembler
    filter { "system:linux", "platforms:arm32", "files:src/**/*.s" }
        buildcommands {
            "arm-linux-gnueabihf-as %{file.relpath} -o %{cfg.objdir}/%{file.basename}.o"
        }

        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.o"
        }


    -- Linux ARM64 GNU assembler
    filter { "system:linux", "platforms:arm64", "files:src/**/*.s" }
        buildcommands {
            "aarch64-linux-gnu-as %{file.relpath} -o %{cfg.objdir}/%{file.basename}.o"
        }
        
        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.o"
        }


    -- Windows x86 NASM
    filter { "system:windows", "platforms:x86", "files:src/**/*.asm" }
        buildcommands {
            "nasm -f win32 %{file.relpath} -o %{cfg.objdir}/%{file.basename}.obj"
        }
        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.obj"
        }



    -- Windows x64 NASM
    filter { "system:windows", "platforms:x64", "files:src/**/*.asm" }
        buildcommands {
            "nasm -f win64 %{file.relpath} -o %{cfg.objdir}/%{file.basename}.obj"
        }
        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.obj"
        }



    -- Windows ARM32
    filter { "system:windows", "platforms:arm32", "files:src/**/*.asm" }
        buildcommands {
            "armasm %{file.relpath} -o %{cfg.objdir}/%{file.basename}.obj"
        }
        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.obj"
        }



    -- Windows ARM64
    filter { "system:windows", "platforms:arm64", "files:src/**/*.asm" }
        buildcommands {
            "armasm64 %{file.relpath} -o %{cfg.objdir}/%{file.basename}.obj"
        }
        buildoutputs {
            "%{cfg.objdir}/%{file.basename}.obj"
        }

    filter{}
