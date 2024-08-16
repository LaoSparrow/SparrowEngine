add_rules("mode.debug", "mode.release")

set_encodings("utf-8")
if is_mode("release") then
    set_runtimes("MT")
else
    set_runtimes("MTd")
end

add_requires(
        "vcpkg::glad >=0.1.36",
        "vcpkg::glfw3 >=3.4",
        "vcpkg::glm >=1.0.1",
        "vcpkg::Stb 2023-04-11",
        "vcpkg::imgui >=1.90.6",
        "vcpkg::assimp >=5.4.0",
        "vcpkg::fmt >=10.2.1", {debug = is_mode("debug")})

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
    if is_plat("windows") then
        add_syslinks("shell32")
    end

target("SparrowEngine.Example")
    set_languages("c++20")
    set_kind("binary")
    add_files("example/src/**.cpp")
    add_includedirs("example/src/")
    add_deps("SparrowEngine")
    after_build(function (target)
        os.rm(target:targetdir() .. "/resources")
        os.cp("$(projectdir)/example/resources/", target:targetdir())
    end)
