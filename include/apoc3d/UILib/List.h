#pragma once
#ifndef APOC3D_LIST_H
#define APOC3D_LIST_H

/* -----------------------------------------------------------------------
 * This source file is part of Apoc3D Framework
 * 
 * Copyright (c) 2009-2014 Tao Xin
 * 
 * This content of this file is subject to the terms of the Mozilla Public 
 * License v2.0. If a copy of the MPL was not distributed with this file, 
 * you can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * This program is distributed in the hope that it will be useful, 
 * WITHOUT WARRANTY OF ANY KIND; either express or implied. See the 
 * Mozilla Public License for more details.
 * 
 * ------------------------------------------------------------------------ */

#include "Control.h"

#include "apoc3d/Collections/List2D.h"

namespace Apoc3D
{
	namespace UI
	{
		class APAPI ListBox : public ScrollableControl
		{
			RTTI_DERIVED(ListBox, ScrollableControl);
		public:
			typedef EventDelegate<ListBox*> ListBoxEvent;
			typedef EventDelegate<ListBox*, int32, const Apoc3D::Math::Rectangle&> ListBoxItemEvent;

			ListBox(const StyleSkin* skin, const Point& position, int width, int height, const List<String>& items);
			ListBox(const ListBoxVisualSettings& settings, const Point& position, int width, int height, const List<String>& items);

			ListBox(const StyleSkin* skin, const Point& position, const Point& size, const List<String>& items);
			ListBox(const ListBoxVisualSettings& settings, const Point& position, const Point& size, const List<String>& items);

			~ListBox();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			int32 FindEntry(const String& v);

			List<String>& getItems() { return m_items; }
			void RemeasureItemWidth();

			int32 getItemHeight() const;

			int getSelectedIndex() const { return m_selectedIndex; }
			void setSelectedIndex(int i) { m_selectedIndex = i; }
			int32 getHoverIndex() const { return m_hoverIndex; }

			bool isMouseHover() const { return m_mouseHover; }
			bool isMouseHoveringSubItems() const;
			Apoc3D::Math::Rectangle getMouseHoverSubArea() const;

			Apoc3D::Math::Rectangle GetItemArea(int32 i) const;
			Apoc3D::Math::Rectangle GetItemArea(const Apoc3D::Math::Rectangle& cntArea, int32 i) const;

			void setWidth(int w) { m_size.X = w; }
			void setHeight(int h) { m_size.Y = h; }

			TextRenderSettings ItemSettings;

			UIGraphic BackgroundGraphic;
			
			ControlBounds Margin;
			ControlBounds Padding;

			ColorValue HoverBGColor = CV_Silver;
			ColorValue SelectionBGColor = CV_LightGray;

			ListBoxEvent eventSelect;
			ListBoxEvent eventSelectionChanged;
			ListBoxEvent eventPress;
			ListBoxEvent eventRelease;

			ListBoxItemEvent eventItemRender;
		private:
			void Initialize(const StyleSkin* skin);
			void Initialize(const ListBoxVisualSettings& settings);

			void AlignHeight();

			void RenderSelectionBox(Sprite* sprite, int index, const Apoc3D::Math::Rectangle& area);
			void DrawBackground(Sprite* sprite);
			
			void OnMouseHover();
			void OnMouseOut();
			void OnPress();
			void OnRelease();

			List<String> m_items;
			int m_previousItemCount = 0;
			bool m_mouseHover = false;

			int m_visisbleItems = 0;

			int m_hoverIndex = -1;
			int m_selectedIndex = -1;

			int m_hScrollWidth = 0;
			int32 m_itemHeight;
		};

		class APAPI TreeViewNode
		{
		public:
			TreeViewNode(const String& text)
				: m_text(text) { }

			TreeViewNode(const String& text, Texture* icon)
				: m_text(text), m_icon(icon) { }

			void Expand()
			{
				m_expanded = true;
			}
			void Close()
			{
				m_expanded = false;
			}


			bool isExpanded() const { return m_expanded; }
			const String& getText() const { return m_text; }
			void setText(const String& txt) { m_text = txt; }

			Texture* getIcon() const { return m_icon; }
			void setIcon(Texture* texture) { m_icon = texture; }

			List<TreeViewNode*>& getNodes() { return m_subNode; }

			void* UserData = nullptr;

		private:
			String m_text;
			Texture* m_icon = nullptr;
			List<TreeViewNode*> m_subNode;

			bool m_expanded = false;
		};

		class APAPI TreeView : public ScrollableControl
		{
			RTTI_DERIVED(TreeView, ScrollableControl);
		public:
			
			TreeView(const StyleSkin* skin, const Point& position, int width, int height);
			TreeView(const StyleSkin* skin, const Point& position, const Point& size);
			virtual ~TreeView();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			void SetSize(const Point& newSize);

			void NukeTreeViewNodes();
			
