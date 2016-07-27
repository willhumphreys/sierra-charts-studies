#include "sierrachart.h"


SCDLLName("News Studies")

struct NewsItem {
    int id;
    int year;
    int month;
    int day;
    int hours;
    int minutes;
    SCString symbol;
    SCString message;
    bool alive;
    bool majorNews;
};

void displayMessage(s_sc &sc, int idCounter, SCString messageInput, int drawingCounter, bool majorNews) {
    s_UseTool Tool;
    Tool.Clear(); // reset tool structure for our next use
    Tool.ChartNumber = sc.ChartNumber;
    Tool.DrawingType = DRAWING_TEXT;
    Tool.LineNumber = idCounter;

    Tool.UseRelativeVerticalValues = 1;
    int toolBeginDate = 0;
    int toolBeginValue = 100;
    int toolFontSize = 30;
    int toolFontBold = false;
    if (majorNews) {
        toolBeginDate = 30;
        toolBeginValue = 50;
        toolFontSize = 60;
        toolFontBold = true;
    }
    Tool.BeginDateTime = toolBeginDate;
    Tool.BeginValue = toolBeginValue - (drawingCounter * 5);
    Tool.Color = COLOR_RED;
    Tool.Region = sc.GraphRegion;

    //SCString Label = messageInput.GetString();

    Tool.Text.Format("%s", messageInput.GetChars());
    Tool.FontSize = toolFontSize;
    Tool.FontBold = toolFontBold;
    //Tool.FontFace= "Courier";
    Tool.AddMethod = UTAM_ADD_OR_ADJUST;
    Tool.ReverseTextColor = 0;

    sc.UseTool(Tool);
}

