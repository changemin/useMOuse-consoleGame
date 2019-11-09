
#pragma once
#include <stdio.h> 
#include <Windows.h>

HANDLE CONSOLE_INPUT, CONSOLE_OUTPUT;
HWND WINDOW_HANDLE;

inline int hasInput() {
	INPUT_RECORD input_record;
	DWORD input_count;

	PeekConsoleInput(CONSOLE_INPUT, &input_record, 1, &input_count);
	return input_count;
}

INPUT_RECORD rec;
DWORD dwNOER;

inline int isMouseClicked() {
	ReadConsoleInput(CONSOLE_INPUT, &rec, 1, &dwNOER);
	while (1) {
		if (rec.EventType == MOUSE_EVENT) {
			const DWORD buttonState = rec.Event.MouseEvent.dwButtonState;
			const DWORD EventFlags = rec.Event.MouseEvent.dwEventFlags;

			if ((buttonState & FROM_LEFT_1ST_BUTTON_PRESSED) && !(buttonState & RIGHTMOST_BUTTON_PRESSED))
			{
				// left button
				return 1;
			}
			else if (!(buttonState & FROM_LEFT_1ST_BUTTON_PRESSED) && buttonState & RIGHTMOST_BUTTON_PRESSED) {
				// right button
				return 2;
			}
			else if ((buttonState & FROM_LEFT_1ST_BUTTON_PRESSED) && (buttonState & RIGHTMOST_BUTTON_PRESSED)) {
				// left & right button clicked
				return 3;
			}
			else if (EventFlags & MOUSE_WHEELED) { 
				// Wheel action
				return 4;
			}
			else if (buttonState & MOUSE_WHEELED) { 
				// Wheel button Clicked
				return 5;
			}
		}
		return 0;
	}
	
}

inline COORD getMousePosition() {
	POINT cursorPosition;
	GetCursorPos(&cursorPosition);
	ScreenToClient(WINDOW_HANDLE, &cursorPosition);
	return (COORD) { (SHORT)cursorPosition.x * 2, (SHORT)cursorPosition.y * 2 };
}

void enableMouseInput() {
	DWORD mode;
	CONSOLE_INPUT = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(CONSOLE_INPUT, &mode);
	SetConsoleMode(CONSOLE_INPUT, mode | ENABLE_MOUSE_INPUT);
}