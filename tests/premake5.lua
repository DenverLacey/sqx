workspace 'tests'
configurations { 'debug', 'release' }

project 'tests'
    kind 'ConsoleApp'
    language 'C'
    cdialect 'gnu17'
    toolset 'clang'

    files {
        'src/**.h',
        'src/**.c',
        'src/**.hpp',
        'src/**.cpp',
        'src/**.hxx',
        'src/**.cxx',
        'src/**.cc',
        '../**.h',
        '../**.c',
    }

    includedirs {
        'src',
        '../'
    }

    filter 'action:gmake2'
        buildoptions {
            '-Wpedantic',
            '-Wall',
            '-Wextra',
            '-Werror',
            '-Wno-gnu-auto-type',
            '-Wno-gnu-statement-expression',
            '-Wno-gnu-alignof-expression',
        }

    filter 'configurations:debug'
        defines { 'DEBUG' }
        targetdir 'bin/debug'
        symbols 'On'
        optimize 'Debug'

    filter 'configurations:release'
        defines { 'NDEBUG' }
        targetdir 'bin/release'
        optimize 'Full'

project 'tests-no-popcount'
    kind 'ConsoleApp'
    language 'C'
    cdialect 'gnu17'
    toolset 'clang'

    files {
        'src/**.h',
        'src/**.c',
        'src/**.hpp',
        'src/**.cpp',
        'src/**.hxx',
        'src/**.cxx',
        'src/**.cc',
        '../**.h',
        '../**.c',
    }

    includedirs {
        'src',
        '..'
    }

    filter 'action:gmake2'
        buildoptions {
            '-Wpedantic',
            '-Wall',
            '-Wextra',
            '-Werror',
            '-Wno-gnu-auto-type',
            '-Wno-gnu-statement-expression',
            '-Wno-gnu-alignof-expression',
        }

    filter 'configurations:debug'
        defines { 'DEBUG', 'SQX_NO_POPCOUNT' }
        targetdir 'bin/debug'
        symbols 'On'
        optimize 'Debug'

    filter 'configurations:release'
        defines { 'NDEBUG', 'SQX_NO_POPCOUNT' }
        targetdir 'bin/release'
        optimize 'Full'

