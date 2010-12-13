#pragma once

#include "../std/shared_ptr.hpp"
#include "../std/unordered_map.hpp"
#include "../std/map.hpp"
#include "../std/vector.hpp"

#include "../geometry/packer.hpp"
#include "../geometry/rect2d.hpp"

#include "pen_info.hpp"
#include "color.hpp"
#include "glyph_cache.hpp"

namespace yg
{
  namespace gl
  {
    class BaseTexture;
  }

  struct CharStyle;
  struct ResourceStyle;
  class ResourceManager;

  struct GlyphUploadCmd
  {
    shared_ptr<GlyphInfo> m_glyphInfo;
    m2::RectU m_rect;
    GlyphUploadCmd(shared_ptr<GlyphInfo> const & glyphInfo, m2::RectU const & rect);
    GlyphUploadCmd();
  };

  struct ColorUploadCmd
  {
    yg::Color m_color;
    m2::RectU m_rect;
    ColorUploadCmd(yg::Color const & color, m2::RectU const & rect);
    ColorUploadCmd();
  };

  struct PenUploadCmd
  {
    yg::PenInfo m_penInfo;
    m2::RectU m_rect;
    PenUploadCmd(yg::PenInfo const & penInfo, m2::RectU const & rect);
    PenUploadCmd();
  };

  struct FontInfo
  {
    int8_t m_fontSize;
    typedef map<int32_t, pair<shared_ptr<CharStyle>, shared_ptr<CharStyle> > > TChars;
    TChars m_chars;

    mutable pair<ResourceStyle *, ResourceStyle *> m_invalidChar;
    FontInfo() : m_invalidChar(0, 0){}

    ResourceStyle * fromID(uint32_t id, bool primaryGlyph = true) const;
  };

  class SkinPage
  {
  public:

    typedef m2::Packer::overflowFn overflowFn;

  private:

//    typedef unordered_map<uint32_t, shared_ptr<ResourceStyle> > TStyles;
    typedef map<uint32_t, shared_ptr<ResourceStyle> > TStyles;
    TStyles m_styles;

//    typedef unordered_map<string, uint32_t> TPointNameMap;
    typedef map<string, uint32_t> TPointNameMap;
    TPointNameMap m_pointNameMap;

    typedef map<PenInfo, uint32_t> TPenInfoMap;
    TPenInfoMap m_penInfoMap;

    typedef map<Color, uint32_t> TColorMap;
    TColorMap m_colorMap;

    typedef map<GlyphKey, uint32_t> TGlyphMap;
    TGlyphMap m_glyphMap;

    m2::Packer m_packer;
    shared_ptr<gl::BaseTexture> m_texture;
    shared_ptr<ResourceManager> m_resourceManager;

    vector<ColorUploadCmd> m_colorUploadCommands;
    vector<PenUploadCmd> m_penUploadCommands;
    vector<GlyphUploadCmd> m_glyphUploadCommands;

    void uploadPenInfo();
    void uploadColors();
    void uploadGlyphs();

    bool m_isDynamic;
    uint32_t m_pageID;

    /// number of pending rendering commands,
    /// that are using this skin_page
    uint32_t m_activeCommands;

    friend class SkinLoader;

  public:

    void clearColorHandles();
    void clearPenInfoHandles();
    void clearFontHandles();

    void clearHandles();

    void uploadData();

    SkinPage();

    /// creation of a static page
    SkinPage(shared_ptr<ResourceManager> const & resourceManager,
             char const * name,
             uint8_t pageID);

    /// creation of a dynamic page
    SkinPage(shared_ptr<ResourceManager> const & resourceManager,
             uint8_t pageID);

    void reserveTexture();
    void freeTexture();

    uint32_t findColor(Color const & c) const;
    uint32_t mapColor(Color const & c);
    bool     hasRoom(Color const & c) const;

    uint32_t findPenInfo(PenInfo const & penInfo) const;
    uint32_t mapPenInfo(PenInfo const & penInfo);
    bool     hasRoom(PenInfo const & penInfo) const;

    uint32_t findGlyph(GlyphKey const & g) const;
    uint32_t mapGlyph(GlyphKey const & g);
    bool hasRoom(GlyphKey const & g) const;

    uint32_t findSymbol(char const * symbolName) const;

    ResourceStyle * fromID(uint32_t idx) const;

    bool isDynamic() const;

    void addOverflowFn(overflowFn fn, int priority);

    shared_ptr<gl::BaseTexture> const & texture() const;
  };
}
