# Yoga graphics or graphics yoga?

TARGET = yg
TEMPLATE = lib
CONFIG += staticlib
DEFINES += YG_LIBRARY

ROOT_DIR = ..
DEPENDENCIES = expat geometry coding base freetype

INCLUDEPATH += $$ROOT_DIR/3party/freetype/include

LIBS += lfreetype

include($$ROOT_DIR/common.pri)

!iphonesimulator-g++42 {
  !iphonedevice-g++42 {
    !bada-simulator {
      !bada-device {
        DEPENDENCIES += platform
      }
    }
  }
}

SOURCES += \
    internal/opengl.cpp \
    yg.cpp \
    vertex.cpp \
    resource_manager.cpp \
    skin.cpp \
    screen.cpp \
    pen_info.cpp \
    resource_style.cpp \
    color.cpp \
    skin_loader.cpp \
    memento.cpp \
    framebuffer.cpp \
    vertexbuffer.cpp \
    indexbuffer.cpp \
    utils.cpp \
    renderbuffer.cpp \
    base_texture.cpp \
    managed_texture.cpp \
    fence.cpp \
    thread_renderer.cpp \
    render_command.cpp \
    blitter.cpp \
    clipper.cpp \
    renderer.cpp \
    render_state.cpp \
    geometry_renderer.cpp \
    skin_page.cpp \
    storage.cpp \
    render_state_updater.cpp \
    glyph_cache.cpp

HEADERS += \
    internal/opengl.hpp \
    yg.hpp \
    vertex.hpp \
    resource_manager.hpp \
    texture.hpp \
    skin.hpp \
    skin_loader.hpp \
    screen.hpp \
    pen_info.hpp \
    resource_style.hpp \
    color.hpp \
    memento.hpp \
    framebuffer.hpp \
    vertexbuffer.hpp \
    indexbuffer.hpp \
    utils.hpp \
    renderbuffer.hpp \
    base_texture.hpp \
    managed_texture.hpp \
    pbuffer_texture.hpp \
    fence.hpp \
    thread_renderer.hpp \
    rendercontext.hpp \
    render_command.hpp \
    blitter.hpp \
    clipper.hpp \
    renderer.hpp \
    render_state.hpp \
    geometry_renderer.hpp \
    skin_page.hpp \
    storage.hpp \
    render_state_updater.hpp \
    render_target.hpp \
    glyph_cache.hpp \
    data_formats.hpp

!iphonesimulator-g++42 {
  !iphonedevice-g++42 {
    !bada-simulator {
      !bada-device {
        HEADERS += internal/opengl_win32.hpp
        SOURCES += internal/opengl_win32.cpp
      }
    }
  }
}