			List<TreeViewNode*>& getNodes() { return m_nodes; }

			TreeViewNode* getSelectedNode() const { return m_selectedNode; }

			TextRenderSettings ItemSettings;

			UIGraphic BackgroundGraphic;

			ControlBounds Margin;

			UIEventHandler eventSelect;
			UIEventHandler eventSelectionChanged;

		private:
			void Initialize(const StyleSkin* skin);
			void NukeTreeViewNodes(List<TreeViewNode*>& nodes);
			int GetItemHeight() const;

			void UpdateHScrollbar();

			void RenderSelectionBox(Sprite* sprite, TreeViewNode* node, const Apoc3D::Math::Rectangle& contentArea, const Point& txtPos);
			void DrawBackground(Sprite* sprite);
			void DrawNodes(Sprite* sprite, const List<TreeViewNode*>& nodes, int depth, int& counter, int maxCount);

			int GetExpandedNodeCount() const { return GetExpandedNodeCount(m_nodes); }
			int GetExpandedNodeCount(const List<TreeViewNode*>& node) const;
			int GetExpandedNodeMaxRight(const List<TreeViewNode*>& node) const;
			int GetAllVisibleNodeCount(const List<TreeViewNode*>& node) const;
			int GetAllVisibleNodeCount() const;
			int MeasureExpandedNodeWidth() const;
			int MeasureExpandedModeHeight() const;

			void OnMouseHover();
			void OnMouseOut();
			void OnPress();
			void OnRelease();

			List<TreeViewNode*> m_nodes;

			int m_visisbleItems = 0;

			TreeViewNode* m_selectedNode = nullptr;
			TreeViewNode* m_hoverNode = nullptr;
			TreeViewNode* m_anyHoverNode = nullptr;
			
			int m_hScrollWidth = 0;
			bool m_mouseHover = false;
		};

		typedef EventDelegate<int, int> ListViewSelectionHandler;

		class APAPI ListView : public ScrollableControl
		{
			RTTI_DERIVED(ListView, ScrollableControl);
		public:
			class Header
			{
			public:
				Header() { }
				Header(const String& text, int width)
					: Text(text), Width(width) { }

				String Text;
				int Width = 0;

				UIEventHandler eventPress;
				UIEventHandler enentRelease;
			};
			enum ListViewHeaderStyle
			{
				LHSTYLE_Clickable,
				LHSTYLE_Nonclockable,
				LHSTYLE_None
			};

			ListView(const StyleSkin* skin, const Point& position, const Point& size, const List2D<String>& items);
			virtual ~ListView();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			List2D<String>& getItems() { return m_items; }
			List<Header>& getColumnHeader() { return m_columnHeader; }
			ListViewHeaderStyle getHeaderStyle() const { return m_headerStyle; }
			void setHeaderStyle(ListViewHeaderStyle s) { m_headerStyle = s; }

			int getSelectedRowIndex() const { return m_selectedRow; }
			int getSelectedColumnIndex() const { return m_selectedColumn; }

			bool ShowGridLines = false;
			bool FullRowSelect = false;
			bool HoverSelectionMode = true;

			TextRenderSettings TextSettings;

			ListViewSelectionHandler eventSelected;
		private:
			void Initialize(const StyleSkin* skin);
			int GetVisibleItems();
			void GetVisibleItemsRange(int32& start, int32& end);

			Point GetColumnHeaderOffset() const;
			Point GetCellOffset() const;

			void UpdateColumnHeaders();
			void UpdateScrollBars();
			void UpdateSelection();

			void DrawBackground(Sprite* sprite);
			void DrawColumnHeaders(Sprite* sprite);
			void DrawHeaderEnd(Sprite* sprite, const Point& pos, int32 width);

			void DrawGridLines(Sprite* sprite);
			void DrawItems(Sprite* sprite);
			void DrawSelectedBox(Sprite* sprite, const Apoc3D::Math::Rectangle& area);
			void DrawHoverBox(Sprite* sprite, const Apoc3D::Math::Rectangle& area);
			void DrawItemBox(Sprite* sprite, const Apoc3D::Math::Rectangle& area, ColorValue cv);

			void OnMouseHover();
			void OnMouseOut();
			void OnPress();
			void OnRelease();

			List2D<String> m_items;

			List<Header> m_columnHeader;
			int32 m_headerHeight = 0;
			int32 m_rowHeight = 0;

			bool m_isResizingHeaders = false;
			int m_resizeIndex = -1;

			ListViewHeaderStyle m_headerStyle = LHSTYLE_Clickable;
			int m_headerHoverIndex = -1;
			
			int m_selectedRow = -1;
			int m_selectedColumn = -1;
			int m_hoverRowIndex = -1;
			int m_hoverColumnIndex = -1;
			Apoc3D::Math::Rectangle m_contentArea;


		};
	}
}

#endif