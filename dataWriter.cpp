#include <fstream>
#include <iostream>
using namespace std;


#include "sierrachart.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd

void logTheCurrentDirectory(s_sc &sc);

#endif

void logTheCurrentDirectory(s_sc &sc) {
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        sc.AddMessageToLog("Didn't work", 0);
        return;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    sc.AddMessageToLog(cCurrentPath, 0);
}


SCDLLName("DataWriter")

SCSFExport scsf_SC_TradingCrossOverExample(SCStudyInterfaceRef sc) {

    SCInputRef line1Ref = sc.Input[0];
    SCInputRef line2Ref = sc.Input[1];

    //logTheCurrentDirectory(sc);

    ofstream myfile;
    myfile.open ("eurusd_out.txt", std::ofstream::app);

    if (sc.SetDefaults) {



        line1Ref.Name = "Daily Low";
        line1Ref.SetStudySubgraphValues(1, 0);

        line2Ref.Name = "Daily High";
        line2Ref.SetStudySubgraphValues(1, 0);

        //myfile.close();

        // Set the configuration and defaults

        sc.GraphName = "DataWriter";

        sc.StudyDescription = "Write the data to file";

        sc.AutoLoop = 1;  // true
        sc.GraphRegion = 0;
        sc.FreeDLL = 1;
        sc.CalculationPrecedence = LOW_PREC_LEVEL;

        sc.AllowMultipleEntriesInSameDirection = false;
        sc.MaximumPositionAllowed = 5;
        sc.SupportReversals = true;

        // This is false by default. Orders will go to the simulation system always.
        sc.SendOrdersToTradeService = false;

        sc.AllowOppositeEntryWithOpposingPositionOrOrders = false;
        sc.SupportAttachedOrdersForTrading = false;

        sc.CancelAllOrdersOnEntriesAndReversals = true;
        sc.AllowEntryWithWorkingOrders = false;
        sc.CancelAllWorkingOrdersOnExit = true;
        sc.AllowOnlyOneTradePerBar = true;

        sc.MaintainTradeStatisticsAndTradesData = true;

        return;
    }


    SCFloatArray dailyLows;
    sc.GetStudyArrayUsingID(line1Ref.GetStudyID(), line1Ref.GetSubgraphIndex(), dailyLows);
    float dailyLow = dailyLows[sc.Index -1];

    SCFloatArray dailyHighs;
    sc.GetStudyArrayUsingID(line2Ref.GetStudyID(), line2Ref.GetSubgraphIndex(), dailyHighs);
    float dailyHigh = dailyHighs[sc.Index -1];

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    sc.BaseDateTimeIn[sc.Index].GetDateTimeYMDHMS(year, month, day, hour, minute, second );
    float LastTradePrice = sc.Close[sc.Index];
    float Open = sc.Open[sc.Index];
    float Low = sc.Low[sc.Index];
    float PreviousLow = sc.Low[sc.Index - 1];
    float High = sc.High[sc.Index];
    float PreviousHigh = sc.High[sc.Index - 1];
    float PreviousClose = sc.Close[sc.Index - 1];

    float LastBarSize = PreviousClose - PreviousLow;

    SCString Buffer2;
    Buffer2.Format("%d-%d-%dT%d:%d:%d,%f,%f,%f,%f,%f,%f\n", year, month, day, hour, minute, second, Open, Low, High, LastTradePrice, dailyLow, dailyHigh);
    //sc.AddMessageToLog(Buffer2, 0);

    myfile << Buffer2;

    s_SCPositionData PositionData;
    int Result = sc.GetTradePosition(PositionData);
}