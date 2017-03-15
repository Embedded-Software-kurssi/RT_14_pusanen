const unsigned long	FRAME_MS = 10;
const unsigned long FRAMES = 10;
unsigned long next_frame_start = 10;


void task1();
void task2();
void task3();


void setup() {

}

void loop() {
	while(1) {
		static unsigned long frame = 0;
		while(next_frame_start - millis() > 0);
		next_frame_start += FRAME_MS;
		switch(frame) {
			case 0:
			task1();
			task2();
			break;
			case 1:
			task1();
			task3();
			break;
			case 2:
			task1();
			break;
			case 3:
			task1();
			break;
			case 4:
			task1();
			break;
			case 5:
			task1();
			break;
			case 6:
			task1();
			task3();
			break;
			case 7:
			task1();
			break;
			case 8:
			task1();
			break;
			case 9:
			task1();
			break;
		}
		frame = (frame + 1) % FRAMES;
	}
}

void task1() {
	for(volatile int i = 0; i < 252; i++) {
		
	}
}

void task2() {
	for(volatile int i = 0; i < 420; i++) {
		
	}
}

void task3() {
	for(volatile int i = 0; i < 588; i++) {
		
	}
}