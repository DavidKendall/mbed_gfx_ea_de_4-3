
/******************************************************************************
 * Includes
 *****************************************************************************/

#include "mbed.h"


#include "LcdController.h"
#include "EaLcdBoardGPIO.h"
#include "AR1021I2C.h"
#include "sdram.h"

#include "wchar.h"
#include "GFXFb.h"



/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

#define LCD_CONFIGURATION_43 \
        40,                         /* horizontalBackPorch */ \
        5,                          /* horizontalFrontPorch */ \
        2,                          /* hsync */ \
        480,                        /* width */ \
        8,                          /* verticalBackPorch */ \
        8,                          /* verticalFrontPorch */ \
        2,                          /* vsync */ \
        272,                        /* height */ \
        false,                      /* invertOutputEnable */ \
        false,                      /* invertPanelClock */ \
        true,                       /* invertHsync */ \
        true,                       /* invertVsync */ \
        1,                          /* acBias */ \
        LcdController::Bpp_16_565,  /* bpp */ \
        9000000,                    /* optimalClock */ \
        LcdController::Tft,         /* panelType */ \
        false                       /* dualPanel */

#define LCD_INIT_STRING_43  (char*)"v1,cd0,c50,cc0,c30,d100,c31,d100,cd1,d10,o,c51,cc100"


/******************************************************************************
 * Local variables
 *****************************************************************************/

// EA LCD Board interface
static EaLcdBoardGPIO lcdBoard(P0_27, P0_28);

static uint16_t const colors[16] = {
        BLACK,
        LIGHTGRAY,
        DARKGRAY,
        WHITE,
        RED,
        GREEN,
        BLUE,
        MAGENTA,
        CYAN,
        YELLOW,
        LIGHTRED,
        LIGHTGREEN,
        LIGHTBLUE,
        LIGHTMAGENTA,
        LIGHTCYAN,
        LIGHTYELLOW
};

/******************************************************************************
 * Local functions
 *****************************************************************************/

static uint16_t random(uint16_t max)
{
  uint16_t temp;

  temp = rand();
  temp = temp % max;
  return temp;
}

static void demo1(GFXFb &gfx) {

    int16_t x0 = 0;
    int16_t y0 = 0;
    int16_t radius = 0;
    int color = 0;
    int fill = 0;


    gfx.fillScreen(BLACK);

    for (int i = 0; i < 100; i++) {
        x0 = random(gfx.width());
        y0 = random(gfx.height());
        color = random(16);
        radius = random(50);
        fill = random(2);

        if (!fill) {
            gfx.drawCircle(x0, y0, radius, colors[color]);
        } else {
            gfx.fillCircle(x0, y0, radius, colors[color]);
        }

        wait_ms(10);
    }

}


static void demo2(GFXFb &gfx) {
    int32_t margin = 5;
    int32_t rowHeight = gfx.height() / 3;
    int32_t colWidth = gfx.width() / 3;
    int32_t graphPosX = gfx.getStringWidth("drawRoundRect");
    int32_t maxGraphW = colWidth - graphPosX;

    gfx.fillScreen(BLACK);

    // ##############
    // C O L U M N  1
    // ##############

    // drawLine
    gfx.setCursor(0, rowHeight/2);
    gfx.writeString("drawLine");
    gfx.drawLine(0+graphPosX+margin, margin,
            0+graphPosX+maxGraphW-margin, rowHeight-margin, colors[1]);

    // drawRect
    gfx.setCursor(0, rowHeight+rowHeight/2);
    gfx.writeString("drawRect");
    gfx.drawRect(0+graphPosX+margin, rowHeight+margin,
            maxGraphW-2*margin, rowHeight-margin, colors[2]);

    // fillRect
    gfx.setCursor(0, rowHeight*2+rowHeight/2);
    gfx.writeString("fillRect");
    gfx.fillRect(0+graphPosX+margin, rowHeight*2+margin,
                maxGraphW-2*margin, rowHeight-margin, colors[3]);

    // ##############
    // C O L U M N  2
    // ##############

    // drawCircle
    gfx.setCursor(colWidth, rowHeight/2);
    gfx.writeString("drawCircle");
    gfx.drawCircle(colWidth+graphPosX+maxGraphW/2, rowHeight/2,
            rowHeight/2-2*margin, colors[4]);

    // fillCircle
    gfx.setCursor(colWidth, rowHeight+rowHeight/2);
    gfx.writeString("fillCircle");
    gfx.fillCircle(colWidth+graphPosX+maxGraphW/2, rowHeight+rowHeight/2,
            rowHeight/2-2*margin, colors[5]);

    // drawTriangle
    gfx.setCursor(colWidth, rowHeight*2+rowHeight/2);
    gfx.writeString("drawTriangle");
    gfx.drawTriangle(colWidth+graphPosX+margin, rowHeight*3-margin,
            colWidth+graphPosX+maxGraphW/2, rowHeight*2+margin,
            colWidth+graphPosX+maxGraphW-margin, rowHeight*3-margin, colors[6]);

    // ##############
    // C O L U M N  3
    // ##############

    // fillTriangle
    gfx.setCursor(colWidth*2, rowHeight/2);
    gfx.writeString("fillTriangle");
    gfx.fillTriangle(colWidth*2+graphPosX+margin, rowHeight-margin,
            colWidth*2+graphPosX+maxGraphW/2, margin,
            colWidth*2+graphPosX+maxGraphW-margin, rowHeight-margin, colors[7]);


    // drawRoundRect
    gfx.setCursor(colWidth*2, rowHeight+rowHeight/2);
    gfx.writeString("drawRoundRect");
    gfx.drawRoundRect(colWidth*2+graphPosX+margin, rowHeight+margin,
            maxGraphW-2*margin, rowHeight-margin, 10, colors[8]);

    // fillRoundRect
    gfx.setCursor(colWidth*2, rowHeight*2+rowHeight/2);
    gfx.writeString("fillRoundRect");
    gfx.fillRoundRect(colWidth*2+graphPosX+margin, rowHeight*2+margin,
            maxGraphW-2*margin, rowHeight-margin, 10, colors[9]);


}

