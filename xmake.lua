add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")
add_repositories("iceblcokmc https://github.com/IceBlcokMC/xmake-repo.git")

add_requires("levilamina 1.7.0", {configs = {target_type = "server"}})
add_requires("levibuildscript")

-- iceblcokmc
add_requires("economy_bridge main")
add_requires("perm_core 8ae921b0b10ec56ff1a1cb840cf92f22fad407d8")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("overworld")
    set_default(true)
    set_showmenu(true)
option_end()


target("PlotX") -- Change this to your mod name.
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
    add_rules("plugin.compile_commands.autoupdate")
    add_cxflags(
        "/EHa",
        "/utf-8",
        "/W4",
        "/w44265",
        "/w44289",
        "/w44296",
        "/w45263",
        "/w44738",
        "/w45204"
    )
    add_defines(
        "NOMINMAX",
        "UNICODE",
        "PLOTX_EXPORT"
    )
    set_exceptions("none") -- To avoid conflicts with /EHa.
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")
    add_headerfiles(
        "src/(plotX/**.hpp)"
    )
    add_includedirs(
        "src",
        "patches"
    )
    add_files(
        "src/**.cpp",
        "src/**.cc"
    )

    add_packages("levilamina", "economy_bridge", "perm_core")

    if has_config("overworld") then
        add_defines("PLOTX_OVERWORLD")
    end

    after_build(function (target) 
        local project_dir =  os.projectdir()
        local bin_dir = path.join(project_dir, "bin")

        local script_src = path.join(project_dir, "scripting", "src")
        local script_bin = path.join(bin_dir, target:name(), "scripting")

        os.mkdir(script_bin)
        os.cp(script_src.."/*", script_bin)
    end)