SCSFExport scsf_NoTradingDays(SCStudyInterfaceRef sc) {

    if (sc.SetDefaults) {
        sc.GraphName = "NoTradingDays";
        sc.FreeDLL = 1;
        sc.StudyDescription = "Set a big message in the middle of screen when I shouldn't be trading.";
        sc.GraphRegion = 0;
        sc.AutoLoop = 0;

        //First set
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

        //Second set
        sc.Input[4].Name = "Year 2";
        sc.Input[4].SetInt(2016);
        sc.Input[4].SetIntLimits(2016, 2050);

        //Higher high close above
        sc.Input[5].Name = "Month 2";
        sc.Input[5].SetInt(7);
        sc.Input[5].SetIntLimits(1, 12);

        //Higher high close above
        sc.Input[6].Name = "Day 2";
        sc.Input[6].SetInt(22);
        sc.Input[6].SetIntLimits(1, 31);

        sc.Input[7].Name = "Message 2";
        sc.Input[7].SetString("Arghhhhh");

        //Third set
        sc.Input[8].Name = "Year 3";
        sc.Input[8].SetInt(2016);
        sc.Input[8].SetIntLimits(2016, 2050);

        //Higher high close above
        sc.Input[9].Name = "Month 3";
        sc.Input[9].SetInt(7);
        sc.Input[9].SetIntLimits(1, 12);

        //Higher high close above
        sc.Input[10].Name = "Day 3";
        sc.Input[10].SetInt(22);
        sc.Input[10].SetIntLimits(1, 31);

        sc.Input[11].Name = "Message 3";
        sc.Input[11].SetString("Arghhhhh");

        //Fourth set
        sc.Input[12].Name = "Year 4";
        sc.Input[12].SetInt(2016);
        sc.Input[12].SetIntLimits(2016, 2050);

        //Higher high close above
        sc.Input[13].Name = "Month 4";
        sc.Input[13].SetInt(7);
        sc.Input[13].SetIntLimits(1, 12);

        //Higher high close above
        sc.Input[14].Name = "Day 4";
        sc.Input[14].SetInt(22);
        sc.Input[14].SetIntLimits(1, 31);

        sc.Input[15].Name = "Message 4";
        sc.Input[15].SetString("Arghhhhh");

        //Fifth set
        sc.Input[16].Name = "Year 5";
        sc.Input[16].SetInt(2016);
        sc.Input[16].SetIntLimits(2016, 2050);

        //Higher high close above
        sc.Input[17].Name = "Month 5";
        sc.Input[17].SetInt(7);
        sc.Input[17].SetIntLimits(1, 12);

        //Higher high close above
        sc.Input[18].Name = "Day 5";
        sc.Input[18].SetInt(22);
        sc.Input[18].SetIntLimits(1, 31);

        sc.Input[19].Name = "Message 5";
        sc.Input[19].SetString("Arghhhhh");

        return;
    }

    const int arrayLength = 12;

    NewsItem newsItems[arrayLength] = {
            {70100, 2016, 7, 26, 15 + 2, 00, "EUR", "15:00 ConsumerConfidence",           true, false},
            {70101, 2016, 7, 27, 2 + 2,  30, "AUD", "02:30 CPI",                          true, false},
            {70102, 2016, 7, 27, 9 + 2,  30, "GBP", "09:30 Prelim GDP",                   true, false},
            {70103, 2016, 7, 27, 13 + 2, 30, "USD", "13:30 Core Durable Goods",           true, false},
            {70104, 2016, 7, 27, 15 + 2, 30, "USD", "15:30 Crude Oil Inventories",        true, false},
            {70105, 2016, 7, 27, 19 + 2, 00, "USD", "19:00 FOMC Statement",               true, true},
            {70106, 2016, 7, 28, 13 + 2, 30, "USD", "13:30 Unemployment Claims",          true, false},
            {70107, 2016, 7, 29, 0 + 2,  0,  "JPY", "Monetary Policy Statement",          true, false},
            {70108, 2016, 7, 29, 6 + 2,  0,  "JPY", "06:00 BOJ Outlook Report",           true, false},
            {70109, 2016, 7, 29, 13 + 2, 30, "CAD", "13:30 GDP",                          true, false},
            {70110, 2016, 7, 29, 13 + 2, 30, "USD", "13:30 Advance GDP",                  true, false},
            {70111, 2016, 7, 29, 21 + 2, 0,  "EUR", "21:00 EBA Bank Stress Test Results", true, false}
    };


    int CurrentTime = sc.CurrentSystemDateTime.GetTime();
    int CurrentDate = sc.CurrentSystemDateTime.GetDate();

    int Time = HMS_TIME(newsItems[0].hours, newsItems[0].minutes, 0);

//    SCString Buffer;
//    Buffer.Format("CT %d T %d", CurrentTime, Time);
//    sc.AddMessageToLog(Buffer, 0);

    int idCounter = 70100;

    int drawingCounter = 0;
    for (int x = 0; x < arrayLength; x = x + 1) {
        NewsItem item = newsItems[x];
        int DateValue0 = YMD_DATE(item.year, item.month, item.day);

        int Time = HMS_TIME(item.hours, item.minutes, 0);

        bool symbolMatch = sc.Symbol.GetSubString(3, 0).CompareNoCase(item.symbol) == 0 ||
                           sc.Symbol.GetSubString(3, 3).CompareNoCase(item.symbol) == 0;

        if (item.alive && CurrentDate == DateValue0 && (symbolMatch || item.majorNews) && CurrentTime < Time) {
            displayMessage(sc, item.id, item.message, drawingCounter, item.majorNews);
            drawingCounter++;
        } else {
            item.alive = false;
            sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_CHARTDRAWING, item.id);
        }
    }

    int DateValue0 = YMD_DATE(sc.Input[0].GetInt(), sc.Input[1].GetInt(), sc.Input[2].GetInt());
    if (CurrentDate == DateValue0) {
        displayMessage(sc, idCounter++, sc.Input[3].GetString(), 0, false);
    }

    int DateValue4 = YMD_DATE(sc.Input[4].GetInt(), sc.Input[5].GetInt(), sc.Input[6].GetInt());
    if (CurrentDate == DateValue4) {
        displayMessage(sc, idCounter++, sc.Input[7].GetString(), 0, false);
    }

    int DateValue8 = YMD_DATE(sc.Input[8].GetInt(), sc.Input[9].GetInt(), sc.Input[10].GetInt());
    if (CurrentDate == DateValue8) {
        displayMessage(sc, idCounter++, sc.Input[11].GetString(), 0, false);
    }

    int DateValue12 = YMD_DATE(sc.Input[12].GetInt(), sc.Input[13].GetInt(), sc.Input[14].GetInt());
    if (CurrentDate == DateValue8) {
        displayMessage(sc, idCounter++, sc.Input[15].GetString(), 0, false);
    }

    int DateValue16 = YMD_DATE(sc.Input[16].GetInt(), sc.Input[17].GetInt(), sc.Input[18].GetInt());
    if (CurrentDate == DateValue8) {
        displayMessage(sc, idCounter++, sc.Input[19].GetString(), 0, false);
    }
}
