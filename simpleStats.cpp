#include "sierrachart.h"


SCDLLName("Simple Stats")

SCSFExport scsf_simpleStats(SCStudyInterfaceRef sc) {

    if (sc.SetDefaults) {
        sc.GraphName = "SimpleStats";
        sc.FreeDLL = 1;

        return;
    }

    sc.AddMessageToLog(sc.Symbol, 0);

    SCString Buffer;
    Buffer.Format("1 %f", sc.BaseData[SC_LAST][sc.ArraySize - 1]);
    sc.AddMessageToLog(Buffer, 0);

    SCString Buffer2;
    Buffer2.Format("2 %f", sc.BaseData[SC_LAST][sc.ArraySize - 2]);
    sc.AddMessageToLog(Buffer2, 0);

    return;

}