#include "sierrachart.h"


struct PercentileGroup {
    float p20;
    float p50;
    float p80;
};

void drawLine(s_sc &sc, PercentileGroup percentileGroup, int lineId, const COLORREF &rectangleColor);

int getPipDivisor(SCStudyInterfaceRef sc);

SCDateTime getDayToUse(SCStudyInterfaceRef sc);

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

SCSFExport scsf_NoTradingDays(SCStudyInterfaceRef sc) {

    if (sc.SetDefaults) {
        sc.GraphName = "NoTradingDays";
        sc.FreeDLL = 1;
        sc.StudyDescription = "Set a big message in the middle of screen when I shouldn't be trading.";
        sc.GraphRegion = 0;
        sc.AutoLoop = 0;


        //Higher high close above
        sc.Input[0].Name = "Year 1";
        sc.Input[0].SetInt(2016);
        sc.Input[0].SetIntLimits(2016, 2050);

        //Higher high close above
        sc.Input[1].Name = "Month 1";
        sc.Input[1].SetInt(7);
        sc.Input[1].SetIntLimits(1, 12);

        //Higher high close above
        sc.Input[2].Name = "Day 1";
        sc.Input[2].SetInt(21);
        sc.Input[2].SetIntLimits(1, 31);

        sc.Input[3].Name = "Message 1";
        sc.Input[3].SetString("Hello World 3");


        return;
    }

    int CurrentDate = sc.BaseDateTimeIn[sc.ArraySize - 1].GetDate();

    int DateValue = YMD_DATE(sc.Input[0].GetInt(), sc.Input[1].GetInt(), sc.Input[2].GetInt());

    if (CurrentDate == DateValue) {
        s_UseTool Tool;
        Tool.Clear(); // reset tool structure for our next use
        Tool.ChartNumber = sc.ChartNumber;
        Tool.DrawingType = DRAWING_TEXT;
        Tool.LineNumber = 5035;
        Tool.BeginDateTime = 50;
        Tool.UseRelativeVerticalValues = 1;
        Tool.BeginValue = 50;
        Tool.Color = COLOR_RED;
        Tool.Region = sc.GraphRegion;

        SCString Label = sc.Input[3].GetString();

        Tool.Text.Format("%s", Label.GetChars());
        Tool.FontSize = 70;
        Tool.FontBold = true;
        //Tool.FontFace= "Courier";
        Tool.AddMethod = UTAM_ADD_OR_ADJUST;
        Tool.ReverseTextColor = 0;

        sc.UseTool(Tool);
    }

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
        sc.Input[0].SetIntLimits(1, 200);

        sc.Input[1].Name = "Higher High Close Above 50";
        sc.Input[1].SetInt(25);
        sc.Input[1].SetIntLimits(1, 200);

        sc.Input[2].Name = "Higher High Close Above 80";
        sc.Input[2].SetInt(30);
        sc.Input[2].SetIntLimits(1, 200);

        //Higher high close inside
        sc.Input[3].Name = "Higher High Close Inside 20";
        sc.Input[3].SetInt(10);
        sc.Input[3].SetIntLimits(1, 200);

        sc.Input[4].Name = "Higher High Close Inside 50";
        sc.Input[4].SetInt(15);
        sc.Input[4].SetIntLimits(1, 200);

        sc.Input[5].Name = "Higher High Close Inside 80";
        sc.Input[5].SetInt(20);
        sc.Input[5].SetIntLimits(1, 200);

        //Lower low close inside
        sc.Input[6].Name = "Loser Low Close Inside 20";
        sc.Input[6].SetInt(10);
        sc.Input[6].SetIntLimits(1, 200);

        sc.Input[7].Name = "Loser Low Close Inside 50";
        sc.Input[7].SetInt(15);
        sc.Input[7].SetIntLimits(1, 200);

        sc.Input[8].Name = "Loser Low Close Inside 80";
        sc.Input[8].SetInt(20);
        sc.Input[8].SetIntLimits(1, 200);

        //Lower low close below
        sc.Input[9].Name = "Loser Low Close Below 20";
        sc.Input[9].SetInt(20);
        sc.Input[9].SetIntLimits(1, 200);

        sc.Input[10].Name = "Loser Low Close Below 50";
        sc.Input[10].SetInt(25);
        sc.Input[10].SetIntLimits(1, 200);

        sc.Input[11].Name = "Loser Low Close Below 80";
        sc.Input[11].SetInt(30);
        sc.Input[11].SetIntLimits(1, 200);


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


    float Open;
    float High;
    float Low;
    float Close;
    float Volume;


    SCDateTime DayToUse = getDayToUse(sc);

    int dateRetrieved = sc.GetOpenHighLowCloseVolumeForDate(DayToUse, Open, High, Low, Close, Volume);

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


SCDateTime getDayToUse(SCStudyInterfaceRef sc) {

    SCDateTime currentDateTime = sc.CurrentSystemDateTime;
    SCDateTime DayToUse;
    //If today is Sunday or Monday(1) we want to use Friday.
    //Otherwise use the previous day.
    if (sc.CurrentSystemDateTime.IsSunday()) {
        DayToUse = currentDateTime - 2 * DAYS;
    }
    else if (sc.CurrentSystemDateTime.GetDayOfWeek() == 1) {
        DayToUse = currentDateTime - 3 * DAYS;
    } else {
        DayToUse = currentDateTime - 1 * DAYS;
    }

    return DayToUse;
}


