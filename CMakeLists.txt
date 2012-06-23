project(nmosh-pdcurses)
cmake_minimum_required(VERSION 2.8)

set(srcs_win32a
    win32a/pdcclip.c
    win32a/pdcdisp.c
    win32a/pdcgetsc.c
    win32a/pdckbd.c
    win32a/pdcscrn.c
    win32a/pdcsetsc.c
    win32a/pdcutil.c)

set(srcs_win32
    win32/pdcclip.c
    win32/pdcdisp.c
    win32/pdcgetsc.c
    win32/pdckbd.c
    win32/pdcscrn.c
    win32/pdcsetsc.c
    win32/pdcutil.c)

set(srcs_common
    pdcurses/addch.c
    pdcurses/addchstr.c
    pdcurses/addstr.c
    pdcurses/attr.c
    pdcurses/beep.c
    pdcurses/bkgd.c
    pdcurses/border.c
    pdcurses/clear.c
    pdcurses/color.c
    pdcurses/debug.c
    pdcurses/delch.c
    pdcurses/deleteln.c
    pdcurses/deprec.c
    pdcurses/getch.c
    pdcurses/getstr.c
    pdcurses/getyx.c
    pdcurses/inch.c
    pdcurses/inchstr.c
    pdcurses/initscr.c
    pdcurses/inopts.c
    pdcurses/insch.c
    pdcurses/insstr.c
    pdcurses/instr.c
    pdcurses/kernel.c
    pdcurses/keyname.c
    pdcurses/mouse.c
    pdcurses/move.c
    pdcurses/outopts.c
    pdcurses/overlay.c
    pdcurses/pad.c
    pdcurses/panel.c
    pdcurses/printw.c
    pdcurses/refresh.c
    pdcurses/scanw.c
    pdcurses/scr_dump.c
    pdcurses/scroll.c
    pdcurses/slk.c
    pdcurses/termattr.c
    pdcurses/terminfo.c
    pdcurses/touch.c
    pdcurses/util.c
    pdcurses/window.c
    )

add_definitions(-DPDC_WIDE -DPDC_FORCE_UTF8)

include_directories(.)

add_library(pdcurses_win32a STATIC
    ${srcs_common}
    ${srcs_win32a})

add_library(pdcurses_win32 STATIC
    ${srcs_common}
    ${srcs_win32})

if(NOT MSVC)
set_target_properties(pdcurses_win32a pdcurses_win32
    PROPERTIES
    COMPILE_FLAGS "-fwide-exec-charset=ucs-2le")
endif()
