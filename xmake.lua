add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")
add_repositories("iceblcokmc https://github.com/IceBlcokMC/xmake-repo.git")

LeviLaminaVersion = "1.7.3"
add_requires("levilamina "..LeviLaminaVersion, {configs = {target_type = "server"}})
add_requires("levibuildscript")

-- iceblcokmc
add_requires("economy_bridge main")
add_requires("ll-bstats 0.1.1")

includes("PermCore/static_lib.lua")

if is_config("flavor", "mdim") then
    add_requires("more-dimensions 0.12.0")
    add_requireconfs("**.levilamina", { override=true, version=LeviLaminaVersion, configs = {target_type = "server"}})
end

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("flavor")
    set_values("vanilla", "mdim") -- vanilla: 原版 mdim: 多维度
    set_default("vanilla")
    set_showmenu(true)
option_end()


target("PlotX") -- Change this to your mod name.
    set_license("AGPL-3.0")
    add_deps("PermCore")
    add_includedirs("PermCore/src")
    add_defines("FMT_HEADER_ONLY=1")
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
    add_files("src/**.cc")

    add_packages(
        "levilamina",
        "economy_bridge",
        "ll-bstats"
    )

    add_configfiles("src/BuildInfo.h.in")
    set_configdir("src/plotx")

    if is_config("flavor", "vanilla") then
        add_files("src-vanilla/**.cc")
        set_configvar("IS_MORE_DIM", "false")
        set_configvar("FLAVOR", "Vanilla")
    end

    if is_config("flavor", "mdim") then
        add_files("src-mdim/**.cc")

        add_packages("more-dimensions")
        set_configvar("IS_MORE_DIM", "true")
        set_configvar("FLAVOR", "MoreDimensions")
    end

    after_build(function(target)
        local project_dir =  os.projectdir()
        local assets_dir = path.join(project_dir, "assets")
        local lang_assets_dir = path.join(assets_dir, "lang")

        local bin_dir = path.join(project_dir, "bin")
        local lang_bin_dir = path.join(bin_dir, target:name(), "lang")

        os.mkdir(lang_bin_dir)
        os.cp(path.join(lang_assets_dir, "**"), lang_bin_dir)
    end)
