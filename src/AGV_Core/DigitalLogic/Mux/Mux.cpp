#include "Mux.h"

namespace AGV_Core::DigitalLogic {

// =======================================================
// Constructor
// =======================================================
Mux::Mux() noexcept
    : _numSelectLines(0),
      _selectedChannel(0)
{}



void Mux::Init(uint8_t numSelectLines, SetSelectLinesFn Select, SetupFn Setup) noexcept
{
    _numSelectLines = numSelectLines;

    if (Setup)
        _Setup = Setup;
    
    if (Select)
        _SetSelectLines = Select;

    if (_Setup)
        _Setup();
}

// =======================================================
// SetNumSelectLines
// =======================================================
void Mux::SetNumSelectLines(uint8_t value) noexcept
{
    _numSelectLines = value;

    // Si el canal actual excede el nuevo rango, regresarlo a 0
    uint8_t maxChannel = (1 << _numSelectLines) - 1;
    if (_selectedChannel > maxChannel)
        _selectedChannel = 0;
}

// =======================================================
// GetNumSelectLines
// =======================================================
uint8_t Mux::GetNumSelectLines() const noexcept
{
    return _numSelectLines;
}

// =======================================================
// SetSelectLines
// =======================================================
void Mux::SelectLine(uint8_t value) noexcept
{
    // Enmascara solo los bits necesarios
    _selectedChannel = value;
    if(_SetSelectLines)
        _SetSelectLines(_selectedChannel);
}

// =======================================================
// GetSelectedChannel
// =======================================================
uint8_t Mux::GetSelectedLine() const noexcept
{
    return _selectedChannel;
}

} // namespace AGV_Core::DigitalLogic
