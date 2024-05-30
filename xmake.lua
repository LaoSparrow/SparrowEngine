add_rules("mode.debug", "mode.release")

add_requires(
        "vcpkg::glad",
        "vcpkg::glfw3",
        "vcpkg::glm",
        "vcpkg::Stb",
        "vcpkg::imgui",
        "vcpkg::assimp",
        "vcpkg::fmt", {debug = is_mode("debug")})

target("SparrowEngine")
    set_languages("c++20")
    set_kind("static")
    add_files(
            "core/external/src/**.cpp",
            "core/src/**.cpp")
    add_includedirs("core/external/include/")
    add_includedirs("core/include/", {public = true})
    add_packages(
            "vcpkg::glad",
            "vcpkg::glfw3",
            "vcpkg::glm",
            "vcpkg::Stb",
            "vcpkg::imgui",
            "vcpkg::assimp",
            "vcpkg::fmt", {public = true})

target("SparrowEngine.Example")
    set_languages("c++20")
    set_kind("binary")
    add_files("example/src/**.cpp")
    add_includedirs("example/src/")
    add_deps("SparrowEngine")
    --after_build(function (target)
    --    os.rm("$(buildir)/$(plat)/$(arch)/$(mode)/resources")
    --    os.cp("$(projectdir)/example/resources/", "$(buildir)/$(plat)/$(arch)/$(mode)/")
    --end)
