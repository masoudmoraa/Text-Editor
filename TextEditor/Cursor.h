#pragma once
#include <process.h>
#include <Windows.h>

#include <iostream>
#include <vector>


class Cursor {
public:

  /// <summary>
  /// Moves the console cursor to the specified position (X, Y).
  /// </summary>
  /// <param name="x">The X-coordinate to move the cursor to.</param>
  /// <param name="y">The Y-coordinate to move the cursor to.</param>
  static void GoToXY(int x, int y);

  /// <summary>
  /// Helps to change the visibility of console cursor.
  /// </summary>
  /// <param name="showFlag">If false cursor will be hidden, visible o.w.</param>
  static void ShowConsoleCursor(bool showFlag);

  /// <summary>
  /// Retrieves the current position of the console cursor.
  /// </summary>
  /// <returns>The current cursor position as a COORD structure.</returns>
  static COORD GetConsoleCursorPosition();

  /// <summary>
  /// Retrieves the position of the first line of visible content in the console window.
  /// </summary>
  static COORD ConsoleTopCharLocator();

  /// <summary>
  /// Moves the console cursor up by one line.
  /// </summary>
  /// <param name="current_pos">The current cursor position.</param>
  /// <param name="lines_len">is used to keep track of the lengths of lines in a text editor.</param>
  static void MoveCursorUp(COORD current_pos, std::vector<int> lines_len);

  /// <summary>
  /// Moves the console cursor down by one line.
  /// </summary>
  /// <param name="current_pos">The current cursor position.</param>
  /// <param name="lines_len">is used to keep track of the lengths of lines in a text editor.</param>
  static void MoveCursorDown(COORD current_pos, std::vector <int> lines_len);

  /// <summary>
  /// Moves the console cursor up by one position.
  /// </summary>
  /// <param name="current_pos">The current cursor position.</param>
  /// <param name="lines_len">is used to keep track of the lengths of lines in a text editor.</param>
  static void MoveCursorLeft(COORD current_pos, std::vector <int> lines_len);

  /// <summary>
  /// Moves the console cursor up by one position.
  /// </summary>
  /// <param name="current_pos">The current cursor position.</param>
  /// <param name="lines_len">is used to keep track of the lengths of lines in a text editor.</param>
  static void MoveCursorRight(COORD current_pos, std::vector <int> lines_len);
};