
#include "Cell.h"
#include "OtherCells.h"
#include "NoActionCell.h"

ICell* ICell::create(CellType _ctype)
{
	switch (_ctype)
	{
	case CellType::CORNER:	return new NoActionCell();		
	case CellType::SEA:		return new SeaCell();
	default: 
		return nullptr;
	}
}

Cell::Cell() : m_mask(0)
{
	setZValue(default_z_value);
}

Cell::~Cell()
{
}

void Cell::set_side_size(size_t _side)
{
	CenteredRoundedRect::set_draw_rect(QRectF(0, 0, _side, _side));
}

BitMask Cell::mask() const
{
	return m_mask;
}

void Cell::set_mask(BitMask _mask)
{
	m_mask = _mask;
}
