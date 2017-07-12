/**
 * $Id: $
 *
 * @brief Red Pitaya Scpi server SCPI configuration and general command implementation
 *
 * @Author Red Pitaya
 *
 * (c) Red Pitaya  http://www.redpitaya.com
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "scpi/error.h"
#include "scpi/ieee488.h"
#include "scpi/minimal.h"
#include "scpi/units.h"
#include "scpi/parser.h"

#include "redpitaya/rp1.h"

#include "scpi-commands.h"
#include "api_cmd.h"
#include "common.h"
#include "scpi_gen.h"

bool RST_executed = FALSE;

/**
 * Interface general commands
 */
size_t SCPI_Write(scpi_t * context, const char * data, size_t len) {
    size_t total = 0;
    rpscpi_context_t *rp = (rpscpi_context_t *) context->user_context;

    if (rp->connfd != 0) {
        while (len > 0) {
            size_t written =  write(rp->connfd, data, len);
            if (written < 0) {
                syslog(LOG_ERR,
                    "Failed to write into the socket. Should send %zu bytes. Could send only %zu bytes",
                    len, written);
                return total;
            }
            len -= written;
            data += written;
            total += written;
        }
    }
    return total;
}

scpi_result_t SCPI_Flush(scpi_t * context) {
    return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int_fast16_t err) {
    return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
    if (SCPI_CTRL_SRQ == ctrl) {
        syslog(LOG_ERR, "**SRQ not implemented");
    } else {
         syslog(LOG_ERR, "**CTRL not implemented");
    }

    return SCPI_RES_ERR;
}

