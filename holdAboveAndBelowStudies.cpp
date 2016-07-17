#include "sierrachart.h"


struct PercentileGroup {
    float p20;
    float p50;
    float p80;
};

void drawLine(s_sc &sc, PercentileGroup percentileGroup, int lineId, const COLORREF &rectangleColor);

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

//    SCString Buffer;
//    Buffer.Format("p20 %f p50 %f p80 %f", percentileGroup.p20, percentileGroup.p50, percentileGroup.p80);
//
//    sc.AddMessageToLog(Buffer, 0);

//    sc.AddMessageToLog(Buffer, 0);

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


    sc.AddMessageToLog(sc.Symbol, 0);

    int idCounter = 74191;

    if (sc.Symbol.CompareNoCase("EURGBP") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (16.0 / pipDivisor),
                                                       High + (float) (31.0 / pipDivisor),
                                                       High + (float) (59.0 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (3.0 / pipDivisor),
                                                       High + (float) (9.0 / pipDivisor),
                                                       High + (float) (20.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (15.0 / pipDivisor),
                                                       Low - (float) (27.0 / pipDivisor),
                                                       Low - (float) (47.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (3.0 / pipDivisor),
                                                       Low - (float) (9.0 / pipDivisor),
                                                       Low - (float) (16.0 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);

    } else if (sc.Symbol.CompareNoCase("EURUSD") == 0) {

        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (29.4 / pipDivisor),
                                                       High + (float) (49.0 / pipDivisor),
                                                       High + (float) (78.0 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (7.0 / pipDivisor),
                                                       High + (float) (15.0 / pipDivisor),
                                                       High + (float) (31.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (27.0 / pipDivisor),
                                                       Low - (float) (49.0 / pipDivisor),
                                                       Low - (float) (75.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (5.0 / pipDivisor),
                                                       Low - (float) (13.0 / pipDivisor),
                                                       Low - (float) (26.0 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);
    } else if (sc.Symbol.CompareNoCase("EURJPY") == 0) {

        int pipDivisor = 100;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (30.8 / pipDivisor),
                                                       High + (float) (60.5 / pipDivisor),
                                                       High + (float) (99.2 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (6.0 / pipDivisor),
                                                       High + (float) (18.0 / pipDivisor),
                                                       High + (float) (32.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (38.4 / pipDivisor),
                                                       Low - (float) (69.0 / pipDivisor),
                                                       Low - (float) (112.2 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (8.0 / pipDivisor),
                                                       Low - (float) (21.0 / pipDivisor),
                                                       Low - (float) (39.6 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);
    } else if (sc.Symbol.CompareNoCase("GBPUSD") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (34.0 / pipDivisor),
                                                       High + (float) (57.0 / pipDivisor),
                                                       High + (float) (93.0 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (7.0 / pipDivisor),
                                                       High + (float) (17.0 / pipDivisor),
                                                       High + (float) (35.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (34.0 / pipDivisor),
                                                       Low - (float) (58.0 / pipDivisor),
                                                       Low - (float) (102.6 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (6.0 / pipDivisor),
                                                       Low - (float) (15.0 / pipDivisor),
                                                       Low - (float) (30.8 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);

    } else if (sc.Symbol.CompareNoCase("USDCAD") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (20.0 / pipDivisor),
                                                       High + (float) (41.5 / pipDivisor),
                                                       High + (float) (64.8 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (4.0 / pipDivisor),
                                                       High + (float) (12.0 / pipDivisor),
                                                       High + (float) (24.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (20.0 / pipDivisor),
                                                       Low - (float) (37.0 / pipDivisor),
                                                       Low - (float) (67.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (4.0 / pipDivisor),
                                                       Low - (float) (10.0 / pipDivisor),
                                                       Low - (float) (20.8 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);

    } else if (sc.Symbol.CompareNoCase("USDCHF") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (23.0 / pipDivisor),
                                                       High + (float) (39.0 / pipDivisor),
                                                       High + (float) (62.0 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (4.0 / pipDivisor),
                                                       High + (float) (11.0 / pipDivisor),
                                                       High + (float) (24.4 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (23.0 / pipDivisor),
                                                       Low - (float) (41.0 / pipDivisor),
                                                       Low - (float) (75.2 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (3.0 / pipDivisor),
                                                       Low - (float) (11.5 / pipDivisor),
                                                       Low - (float) (24.2 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);

    } else if (sc.Symbol.CompareNoCase("AUDUSD") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (23.8 / pipDivisor),
                                                       High + (float) (42.5 / pipDivisor),
                                                       High + (float) (65.2 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (7.0 / pipDivisor),
                                                       High + (float) (14.0 / pipDivisor),
                                                       High + (float) (26.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (22.2 / pipDivisor),
                                                       Low - (float) (41.5 / pipDivisor),
                                                       Low - (float) (68.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (5.0 / pipDivisor),
                                                       Low - (float) (15.5 / pipDivisor),
                                                       Low - (float) (28.4 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);

    } else if (sc.Symbol.CompareNoCase("NZDUSD") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (27.0 / pipDivisor),
                                                       High + (float) (47.0 / pipDivisor),
                                                       High + (float) (70.0 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (6.0 / pipDivisor),
                                                       High + (float) (14.0 / pipDivisor),
                                                       High + (float) (25.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (22.0 / pipDivisor),
                                                       Low - (float) (40.0 / pipDivisor),
                                                       Low - (float) (69.4 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (4.0 / pipDivisor),
                                                       Low - (float) (12.5 / pipDivisor),
                                                       Low - (float) (26.0 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);
    } else if (sc.Symbol.CompareNoCase("USDJPY") == 0) {
        int pipDivisor = 100;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (21.0 / pipDivisor),
                                                       High + (float) (40.0 / pipDivisor),
                                                       High + (float) (72.0 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (4.0 / pipDivisor),
                                                       High + (float) (11.0 / pipDivisor),
                                                       High + (float) (23.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (28.0 / pipDivisor),
                                                       Low - (float) (47.0 / pipDivisor),
                                                       Low - (float) (89.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (6.0 / pipDivisor),
                                                       Low - (float) (15.0 / pipDivisor),
                                                       Low - (float) (25.0 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);

    } else if (sc.Symbol.CompareNoCase("EURCHF") == 0) {
        int pipDivisor = 10000;

        PercentileGroup percentileGroups[4] = {{
                                                       High + (float) (11.0 / pipDivisor),
                                                       High + (float) (26.0 / pipDivisor),
                                                       High + (float) (51.6 / pipDivisor)
                                               },
                                               {
                                                       High + (float) (1.0 / pipDivisor),
                                                       High + (float) (5.0 / pipDivisor),
                                                       High + (float) (13.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (9.0 / pipDivisor),
                                                       Low - (float) (22.0 / pipDivisor),
                                                       Low - (float) (45.0 / pipDivisor)
                                               },
                                               {
                                                       Low - (float) (1.0 / pipDivisor),
                                                       Low - (float) (5.0 / pipDivisor),
                                                       Low - (float) (13.0 / pipDivisor)
                                               }
        };

        drawLine(sc, percentileGroups[0], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[1], idCounter++, COLOR_YELLOW);
        drawLine(sc, percentileGroups[2], idCounter++, COLOR_SANDYBROWN);
        drawLine(sc, percentileGroups[3], idCounter++, COLOR_YELLOW);
    }
}

