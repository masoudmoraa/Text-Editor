#pragma once
#include <conio.h>
#include <process.h>
#include <stdio.h> 
#include <Windows.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>

#include "File.h"
#include "Console.h"
#include "Cursor.h"

#define ctrl(x) (x & 0x1F)


class Editor {
public:

  /// <summary>
  /// Initializes a new instance of the Editor class and displays 
  /// a welcome message.
  /// </summary>
  Editor();
private:

  /// <summary>
  /// Creates a delay for a specified number of milliseconds. used in one case.
  /// after Welcome message.
  /// </summary>
  /// <param name="milisec">The duration of the delay in milliseconds.</param>
  void Delay(int milisec);

  /// <summary>
  /// Displays the main menu to the user and handles menu options. managing the
  /// selected menu option made by the user.
  /// </summary>
  void GetInputMenu();

  /// <summary>
  /// Opens a file, allowing the user to edit its content.
  /// </summary>
  void OpenFileGetKey();

  /// <summary>
  /// Handles key presses when editing a file.
  /// </summary>
  /// <param name="value">The character value of the key pressed.</param>
  /// <param name="position">The current cursor position.</param>
  void KeyDownDetected(char value, COORD position);

  /// <summary>
  /// Displays a search text input page when user click ctrl+f, and save the 
  /// input in search_query_.
  /// </summary>
  /// <returns>True if the search was successful, false otherwise.</returns>
  bool SearchTextPage();

  /// <summary>
  /// Searches for a given text in the file's content. use in SearchTextPage().
  /// this function modifies search_found_position_ vector.
  /// </summary>
  /// <param name="search_query">The text to search for.</param>
  /// <returns>True if the text is found, false otherwise.</returns>
  bool SearchText(std::string search_query);

  /// <summary>
  /// Finds the index of a substring within a string (one line of file), 
  /// starting from a specified position. use in SearchText().
  /// </summary>
  /// <param name="str">The string to search in.</param>
  /// <param name="search">The substring to find.</param>
  /// <param name="start_pos">The starting position for the search.</param>
  /// <returns>The index of the substring if found, or -1 if not found.</returns>
  int  FindSubstringIndex(std::string str, std::string search, int start_pos);

  /// <summary>
  /// Updates variables related to text selection. when user select/deselect text.
  /// use in OpenFileGetKey().
  /// </summary>
  /// <param name="shiftkey">True if the Shift key is pressed, indicates text selection.</param>
  /// <param name="previous_position">The previous cursor position.</param>
  /// <param name="new_position">The new cursor position.</param>
  void UpdateSelectVars(bool shiftkey, COORD previous_position, COORD new_position);


  /// <summary>
  /// Resets text selection variables to their default state.
  /// </summary>
  void DefaultSelectVars();

  /// <summary>
  /// Displays a help page with information about using the editor. 
  /// calls when user choose Help option.
  /// </summary>
  void Help();

  /// <summary>
  /// Displays a settings page for customizing editor options.
  /// </summary>
  void Setting();

  /// <summary>
  /// Exits the editor program.
  /// </summary>
  void Exit();

  /// <summary>
  /// The color code used for printing text in the editor.
  /// </summary>
  int printing_color_ = 7;

  /// <summary>
  /// The color code used for quotation text in the editor.
  /// </summary>
  int quotation_color_ = 2;

  /// <summary>
  /// The color code used for selecting text in the editor.
  /// </summary>
  int text_selecting_color_ = 135;

  /// <summary>
  /// The color code used for highlighting matches when searching.
  /// </summary>
  int highlight_matches_color_ = 114;

  /// <summary>
  /// An instance of the File class used to access file-related functions.
  /// Modifying this variable in the hello() function is done by the temp variable.
  /// </summary>
  File file_;

  /// <summary>
  /// Stores the selected menu option to highlight it and processing 
  /// when the user presses Enter. for example remove file is 2.
  /// </summary>
  int option_number_ = 0;

  /// <summary>
  /// Stores the user's keyboard input.
  /// </summary>
  char key_;

  /// <summary>
  /// Stores the text that the user wants to find in the opened file.
  /// </summary>
  std::string search_query_ = "";

  /// <summary>
  /// Stores positions of found search_query_ in the opened file.
  /// size of vector equals to number of file's lines and each index
  /// contains a vector of founded positions at corresponding line.
  /// </summary>
  std::vector <std::vector<int>> search_found_position_;

  /// <summary>
  /// Indicates whether text is selected by the user. if user select a text, 
  /// select_ will be true, o.w. it will be false.
  /// it will be useful to detect selected text by user and highlight that.
  /// </summary>
  bool select_ = false;

  /// <summary>
  /// The starting position of the selected text. this will be useful to detect
  /// selected text by user to highlight that.
  /// also when user want copy text (or cut or delete).
  /// </summary>
  COORD select_start_pos_;

  /// <summary>
  /// The ending position of the selected text. this will be useful to detect 
  /// selected text by user to highlight that.
  /// also when user want copy text (or cut or delete).
  /// </summary>
  COORD select_end_pos_;

  /// <summary>
  /// An array of available options in the editor's menu.
  /// </summary>
  std::string options[6] = {
    " Create new file\n",
    " Open file\n",
    " Remove file\n",
    " Copy file\n",
    " Help\n",
    " Setting\n",
  };
};