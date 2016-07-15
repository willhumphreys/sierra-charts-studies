#include "sierrachart.h"

struct trade {
    SCDateTime filledTime;
    float filledLevel;
    int red;
    int green;
    int blue;
    SCString toolTipText;


};

SCDLLName("Draw Trades Studies")

SCSFExport scsf_UseToolExample(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "Display Trades";

        sc.StudyDescription = "Displays the supplied trades as crosses";

        sc.GraphRegion = 0;

        //During development set this flag to 1, so the DLL can be modified. When development is completed, set it to 0 to improve performance.
        sc.FreeDLL = 1;


        sc.AutoLoop = 0;

        return;
    }

    const int UniqueLineNumber =57683541;


    //int tradeCount = <TRADE_COUNT>;
    int tradeCount = 5;

    if (sc.LastCallToFunction)
    {
        for (int x = 0; x < tradeCount; x++) {
            // Be sure to remove the Text drawing added as a user drawn drawing
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, UniqueLineNumber + x);
            sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, UniqueLineNumber + 1000 + x);
        }
        return;//No further processing needed in this case.
    }


    //<MARKERS>

    // Do data processing
    int BarIndex;

    s_UseTool Tool;

    //SCDateTime MySCDateTime3;
    for (int x = 0; x < tradeCount; x++) {

        SCString str;
        str.Format("Counter:%d,",x);

        sc.AddMessageToLog(str, 0);

        // Marker example
        // There is no need to define 's_UseTool Tool;' since we already did above.
        Tool.Clear(); // reset tool structure for our next use
        Tool.ChartNumber = sc.ChartNumber;
        Tool.DrawingType = DRAWING_MARKER;
        Tool.LineNumber =  UniqueLineNumber + x;

        Tool.BeginDateTime = trades[x].filledTime;
        Tool.BeginValue = trades[x].filledLevel;
        Tool.Color = RGB(trades[x].red, trades[x].green, trades[x].blue);
        Tool.AddMethod = UTAM_ADD_OR_ADJUST;
        Tool.MarkerType = MARKER_X;
        Tool.MarkerSize = 8;
        Tool.LineWidth = 2;

        sc.UseTool(Tool);

        // Display some text after the end of the chart
        // There is no need to define 's_UseTool Tool;' since we already did above.
        Tool.Clear(); // reset tool structure for our next use
        Tool.ChartNumber = sc.ChartNumber;
        Tool.DrawingType = DRAWING_TEXT;
        Tool.LineNumber = UniqueLineNumber + 1000 + x;
        Tool.BeginDateTime = trades[x].filledTime;
        Tool.BeginValue = trades[x].filledLevel;

        Tool.Color = RGB(255,255,255);  // White;
        Tool.Text = trades[x].toolTipText;
        Tool.AddMethod = UTAM_ADD_OR_ADJUST;
        Tool.ReverseTextColor = 1;  // true

        sc.UseTool(Tool);
    }
}
