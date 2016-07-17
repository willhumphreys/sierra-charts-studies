#include "sierrachart.h"

struct PercentileGroup {
    float p20;
    float p50;
    float p80;
};

void drawLine(s_sc &sc, PercentileGroup percentileGroup, int lineId, const COLORREF &rectangleColor);

SCDLLName("Hold Above and Below Studies")

void drawLine(s_sc &sc, PercentileGroup percentileGroup, int lineId, const COLORREF &rectangleColor) {
    s_UseTool Tool;

    Tool.Clear();
    Tool.ChartNumber = sc.ChartNumber;

    Tool.DrawingType = DRAWING_RECTANGLEHIGHLIGHT;
    Tool.LineNumber = lineId;

    Tool.TransparencyLevel = 50;
    int BarIndex;

    int CurrentDate = sc.BaseDateTimeIn[sc.ArraySize - 1].GetDate();

    SCDateTime TimeToCheckFor;

    TimeToCheckFor.SetDate(CurrentDate);
    TimeToCheckFor.SetTimeHMS(0, 0, 0);


    // Update BarIndex to 30 bars from the end
    BarIndex = max(sc.ArraySize - 50, 0);
    Tool.BeginDateTime = TimeToCheckFor;
    BarIndex = max(sc.ArraySize, 0);
    Tool.EndDateTime = sc.BaseDateTimeIn[BarIndex];

    SCString Buffer;
    Buffer.Format("p20 %f p50 %f p80 %f", percentileGroup.p20, percentileGroup.p50, percentileGroup.p80);

    sc.AddMessageToLog(Buffer, 0);

    Tool.BeginValue = percentileGroup.p20;
    Tool.EndValue = percentileGroup.p80;

    Tool.ShowPrice = 1;

    Tool.Color = rectangleColor;
    Tool.SecondaryColor = rectangleColor;
    Tool.AddMethod = UTAM_ADD_OR_ADJUST;
    Tool.LineWidth = 1;
    sc.UseTool(Tool);

    //Middle line
    Tool.Clear(); // Reset tool structure.  Good practice but unnecessary in this case.
    Tool.ChartNumber = sc.ChartNumber;

    Tool.DrawingType = DRAWING_HORIZONTAL_LINE_NON_EXTENDED;
    Tool.LineStyle = DRAWSTYLE_DASH;
    Tool.LineNumber = lineId * 2;

    Tool.BeginDateTime = TimeToCheckFor;
    Tool.EndDateTime = sc.BaseDateTimeIn[BarIndex];
    Tool.BeginValue = percentileGroup.p50;
    Tool.Color = rectangleColor;
    Tool.AddMethod = UTAM_ADD_OR_ADJUST;
    Tool.LineWidth = 2;
    sc.UseTool(Tool);

}

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

    SCDateTime Friday = SCDateTime(2016, 7, 14, 15, 0, 0);

    float Open;
    float High;
    float Low;
    float Close;
    float Volume;

    int dateRetrieved = sc.GetOpenHighLowCloseVolumeForDate(Friday, Open, High, Low, Close, Volume);
    int pipDivisor = 10000;

    sc.AddMessageToLog(sc.Symbol, 0);

    int idCounter = 74191;

    if (sc.Symbol.CompareNoCase("SPX500") == 0) {

        SCString Buffer;
        Buffer.Format("Found SPX. Daily high is Day %d Result %d Open %f High %f Low %f Close %f Volume %f",
                      Friday.GetDay(), dateRetrieved, Open, High, Low, Close, Volume);

        sc.AddMessageToLog(Buffer, 0);
    } else if (sc.Symbol.CompareNoCase("EURUSD") == 0) {

        PercentileGroup eurUsdTakeOutHighHoldAbove;
        eurUsdTakeOutHighHoldAbove.p20 = High + (29.4 / pipDivisor);
        eurUsdTakeOutHighHoldAbove.p50 = High + (49.0 / pipDivisor);
        eurUsdTakeOutHighHoldAbove.p80 = High + (78.0 / pipDivisor);

        PercentileGroup eurUsdTakeOutHighHoldInside;
        eurUsdTakeOutHighHoldInside.p20 = High + (7.0 / pipDivisor);
        eurUsdTakeOutHighHoldInside.p50 = High + (15.0 / pipDivisor);
        eurUsdTakeOutHighHoldInside.p80 = High + (31.0 / pipDivisor);

        PercentileGroup eurUsdTakeOutLowHoldBelow;
        eurUsdTakeOutLowHoldBelow.p20 = Low - (27.0 / pipDivisor);
        eurUsdTakeOutLowHoldBelow.p50 = Low - (49.0 / pipDivisor);
        eurUsdTakeOutLowHoldBelow.p80 = Low - (75.0 / pipDivisor);

        PercentileGroup eurUsdTakeOutLowHoldInside;
        eurUsdTakeOutLowHoldInside.p20 = Low - (5.0 / pipDivisor);
        eurUsdTakeOutLowHoldInside.p50 = Low - (13.0 / pipDivisor);
        eurUsdTakeOutLowHoldInside.p80 = Low - (26.0 / pipDivisor);

        drawLine(sc, eurUsdTakeOutHighHoldAbove, idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, eurUsdTakeOutHighHoldInside, idCounter++, COLOR_YELLOW);

        drawLine(sc, eurUsdTakeOutLowHoldBelow, idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, eurUsdTakeOutLowHoldInside, idCounter++, COLOR_YELLOW);
    }
}

