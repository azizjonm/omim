#pragma once

#include "resource_style.hpp"
#include "pen_info.hpp"
#include "skin_page.hpp"
#include "../geometry/rect2d.hpp"
#include "../std/unordered_map.hpp"
#include "../std/shared_ptr.hpp"
#include "../std/function.hpp"
#include "../std/map.hpp"
#include "../std/vector.hpp"
#include "../std/string.hpp"
#include "../std/utility.hpp"
#include "../geometry/packer.hpp"

namespace yg
{
  template <typename pair_t>
  struct first_less
  {
    bool operator()(pair_t const & first, pair_t const & second)
    {
      return first.first < second.first;
    }
  };

  namespace gl
  {
    class BaseTexture;
    class VertexBuffer;
    class IndexBuffer;
  }

  class SkinPage;
  class ResourceManager;

  class Skin
  {
  public:

    typedef vector<shared_ptr<SkinPage> > TSkinPages;
    typedef function<void(uint8_t)> clearPageFn;
    typedef function<void(uint8_t)> overflowFn;

  private:

    TSkinPages m_pages;

    uint8_t m_startDynamicPage;
    uint8_t m_currentDynamicPage;

    shared_ptr<ResourceManager> m_resourceManager;

    void clearHandles();

    Skin(shared_ptr<ResourceManager> const & resourceManager,
         TSkinPages const & pages);

    friend class SkinLoader;

    void addDynamicPages(int count);

    typedef pair<uint8_t, uint32_t> id_pair_t;
    id_pair_t unpackID(uint32_t id) const;
    uint32_t packID(uint8_t, uint32_t) const;

    typedef priority_queue<pair<size_t, clearPageFn>,
                           vector<pair<size_t, clearPageFn> >,
                           first_less<pair<size_t, clearPageFn> >
                           > clearPageFns;

    clearPageFns m_clearPageFns;
    void callClearPageFns(uint8_t pageID);

    typedef priority_queue<pair<size_t, overflowFn>,
                           vector<pair<size_t, overflowFn> >,
                           first_less<pair<size_t, overflowFn> >
                           > overflowFns;

    overflowFns m_overflowFns;
    void callOverflowFns(uint8_t pageID);

    void clearPageHandles(uint8_t pageID);
    void onOverflow(uint8_t pageID);
    void changeCurrentDynamicPage();

  public:

    void uploadData();

    /// clean and destroy
    ~Skin();
    /// obtain ResourceStyle from id
    ResourceStyle const * fromID(uint32_t id);
    /// get an identifier from the styleName.
    /// return 0xFFFF if this style is not found in Skin.
    uint32_t mapSymbol(char const * symbolName);
    /// find ruleDef on the texture.
    /// if found - return id.
    /// if not - pack and return id.
    uint32_t mapPenInfo(PenInfo const & penInfo);
    /// find brushDef on the texture.
    /// if found - return id.
    /// if not - pack and return id.
    uint32_t mapColor(Color const & c);
    /// find glyph identified by GlyphKey on texture
    /// if found - return id
    /// if not - pack and return id
    uint32_t mapGlyph(GlyphKey const & gk);

    void addClearPageFn(clearPageFn fn, int priority);
    void addOverflowFn(overflowFn, int priority);

    FontInfo const & getFont(int size) const;

    TSkinPages const & pages() const;

    uint32_t invalidHandle() const;
    uint32_t invalidPageHandle() const;
  };
}
