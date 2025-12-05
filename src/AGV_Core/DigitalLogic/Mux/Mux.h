#ifndef AGV_CORE_DIGITAL_LOGIC_MUX_H
#define AGV_CORE_DIGITAL_LOGIC_MUX_H

    #include <stdint.h>

    namespace AGV_Core::DigitalLogic {
        
        // =======================================================
        // Multiplexer (Mux) básico
        // =======================================================
        class Mux {
        public:

            typedef void(*SetupFn)(void);
            typedef void(*SetSelectLinesFn)(uint8_t);

            Mux() noexcept;

            void Init(uint8_t numSelectLines, SetSelectLinesFn Select, SetupFn Setup = nullptr) noexcept;

            // Establece las líneas de selección (valores binarios)
            void SetNumSelectLines(uint8_t value) noexcept;

            // Obtiene el número de líneas de selección
            uint8_t GetNumSelectLines() const noexcept;

            // Establece las líneas de selección (valores binarios)
            void SelectLine(uint8_t value) noexcept;

            // Obtiene el número de canal seleccionado actualmente
            uint8_t GetSelectedLine() const noexcept;

        private:
            uint8_t _numSelectLines;
            uint8_t _selectedChannel;
            SetupFn _Setup;
            SetSelectLinesFn _SetSelectLines;
        };

    }


#endif//AGV_CORE_DIGITAL_LOGIC_MUX_H