static void demo3(GFXFb &gfx, TouchPanel* touchPanel) {

    uint16_t x = 0;
    uint16_t y = 0;
    bool hasMorePoints = false;
    TouchPanel::touchCoordinate_t coord;


    do {
        if (!touchPanel->init(gfx.width(), gfx.height())) {
            printf("TouchPanel.init failed\n");
            break;
        }

        printf("Starting calibration\n");
        if (!touchPanel->calibrateStart()) {
            printf("Failed to start calibration\n");
            break;
        }


        do {
            if (!touchPanel->getNextCalibratePoint(&x, &y)) {
                printf("Failed to get next calibrate point\n");
                break;
            }

            printf("calib: x=%d, y=%d\n", x, y);
            gfx.fillScreen(BLACK);
            gfx.setCursor(0, 0);
            gfx.writeString("Calibrate Touch Screen");
            gfx.drawRect(x-5, y-5, 10, 10, WHITE);


            if (!touchPanel->waitForCalibratePoint(&hasMorePoints, 0)) {
                printf("Failed waiting for calibration point\n");
                break;
            }

        } while(hasMorePoints);

        printf("Calibration done\n");

        gfx.fillScreen(BLACK);


        while(1) {
            touchPanel->read(coord);
            if (coord.z > 0) {
                gfx.drawPixel(coord.x, coord.y, WHITE);
            }
            wait_ms(2);
        }

    } while (0);



    while(1);
}

/******************************************************************************
 * Main function
 *****************************************************************************/


int main (void) {
    bool initSuccessful = false;

    EaLcdBoard::Result result;
	  LcdController::Config *lcdCfg = new LcdController::Config(LCD_CONFIGURATION_43);
    uint32_t frameBuf1 = 0;
    TouchPanel* tp = new AR1021I2C(P0_27, P0_28, P2_25);
;
    // frame buffer is put in SDRAM
    if (sdram_init() == 1) {
    	printf("Failed to initialize SDRAM\n");
    	return 1;
    }

    do {

        result = lcdBoard.open(lcdCfg, LCD_INIT_STRING_43);
        if (result != EaLcdBoard::Ok) {
            printf("Failed to open display: %d\n", result);
            break;
        }




        // allocate framebuffer, width x height x 2 (2 bytes = 16 bit color data)
        frameBuf1 = (uint32_t)malloc(lcdCfg->width*lcdCfg->height*2);
        if (frameBuf1 == 0) {
            printf("Failed to allocate frame buffer\n");
            break;
        }


        result = lcdBoard.setFrameBuffer(frameBuf1);
        if (result != EaLcdBoard::Ok) {
            printf("Failed to activate frameBuffer: %d\n", result);
            break;
        }


        memset((void*)frameBuf1, 0x0, lcdCfg->width*lcdCfg->height*2);



        initSuccessful = true;


    } while(0);




    if (initSuccessful) {

        GFXFb gfx(lcdCfg->width, lcdCfg->height, (uint16_t*)frameBuf1);

        while (1) {
            demo1(gfx);
            wait_ms(5000);
            demo2(gfx);
            wait_ms(5000);
            demo3(gfx, tp);
        }

    }
    else {
        printf("Couldn't start demo -> Initialization failed\n");
    }


    return 0;
}

