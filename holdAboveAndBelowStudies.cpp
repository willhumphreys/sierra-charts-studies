#include "sierrachart.h"


struct PercentileGroup {
    float p20;
    float p50;
    float p80;
};

void drawLine(s_sc &sc, PercentileGroup percentileGroup, int lineId, const COLORREF &rectangleColor);

int getPipDivisor(SCStudyInterfaceRef sc);

SCDLLName("Hold Above and Below Studies")

void drawLine(s_sc &sc, PercentileGroup percentileGroup, int lineId, const COLORREF &rectangleColor) {
    int pipDivisor = 10000;
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


        //Higher high close above
        sc.Input[0].Name = "Higher High Close Above 20";
        sc.Input[0].SetInt(20);
        sc.Input[0].SetIntLimits(1, 100);

        sc.Input[1].Name = "Higher High Close Above 50";
        sc.Input[1].SetInt(25);
        sc.Input[1].SetIntLimits(1, 100);

        sc.Input[2].Name = "Higher High Close Above 80";
        sc.Input[2].SetInt(30);
        sc.Input[2].SetIntLimits(1, 100);

        //Higher high close inside
        sc.Input[3].Name = "Higher High Close Inside 20";
        sc.Input[3].SetInt(10);
        sc.Input[3].SetIntLimits(1, 100);

        sc.Input[4].Name = "Higher High Close Inside 50";
        sc.Input[4].SetInt(15);
        sc.Input[4].SetIntLimits(1, 100);

        sc.Input[5].Name = "Higher High Close Inside 80";
        sc.Input[5].SetInt(20);
        sc.Input[5].SetIntLimits(1, 100);

        //Lower low close inside
        sc.Input[6].Name = "Loser Low Close Inside 20";
        sc.Input[6].SetInt(10);
        sc.Input[6].SetIntLimits(1, 100);

        sc.Input[7].Name = "Loser Low Close Inside 50";
        sc.Input[7].SetInt(15);
        sc.Input[7].SetIntLimits(1, 100);

        sc.Input[8].Name = "Loser Low Close Inside 80";
        sc.Input[8].SetInt(20);
        sc.Input[8].SetIntLimits(1, 100);

        //Lower low close below
        sc.Input[9].Name = "Loser Low Close Below 20";
        sc.Input[9].SetInt(20);
        sc.Input[9].SetIntLimits(1, 100);

        sc.Input[10].Name = "Loser Low Close Below 50";
        sc.Input[10].SetInt(25);
        sc.Input[10].SetIntLimits(1, 100);

        sc.Input[11].Name = "Loser Low Close Below 80";
        sc.Input[11].SetInt(30);
        sc.Input[11].SetIntLimits(1, 100);


        return;
    }

    //Don't process the bars other than the most recent more than one.
    int &LastBarIndexProcessed = sc.PersistVars->i1;
    if (sc.Index == 0)
        LastBarIndexProcessed = -1;

    if (sc.Index == LastBarIndexProcessed) {
        return;
    }

    LastBarIndexProcessed = sc.Index;

    SCDateTime Friday = SCDateTime(2016, 7, 15, 0, 0, 0);

    float Open;
    float High;
    float Low;
    float Close;
    float Volume;

    int dateRetrieved = sc.GetOpenHighLowCloseVolumeForDate(Friday, Open, High, Low, Close, Volume);


    //sc.AddMessageToLog(sc.Symbol, 0);

    int idCounter = 74191;

    int pipDivisor = getPipDivisor(sc);

    drawLine(sc,
             {
                     High + sc.Input[0].GetFloat() / pipDivisor,
                     High + sc.Input[1].GetFloat() / pipDivisor,
                     High + sc.Input[2].GetFloat() / pipDivisor,
             },
             idCounter++, COLOR_SANDYBROWN);
    drawLine(sc,
             {
                     High + sc.Input[3].GetFloat() / pipDivisor,
                     High + sc.Input[4].GetFloat() / pipDivisor,
                     High + sc.Input[5].GetFloat() / pipDivisor,
             },
             idCounter++, COLOR_YELLOW);
    drawLine(sc,
             {
                     Low - sc.Input[6].GetFloat() / pipDivisor,
                     Low - sc.Input[7].GetFloat() / pipDivisor,
                     Low - sc.Input[8].GetFloat() / pipDivisor,
             }, idCounter++, COLOR_SANDYBROWN);
    drawLine(sc,
             {
                     Low - sc.Input[9].GetFloat() / pipDivisor,
                     Low - sc.Input[10].GetFloat() / pipDivisor,
                     Low - sc.Input[11].GetFloat() / pipDivisor,
             }, idCounter++, COLOR_YELLOW);
}

int getPipDivisor(SCStudyInterfaceRef sc) {
    if (sc.Symbol.CompareNoCase("USDJPY") == 0 || sc.Symbol.CompareNoCase("EURJPY") == 0) {
       return 100;
    } else {
        return 10000;
    }
}



