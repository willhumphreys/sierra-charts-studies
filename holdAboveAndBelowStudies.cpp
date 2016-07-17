// The top of every source code file must include this line
#include "sierrachart.h"

// For reference, refer to this page:
// https://www.sierrachart.com/index.php?page=doc/AdvancedCustomStudyInterfaceAndLanguage.php

// This line is required. Change the text within the quote
// marks to what you want to name your group of custom studies.
void drawLine(s_sc &sc, float high, int lineId);

SCDLLName("Hold Above and Below Studies")

void drawLine(s_sc &sc, float low, float high, float lineLevel, int lineId) {
    s_UseTool Tool;

    Tool.Clear(); // Reset tool structure.  Good practice but unnecessary in this case.
    Tool.ChartNumber = sc.ChartNumber;

    Tool.DrawingType = DRAWING_RECTANGLEHIGHLIGHT;
    Tool.LineNumber = lineId;

    Tool.TransparencyLevel = 50;
    int BarIndex;

    // Update BarIndex to 30 bars from the end
    BarIndex = max(sc.ArraySize - 50, 0);
    Tool.BeginDateTime = sc.BaseDateTimeIn[BarIndex];
    BarIndex = max(sc.ArraySize, 0);
    Tool.EndDateTime = sc.BaseDateTimeIn[BarIndex];


    Tool.BeginValue = low;
    Tool.EndValue = high;

    Tool.ShowPrice = 1;

    Tool.Color = RGB(255, 255, 0);
    Tool.SecondaryColor = RGB(255, 0, 255);
    Tool.AddMethod = UTAM_ADD_OR_ADJUST;
    Tool.LineWidth = 1;
    sc.UseTool(Tool);

    //Middle line
    Tool.Clear(); // Reset tool structure.  Good practice but unnecessary in this case.
    Tool.ChartNumber = sc.ChartNumber;

    Tool.DrawingType = DRAWING_HORIZONTAL_LINE_NON_EXTENDED;
    Tool.LineStyle = DRAWSTYLE_DASH;
    Tool.LineNumber = lineId * 2;

    Tool.BeginDateTime = 100;
    Tool.EndDateTime = sc.BaseDateTimeIn[BarIndex];
    Tool.BeginValue = lineLevel;
    Tool.Color = RGB(255, 255, 255);
    Tool.AddMethod = UTAM_ADD_OR_ADJUST;
    Tool.LineWidth = 2;
    sc.UseTool(Tool);

}



//This is the basic framework of a study function. Change the name 'TemplateFunction' to what you require.
SCSFExport scsf_TemplateFunction(SCStudyInterfaceRef sc) {
    // Section 1 - Set the configuration variables and defaults
    if (sc.SetDefaults) {
        sc.GraphName = "takeOutHighAndHoldAbove";

        // During development set this flag to 1, so the DLL can be rebuilt without restarting Sierra Chart. When development is completed, set it to 0 to improve performance.
        sc.FreeDLL = 1;
        sc.StudyDescription = "Show expected reversal and hold levels";
        sc.GraphRegion = 0;
        sc.AutoLoop = 0;

        return;
    }

    struct PercentileGroup {
        float p20;
        float p50;
        float p80;
    };

    struct SymbolPercentile {
        SCString symbol;
        PercentileGroup takeOutHighHoldAbove;
        PercentileGroup takeOutHighHoldInside;
        PercentileGroup takeOutLowHoldBelow;
        PercentileGroup takeOutLowHoldInside;

    } eurusd, eurgbp, eurjpy, gbpusd, usdcad, usdchf, audusd, nzdusd, usdjpy, eurchf;

    PercentileGroup eurUsdTakeOutHighHoldAbove;
    eurUsdTakeOutHighHoldAbove.p20 = 29.4;
    eurUsdTakeOutHighHoldAbove.p50 = 49;
    eurUsdTakeOutHighHoldAbove.p80 = 78;

    PercentileGroup eurUsdTakeOutHighHoldInside;
    eurUsdTakeOutHighHoldInside.p20 = 7;
    eurUsdTakeOutHighHoldInside.p50 = 15;
    eurUsdTakeOutHighHoldInside.p80 = 31;

    PercentileGroup eurUsdTakeOutLowHoldBelow;
    eurUsdTakeOutLowHoldBelow.p20 = 27;
    eurUsdTakeOutLowHoldBelow.p50 = 49;
    eurUsdTakeOutLowHoldBelow.p80 = 75;

    PercentileGroup eurUsdTakeOutLowHoldInside;
    eurUsdTakeOutLowHoldInside.p20 = 5;
    eurUsdTakeOutLowHoldInside.p50 = 13;
    eurUsdTakeOutLowHoldInside.p80 = 26;

    eurusd.symbol = "EURUSD";
    eurusd.takeOutHighHoldAbove = eurUsdTakeOutHighHoldAbove;
    eurusd.takeOutHighHoldInside = eurUsdTakeOutHighHoldInside;
    eurusd.takeOutLowHoldBelow = eurUsdTakeOutLowHoldBelow;
    eurusd.takeOutLowHoldInside = eurUsdTakeOutLowHoldInside;

    SCDateTime Friday = SCDateTime(2016, 7, 14, 15, 0, 0);

    float Open;
    float High;
    float Low;
    float Close;
    float Volume;

    int dateRetrieved = sc.GetOpenHighLowCloseVolumeForDate(Friday, Open, High, Low, Close, Volume);

    sc.AddMessageToLog(sc.Symbol, 0);

    int idCounter = 74191;

    if (sc.Symbol.CompareNoCase("SPX500") == 0) {

        SCString Buffer;
        Buffer.Format("Found SPX. Daily high is Day %d Result %d Open %f High %f Low %f Close %f Volume %f", Friday.GetDay(), dateRetrieved, Open, High, Low, Close, Volume);

        sc.AddMessageToLog(Buffer, 0);
    } else if(sc.Symbol.CompareNoCase("EURUSD") == 0){
        drawLine(sc, Low + eurUsdTakeOutHighHoldAbove.p20 / 10000, High + eurUsdTakeOutHighHoldAbove.p20 / 10000, High + eurUsdTakeOutHighHoldAbove.p50 / 10000, idCounter);
//        drawLine(sc, High + eurUsdTakeOutHighHoldAbove.p50 / 10000, idCounter++);
//        drawLine(sc, High + eurUsdTakeOutHighHoldAbove.p80 / 10000, idCounter++);
//
//        drawLine(sc, High + eurUsdTakeOutHighHoldInside.p20 / 10000, idCounter++);
//        drawLine(sc, High + eurUsdTakeOutHighHoldInside.p50 / 10000, idCounter++);
//        drawLine(sc, High + eurUsdTakeOutHighHoldInside.p80 / 10000, idCounter++);
//
//        drawLine(sc, Low - eurUsdTakeOutLowHoldBelow.p20 / 10000, idCounter++);
//        drawLine(sc, Low - eurUsdTakeOutLowHoldBelow.p50 / 10000, idCounter++);
//        drawLine(sc, Low - eurUsdTakeOutLowHoldBelow.p80 / 10000, idCounter++);
//
//        drawLine(sc, Low - eurUsdTakeOutLowHoldInside.p20 / 10000, idCounter++);
//        drawLine(sc, Low - eurUsdTakeOutLowHoldInside.p50 / 10000, idCounter++);
//        drawLine(sc, Low - eurUsdTakeOutLowHoldInside.p80 / 10000, idCounter++);
    }
}

