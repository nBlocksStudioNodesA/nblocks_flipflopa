#include "flipflopA.h"

// FLIP FLOP
nBlock_FlipFlopA::nBlock_FlipFlopA(uint32_t triggerMode) {
    _triggerMode = triggerMode;
    // The following 2 variables should be frozen during a frame
    // and should only be changed in the step method (end of frame)
    _exposed_output[0] = 0;
    _exposed_available[0] = 0;

    // If you don't use a FIFO or a buffer of some kind,
    // then use the following 2 variables to queue changes during the frame
    // And then in the step method (end of frame) copy their values to the definitive ones
    _output[0] = 0;
    _available[0] = 0;
    return;
}
uint32_t nBlock_FlipFlopA::outputAvailable(uint32_t outputNumber) { return _exposed_available[outputNumber]; }
uint32_t nBlock_FlipFlopA::readOutput(uint32_t outputNumber) { return _exposed_output[outputNumber]; }

void nBlock_FlipFlopA::triggerInput(nBlocks_Message message) { // inputNumber is ignored
    // If value is adequate to the trigger mode, act on output
    if (
            (_triggerMode == triggerMode_Both) ||                        // if triggers on anything, or
            ((_triggerMode == triggerMode_Positive) && (message.intValue != 0)) ||  // triggers on positive values (and is positive), or
            ((_triggerMode == triggerMode_Zero) && (message.intValue == 0))         // triggers on zero values (and is zero)
    ) {
        // Change output data
        _output[0] = (_output[0]? 0 : 1); // Toggles value
        // flag as output available for the next frame
        _available[0] = 1;
    }
}
void nBlock_FlipFlopA::step(void) {
    int i;
    for (i=0; i < 1/*Number of outputs*/; i++) {
        _exposed_output[i] = _output[i];
        // do not reset _output, this is a memory cell
        _exposed_available[i] = _available[i];
        _available[i] = 0;
    }
    return;
}