scpi_result_t SCPI_Reset(scpi_t * context) {
    /* Terminating all scpi operations */
    (void) context;
    RP_LOG(LOG_INFO, "*RST Sucsessfuly reset scpi server.");
    return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemCommTcpipControlQ(scpi_t * context) {
    syslog(LOG_ERR, "**SCPI_SystemCommTcpipControlQ not implemented");
    return SCPI_RES_ERR;
}

/**
 * SCPI Configuration
 */

const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { .pattern = "*CLS" , .callback = SCPI_CoreCls,},
    { .pattern = "*ESE" , .callback = SCPI_CoreEse,},
    { .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
    { .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
    { .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
    { .pattern = "*OPC" , .callback = SCPI_CoreOpc,},
    { .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
    { .pattern = "*RST" , .callback = SCPI_CoreRst,},
    { .pattern = "*SRE" , .callback = SCPI_CoreSre,},
    { .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
    { .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
    { .pattern = "*TST?", .callback = SCPI_CoreTstQ,},
    { .pattern = "*WAI" , .callback = SCPI_CoreWai,},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {.pattern = "SYSTem:ERRor[:NEXT]?", .callback = SCPI_SystemErrorNextQ,},
    {.pattern = "SYSTem:ERRor:COUNt?",  .callback = SCPI_SystemErrorCountQ,},
    {.pattern = "SYSTem:VERSion?",      .callback = SCPI_SystemVersionQ,},

    {.pattern = "STATus:QUEStionable[:EVENt]?", .callback = SCPI_StatusQuestionableEventQ,},
    {.pattern = "STATus:QUEStionable:ENABle",   .callback = SCPI_StatusQuestionableEnable,},
    {.pattern = "STATus:QUEStionable:ENABle?",  .callback = SCPI_StatusQuestionableEnableQ,},
    {.pattern = "STATus:PRESet",                .callback = SCPI_StatusPreset,},

    {.pattern = "SYSTem:COMMunication:TCPIP:CONTROL?", .callback = SCPI_SystemCommTcpipControlQ,},

    /* RedPitaya */

    /* General commands */
//    {.pattern = "RP:INit", .callback                    = RP_InitAll,},
//    {.pattern = "RP:REset", .callback                   = RP_ResetAll,},
//    {.pattern = "RP:RELease", .callback                 = RP_ReleaseAll,},
//    {.pattern = "RP:FPGABITREAM", .callback             = RP_FpgaBitStream,},
//    {.pattern = "RP:DIg[:loop]", .callback              = RP_EnableDigLoop,},

//    /* Acquire */
//    {.pattern = "ACQ:START", .callback                  = RP_AcqStart,},
//    {.pattern = "ACQ:STOP", .callback                   = RP_AcqStop,},
//    {.pattern = "ACQ:RST", .callback                    = RP_AcqReset,},
//    {.pattern = "ACQ:DEC", .callback                    = RP_AcqDecimation,},
//    {.pattern = "ACQ:DEC?", .callback                   = RP_AcqDecimationQ,},
//    {.pattern = "ACQ:SRAT?", .callback                  = RP_AcqSamplingRateHzQ,},
//    {.pattern = "ACQ:AVG", .callback                    = RP_AcqAveraging,},
//    {.pattern = "ACQ:AVG?", .callback                   = RP_AcqAveragingQ,},
//    {.pattern = "ACQ:TRIGger", .callback                = RP_AcqTriggerSrc,},
//    {.pattern = "ACQ:TRIGger:STAT?", .callback          = RP_AcqTriggerSrcQ,},
//    {.pattern = "ACQ:TRIGger:DLY", .callback            = RP_AcqTriggerDelay,},
//    {.pattern = "ACQ:TRIGger:DLY?", .callback           = RP_AcqTriggerDelayQ,},
//    {.pattern = "ACQ:TRIGger:DLY:NS", .callback         = RP_AcqTriggerDelayNs,},
//    {.pattern = "ACQ:TRIGger:DLY:NS?", .callback        = RP_AcqTriggerDelayNsQ,},
//    {.pattern = "ACQ:TRIGger:HYST", .callback           = RP_AcqTriggerHyst,},
//    {.pattern = "ACQ:TRIGger:HYST?", .callback          = RP_AcqTriggerHystQ,},
//    {.pattern = "ACQ:SOURce#:GAIN", .callback           = RP_AcqGain,},
//    {.pattern = "ACQ:SOURce#:GAIN?", .callback          = RP_AcqGainQ,},
//    {.pattern = "ACQ:TRIG:LEV", .callback               = RP_AcqTriggerLevel,},
//    {.pattern = "ACQ:TRIG:LEV?", .callback              = RP_AcqTriggerLevelQ,},
//    {.pattern = "ACQ:WPOS?", .callback                  = RP_AcqWritePointerQ,},
//    {.pattern = "ACQ:TPOS?", .callback                  = RP_AcqWritePointerAtTrigQ,},
//    {.pattern = "ACQ:DATA:UNITS", .callback             = RP_AcqScpiDataUnits,},
//    {.pattern = "ACQ:DATA:UNITS?", .callback            = RP_AcqScpiDataUnitsQ,},
//    {.pattern = "ACQ:DATA:FORMAT", .callback            = RP_AcqSetDataFormat,},
//    {.pattern = "ACQ:SOURce#:DATA:STA:END?", .callback  = RP_AcqDataPosQ,},
//    {.pattern = "ACQ:SOURce#:DATA:STA:N?", .callback    = RP_AcqDataQ,},
//    {.pattern = "ACQ:SOURce#:DATA:OLD:N?", .callback    = RP_AcqOldestDataQ,},
//    {.pattern = "ACQ:SOURce#:DATA?", .callback          = RP_AcqDataOldestAllQ,},
//    {.pattern = "ACQ:SOURce#:DATA:LAT:N?", .callback    = RP_AcqLatestDataQ,},
//    {.pattern = "ACQ:BUF:SIZE?", .callback              = RP_AcqBufferSizeQ,},

    /* Generate */
    {.pattern = "GEN:RST", .callback                    = rpscpi_gen_reset,},
    {.pattern = "OUTPUT#[:STATe]",                            .callback = rpscpi_gen_set_enable,},
    {.pattern = "OUTPUT#[:STATe]?",                           .callback = rpscpi_gen_get_enable,},
    {.pattern = "[SOURce#]:FREQuency[:FIXed]",                .callback = rpscpi_gen_set_frequency,},
    {.pattern = "[SOURce#]:FREQuency[:FIXed]?",               .callback = rpscpi_gen_get_frequency,},
    {.pattern = "[SOURce#]:FUNCtion[:SHAPe]",                 .callback = rpscpi_gen_set_waveform_tag,},
//    {.pattern = "[SOURce#]:FUNCtion[:SHAPe]?",                .callback = rpscpi_gen_get_waveform_tag,},
    {.pattern = "[SOURce#]:VOLTage[:IMMediate][:AMPlitude]",  .callback = rpscpi_gen_set_amplitude,},
    {.pattern = "[SOURce#]:VOLTage[:IMMediate][:AMPlitude]?", .callback = rpscpi_gen_get_amplitude,},
    {.pattern = "[SOURce#]:VOLTage[:IMMediate]:OFFSet",       .callback = rpscpi_gen_set_offset,},
    {.pattern = "[SOURce#]:VOLTage[:IMMediate]:OFFSet?",      .callback = rpscpi_gen_get_offset,},
    {.pattern = "[SOURce#]:PHASe[:ADJust]",                   .callback = rpscpi_gen_set_phase,},
    {.pattern = "[SOURce#]:PHASe[:ADJust]?",                  .callback = rpscpi_gen_get_phase,},
//  {.pattern = "[SOURce#]:PHASe:INITiate",                   .callback = RP_GenPhaseInit,},
//    {.pattern = "[SOURce#]:FUNCtion[:SQUare]:DCYCle",         .callback = RP_GenDutyCycle,},
//    {.pattern = "[SOURce#]:FUNCtion[:SQUare]:DCYCle?",        .callback = RP_GenDutyCycleQ,},
//    {.pattern = "[SOURce#]:FUNCtion[:TRIangle]:DCYCle",       .callback = RP_GenDutyCycle,},
//    {.pattern = "[SOURce#]:FUNCtion[:TRIangle]:DCYCle?",      .callback = RP_GenDutyCycleQ,},
//    {.pattern = "[SOURce#]:TRACe:DATA[:DATA]",                .callback = RP_GenArbitraryWaveForm,},
//    {.pattern = "[SOURce#]:TRACe:DATA[:DATA]?",               .callback = RP_GenArbitraryWaveFormQ,},
//    {.pattern = "[SOURce#]:BURSt[:STATe]",                    .callback = RP_GenGenerateMode,},
//    {.pattern = "[SOURce#]:BURSt[:STATe]?",                   .callback = RP_GenGenerateModeQ,},
//    {.pattern = "[SOURce#]:BURSt:NCYC", .callback          = RP_GenBurstCount,},
//    {.pattern = "[SOURce#]:BURSt:NCYC?", .callback         = RP_GenBurstCountQ,},
//    {.pattern = "[SOURce#]:BURSt:NOR", .callback           = RP_GenBurstRepetitions,},
//    {.pattern = "[SOURce#]:BURSt:NOR?", .callback          = RP_GenBurstRepetitionsQ,},
//    {.pattern = "[SOURce#]:BURSt:INTernal:PERiod", .callback       = RP_GenBurstPeriod,},
//    {.pattern = "[SOURce#]:BURSt:INTernal:PERiod?", .callback      = RP_GenBurstPeriodQ,},
//    {.pattern = "SOURce#:TRIGger:SOUR", .callback       = RP_GenTriggerSource,},
//    {.pattern = "SOURce#:TRIGger:SOUR?", .callback      = RP_GenTriggerSourceQ,},
//    {.pattern = "SOURce#:TRIGger:IMM", .callback        = RP_GenTrigger,},

    SCPI_CMD_LIST_END
};

// TODO
// SHAPe should provide some kind of enumerated value parser

scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

scpi_t scpi_context;

