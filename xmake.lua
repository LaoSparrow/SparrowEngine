add_rules("mode.debug", "mode.release")

set_encodings("utf-8")
if is_plat("windows") then
    if is_mode("release") then
        set_runtimes("MT")
    else
        set_runtimes("MTd")
    end
end

add_requires(
        "vcpkg::glad",
        "vcpkg::glfw3",
        "vcpkg::glm",
        "vcpkg::stb",
        "vcpkg::imgui",
        "vcpkg::assimp",
        "vcpkg::fmt",
        {
            configs = {
                baseline = "98aa6396292d57e737a6ef999d4225ca488859d5"
            },
            debug = is_mode("debug")
        })

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
