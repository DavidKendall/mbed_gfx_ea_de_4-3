#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <mbed.h>
#include <Graphics.h>
#include <LcdController.h>
#include <EaLcdBoardGPIO.h>
#include <AR1021I2C.h>
#include <sdram.h>
#include <wchar.h>
#include <GFXFb.h>

class Display : public GFXFb {
	public:
		static Display& getInstance();
	  bool initOk();
	  ~Display();
	
	private:
	  Display();
	  char *initStr;
	  LcdController::Config *lcdCfg;
		EaLcdBoardGPIO lcdBoard;
    uint32_t frameBuffer;
	  EaLcdBoard::Result result;
};
#endif
