/*=============================================================================
   Copyright (c) 2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_GALLERY_NOTEBOOK_MAY_23_2020)
#define ELEMENTS_GALLERY_NOTEBOOK_MAY_23_2020

#include <elements/element/gallery/tab.hpp>
#include <elements/element/tile.hpp>
#include <elements/element/align.hpp>
#include <elements/element/margin.hpp>
#include <elements/view.hpp>
#include <stdexcept>

namespace cycfi { namespace elements
{
   namespace detail
   {
      template <typename Pages, typename Tab, typename... RestTabs>
      void link_tabs(view& view_, Pages pages, std::size_t index, Tab tab, RestTabs... rest)
      {
         tab->on_click =
            [index, pages, &view_](bool state)
            {
               if (state)
               {
                  pages->select(index);
                  view_.refresh(*pages);
               }
            };

         if constexpr(sizeof...(RestTabs) > 0)
            link_tabs(view_, pages, index+1, rest...);
      }

      template <typename Pages, typename Tab, typename... RestTabs>
      auto make_notebook(view& view_, Pages pages, Tab tab, RestTabs... rest)
      {
         if (pages->size() != sizeof...(RestTabs) + 1)
            throw std::runtime_error{ "Error: The number of pages and tabs do not match." };

         // Select the first tab
         tab->select(true);

         // Link the tabs
         link_tabs(view_, pages, 0, tab, rest...);

         return vtile(
            margin(
               { 5, 15, 20, 0 },
               align_left(htile(hold(tab), hold(rest)...))
            ),
            hold(pages)
         );
      }
   }

   template <typename Pages, typename... Tab>
   auto notebook(view& view_, Pages&& pages_, Tab&&... tab)
   {
      return detail::make_notebook(view_, share(pages_), share(tab)...);
   }
}}

#endif