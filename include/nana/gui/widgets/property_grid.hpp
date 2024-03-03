/*
 *		nana::propertygrid Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROPERTYGRID_H
#define NANA_CREATOR_PROPERTYGRID_H

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/menu.hpp>

namespace nana
{
	class propertygrid;

	namespace drawerbase::propertygrid
	{
		struct index_pair
		{
			std::size_t cat;	//pos of category
			std::size_t item;	//pos of item inside category

			index_pair(std::size_t cat_pos = npos, std::size_t item_pos = npos)
				: cat(cat_pos), item(item_pos)
			{}

			bool empty() const
			{
				return (npos == cat);
			}

			void reset()
			{
				cat = item = npos;
			}

			void set_both(std::size_t n)
			{
				cat = item = n;
			}

			bool is_category() const
			{
				return (npos != cat && npos == item);
			}

			bool is_item() const
			{
				return (npos != cat && npos != item);
			}

			bool operator==(const index_pair& r) const
			{
				return (r.cat == cat && r.item == item);
			}

			bool operator!=(const index_pair& r) const
			{
				return !this->operator==(r);
			}

			bool operator>(const index_pair& r) const
			{
				return (cat > r.cat) || (cat == r.cat && item > r.item);
			}
		};

		/// struct essence_t
		///@brief:	this struct gives many data for propertygrid,
		///			the state of the struct does not effect on member funcions, therefore all data members are public.
		struct essence_t;

		/// class pgitem
		class pgitem
		{
		public:
			pgitem() = default;

			pgitem(const std::string& label, const std::string& value = "")	///< Constructor
				: label_(label), value_(value), def_(value), ess_(nullptr)
			{}

			virtual ~pgitem() = default;

			void init(window wd);							///< Initialize the inline controls (should not be used)

			virtual void activate(essence_t* ess, const index_pair& idx)	///< Activate inline widgets (should not be used)
			{
				ess_ = ess;
				idx_ = idx;
			}

			virtual void label(const std::string& label)	///< Sets item label
			{
				label_ = label;
			}
			virtual std::string label() const				///< Gets item label
			{
				return label_;
			}
			virtual void value(const std::string& value)	///< Sets item value
			{
				if (value_ == value)
					return;
				value_ = value;
				update();
			}
			virtual std::string value() const				///< Gets item value
			{
				return value_;
			}
			virtual void defvalue(const std::string& value)	///< Sets item default value
			{
				if (def_ == value)
					return;
				def_ = value;
				update();
			}
			virtual std::string defvalue() const			///< Gets item default value
			{
				return def_;
			}
			virtual bool isdefault() const					///< Returns true if item value is equal to default value. False otherwise
			{
				return def_ == value_;
			}

			virtual void reset()							///< Resets item value to default
			{
				value(def_);
			}

			virtual unsigned size() const					///< Returns the item (vertical) size
			{
				return size_;
			}

			virtual void tooltip(const std::string& help_text)
			{
				// nothing to be done for the base class
			}

			virtual void typeface_changed(unsigned text_height)	///< Inform the item the font is changed (should not be used)
			{
				size_ = text_height + 10;
			}

			virtual void draw(paint::graphics* graph, rectangle area, unsigned labelw, unsigned valuew, unsigned iboxw, const int txtoff, color bgcolor, color fgcolor) const;	///< Draw the item. Position and size of inline widgets should be set here

			void update();		///< Update the item (refresh)
			void scroll();		///< Scrolls the view to show this item

			virtual void emit_event();			///< Emit property_changed event

			virtual void enabled(bool state)	///< Enables/disables the item
			{
				en_ = state;
			}
			virtual bool enabled()				///< Get the enables state of the item
			{
				return en_;
			}

		protected:
			virtual void create(window wd) = 0; ///< Creates the inline widgets. Position and size of widgets can be ignored at this stage

			virtual void draw_label(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const;	///< Draw the label of the item
			virtual void draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const;	///< Draw the value of the item
			virtual void draw_ibox(paint::graphics* graph, rectangle rect, color bgcolor, color fgcolor) const;	///< Draw the interaction box of the item

			std::string		label_, value_, def_;
			unsigned		size_{ 24 };
			bool			en_{ true };
			bool			evt_emit_{ true };

			essence_t* ess_;
			index_pair		idx_;

			mutable nana::panel<true>	ibox_;
			mutable nana::menu			menu_;
		};

		typedef std::unique_ptr<pgitem>	pgitem_ptr;
	}
}

#endif