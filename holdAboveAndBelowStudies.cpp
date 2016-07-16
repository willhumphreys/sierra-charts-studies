// The top of every source code file must include this line
#include "sierrachart.h"

// For reference, refer to this page:
// https://www.sierrachart.com/index.php?page=doc/AdvancedCustomStudyInterfaceAndLanguage.php

// This line is required. Change the text within the quote
// marks to what you want to name your group of custom studies.
SCDLLName("Hold Above and Below Studies")

//This is the basic framework of a study function. Change the name 'TemplateFunction' to what you require.
SCSFExport scsf_TemplateFunction(SCStudyInterfaceRef sc) {
    // Section 1 - Set the configuration variables and defaults
    if (sc.SetDefaults) {
        sc.GraphName = "takeOutHighAndHoldAbove";

        // During development set this flag to 1, so the DLL can be rebuilt without restarting Sierra Chart. When development is completed, set it to 0 to improve performance.
        sc.FreeDLL = 1;

        sc.StudyDescription = "Displays the supplied trades as crosses";

        sc.GraphRegion = 0;

        sc.AutoLoop = 0;

        return;
    }

    s_UseTool Tool;
    int UniqueLineNumber = 74191;//any random number.

    Tool.Clear(); // Reset tool structure.  Good practice but unnecessary in this case.
    Tool.ChartNumber = sc.ChartNumber;

    Tool.DrawingType = DRAWING_HORIZONTAL_LINE_NON_EXTENDED;
    Tool.LineStyle = DRAWSTYLE_DASH;
    Tool.LineNumber = UniqueLineNumber + 1;

    Tool.BeginDateTime = 100;
    Tool.EndDateTime = 130;
    Tool.BeginValue = 2148;
    Tool.Color = RGB(255, 255, 255);
    Tool.AddMethod = UTAM_ADD_OR_ADJUST;
    Tool.LineWidth = 2;

    sc.UseTool(Tool);


}

