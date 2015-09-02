
/******************************************************************************
 * Includes
 *****************************************************************************/

#include "mbed.h"
#include "display.h"



/******************************************************************************
 * Main function
 *****************************************************************************/


int main (void) {
  Display d;

  if (d.initOk()) {
		d.fillScreen(WHITE);
		d.setTextColor(BLACK, WHITE);
		d.setCursor(12, 2);
		d.writeString("Hello World");
		d.setCursor(12, 14);
		d.writeString("The sun has got his hat on");
		d.setCursor(12, 26);
		d.printf("Hello integer world %d\n", 12345);
	}
}

