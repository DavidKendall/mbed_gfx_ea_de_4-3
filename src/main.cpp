/*
 * Simple program to illustrate use of Display class with
 * the EA 4.3" display expansion board.
 *
 * Displays some text and a couple of shapes then
 * continually updates a field showing the current
 * value of a potentiometer.
 */
 
#include <mbed.h>
#include <display.h>


void anotherDisplayUser(void);

int main (void) {
  Display* d = Display::theDisplay();
	AnalogIn pot(p15);

  if (d->initOk()) {
		d->fillScreen(WHITE);
		d->setTextColor(BLACK, WHITE);
		d->setCursor(0, 5);
		d->printf(" Hello World!\n");
		d->printf(" The sun has got his hat on\n");
		d->printf(" An integer value : %d\n", 12345);
		d->printf(" A floating point value : %.3f\n", 3.141);
		anotherDisplayUser();
		d->fillRoundRect(200, 2, 100, 50, 10, RED);
		d->drawRoundRect(200, 2, 100, 50, 10, BLUE);
		while (true) {
			d->setCursor(0, 150);
			d->printf(" Pot  : %1.2f\n", pot.read());
		}
	}
}

void anotherDisplayUser(void) {
	Display* d = Display::theDisplay();
	
	d->printf(" Hello from another user\n");
	d->fillCircle(200, 100, 25, GREEN);
	d->drawCircle(200, 100, 25, BLUE);
}



