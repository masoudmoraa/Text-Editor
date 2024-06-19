#include "Cursor.h"

/*
Explanation: Moves the console cursor to the specified position (X, Y).
Parameters:
    - x: The X-coordinate to move the cursor to.
    - y: The Y-coordinate to move the cursor to.
*/
void Cursor::GoToXY(int x, int y) {
  COORD c{ 0,0 };
  c.X = x;
  c.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
  return;
}

/*
Explanation: Helps to change the visibility of console cursor.
Parameters:
    - show_flag: If false cursor will be hidden, visible o.w.
*/
void Cursor::ShowConsoleCursor(bool show_flag) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO     cursor_info;
  GetConsoleCursorInfo(out, &cursor_info);
  cursor_info.bVisible = show_flag; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursor_info);
}

/*
Explanation: Retrieves the current position of the console cursor.
Return: The current cursor position as a COORD structure.
*/
COORD Cursor::GetConsoleCursorPosition() {
  HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO cbsi;
  if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi)) {
    return cbsi.dwCursorPosition;
  }

  /* If, for any reason, it cannot obtain the correct position, it will return invalid data. */
  COORD invalid = { -1, -1 };
  return invalid;
}

/*
Explanation: Retrieves the position of the first line of visible content in the console window.
Return: The position of the first line of visible content in the console window.
*/
COORD Cursor::ConsoleTopCharLocator() {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);

  COORD position = { csbi.srWindow.Left, csbi.srWindow.Top };
  return position;
}


/*
Explanation: Moves the console cursor up by one line.
Parameters:
    - cursor_position: The current cursor position.
    - line_length_vector: A vector storing the lengths of each line of text.
*/
void Cursor::MoveCursorUp(COORD cursor_position, std::vector <int> line_length_vector) {

  /* it checks that is there a line to move cursor up or not. */
  if (cursor_position.Y > 0) {
    GoToXY(cursor_position.X, cursor_position.Y - 1);

    /* it checks the lenght of upper line to move cursor up correctly.
    for example if cursor is at position 5. the upper line has 2 character. so the cursor should move to position 2 of upper line.*/

    if (cursor_position.X > line_length_vector[cursor_position.Y - 1]) {
      GoToXY(line_length_vector[cursor_position.Y - 1], cursor_position.Y - 1);
    }
  }
  return;
}

/*
Explanation: Moves the console cursor down by one line.
Parameters:
    - cursor_position: The current cursor position.
    - line_length_vector: A vector storing the lengths of each line of text.
*/
void Cursor::MoveCursorDown(COORD cursor_position, std::vector <int> line_length_vector) {

  /* it checks that is there a next line to move cursor down or not. */
  if (cursor_position.Y < line_length_vector.size() - 1) {
    GoToXY(cursor_position.X, cursor_position.Y + 1);

    /* it checks the lenght of next line to move cursor down correctly.
    for example if cursor is at position 5. the next line has 2 character. so the cursor should move to position 2 of next line.*/
    if (cursor_position.X > line_length_vector[cursor_position.Y + 1]) {
      GoToXY(line_length_vector[cursor_position.Y + 1], cursor_position.Y + 1);
    }
  }
  return;
}

/*
Explanation: Moves the console cursor left by one position.
Parameters:
    - cursor_position: The current cursor position.
    - line_length_vector: A vector storing the lengths of each line of text.
*/
void Cursor::MoveCursorLeft(COORD cursor_position, std::vector <int> line_length_vector) {

  /* if cursur is not at the beggining of line. */
  if (cursor_position.X > 0) {
    GoToXY(cursor_position.X - 1, cursor_position.Y);
    return;
  }

  /* cursur is at the beggining of line. so cursor should move to end of previous line. "if" is check is there a line before the current*/
  if (cursor_position.Y > 0) {
    GoToXY(line_length_vector[cursor_position.Y - 1], cursor_position.Y - 1);
    return;
  }
}

/*
Explanation: Moves the console cursor right by one position.
Parameters:
    - cursor_position: The current cursor position.
    - line_length_vector: A vector storing the lengths of each line of text.
*/
void Cursor::MoveCursorRight(COORD cursor_position, std::vector <int> line_length_vector) {

  /* if cursor is not at the end of line. */
  if (cursor_position.X < line_length_vector[cursor_position.Y]) {
    GoToXY(cursor_position.X + 1, cursor_position.Y);
    return;
  }

  /* cursor is at the end of line so it should move to the beggining of next line.
  "if" is checking is there next line or not.*/
  if (cursor_position.Y < line_length_vector.size() - 1) { // at end of line
    GoToXY(0, cursor_position.Y + 1);
    return;
  }
}