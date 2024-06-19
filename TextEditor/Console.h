#pragma once
#include <conio.h>
#include <Windows.h>

#include <iostream>
#include <vector>
#include <string>

#include "Cursor.h"
#include "File.h"


class Console {
public:

  /// <summary>
  /// Sets the text color in the console.
  /// </summary>
  /// <param name="color">The color code to set.</param>
  static void ColorFunction(int color);

  /// <summary>
  /// Displays a message with specified colors(usually red for displaying errors) and waits for user input.
  /// </summary>
  /// <param name="message">The message to display.</param>
  /// <param name="message_color">The color for the message text.</param>
  /// <param name="printing_color">The default color of text in console.</param>
  static void ShowMessage(std::string message, int message_color, int printing_color);

  /// <summary>
  /// Prints the main menu with specified options and highlights the selected option.
  /// </summary>
  /// <param name="menu">An array of menu options.</param>
  /// <param name="option_number">The index of the currently selected option.</param>
  /// <param name="printing_color">The default color of text in console.</param>
  static void PrintMenu(std::string menu[], int option_number, int printing_color);

  /// <summary>
  /// Prints whole content of opened file. it uses when editor moves to edit page (after openning file or close search page).
  /// </summary>
  /// <param name="temp_content">The content to display.</param>
  /// <param name="print_color">The color for printing text.</param>
  /// <param name="quotation_color">The color for quotation text.</param>
  static void ShowContent(std::vector<std::string> temp_content, int print_color, int quotation_color);

  /// <summary>
  /// Updates the screen for one line of file.
  /// </summary>
  /// <param name="line_number">The number of line to update it.</param>
  /// <param name="temp_content">The whole content of file.</param>
  /// <param name="previous_size">Previous size of the line to update.</param>
  /// <param name="quot">Quotation check of the beggining of the line.</param>
  /// <param name="print_color">The color for printing text.</param>
  /// <param name="quot_color">The color for quotation text.</param>
  static void UpdateOneLine(int line_number, std::string temp_content, int previous_size, bool quot, int print_color, int quot_color);

  /// <summary>
  /// Updates the screen from line "line_number" to the end of file.
  /// </summary>
  /// <param name="line_number">The number of line to start updating the screen.</param>
  /// <param name="previous_content">Previous content of the file.</param>
  /// <param name="new_content">New content of the file.</param>
  /// <param name="quot_vector">Quotation check of the beggining of the lines.</param>
  /// <param name="print_color">The color for printing text.</param>
  /// <param name="quot_color">The color for quotation text.</param>
  static void UpdateFromLineI(int line_number, std::vector <std::string> previous_content, std::vector <std::string> new_content,
    std::vector <bool> quot_vector, int print_color, int quot_color);

  /// <summary>
  /// If user selects text so we should change the color of the text. this function does that.
  /// </summary>
  /// <param name="temp_content">The whole content of file.</param>
  /// <param name="select_start_pos">Start position of selection by user.</param>
  /// <param name="select_end_pos">End position of selection by user.</param>
  /// <param name="print_color">The color for regular text.</param>
  /// <param name="select_color">The color for selected text.</param>
  static void HighlightSelectedText(std::vector <std::string> temp_content, COORD select_start_pos, COORD select_end_pos,
    int print_color, int select_color);

  /// <summary>
  /// Removes highlights when user deselect the text.
  /// </summary>
  /// <param name="temp_content">The whole content of file.</param>
  /// <param name="select_start_pos">Start position of selection by user.</param>
  /// <param name="select_end_pos">End position of selection by user.</param>
  /// <param name="quot_vector">Quotation check of the beggining of the lines.</param>
  /// <param name="print_color">The color for printing text.</param>
  /// <param name="quot_color">The color for quotation text.</param>
  static void RemoveSelectHighlight(std::vector <std::string> temp_content, COORD select_start_pos, COORD select_end_pos,
    std::vector<bool> quot_vector, int print_color, int quot_color);

  /// <summary>
  /// If user has searched for a text, it highlights the founded search text in file.
  /// </summary>
  /// <param name="search_found_position"> Positions of found search query in each line.</param>
  /// <param name="search_query">The search query that user inputed. it used for searching in the file and highlight them.</param>
  /// <param name="print_color">The color for regular text.</param>
  /// <param name="match_color">The color for matched search text.</param>
  static void HighlightSearchText(std::vector <std::vector<int>> search_found_position, std::string search_query, int print_color, int match_color);

  /// <summary>
  /// Removes highlight of founded matches of search query when user press escape to remove highlight.
  /// </summary>
  /// <param name="search_found_position">Positions of found search query in each line.</param>
  /// <param name="search_query">The search query that user inputed. it used for searching in the file and highlight them.</param>
  /// <param name="print_color">The color for regular text.</param>
  static void RemoveSearchHighlight(std::vector <std::vector<int>> search_found_position, std::string search_query, int print_color);

  /// <summary>
  /// Asks the user to confirm saving the changes to the file and returns the user's choice.
  /// </summary>
  /// <param name="file_name">The name of the file to save.</param>
  /// <param name="print_color">The color for regular text.</param>
  /// <param name="select_color">The color for selected option.</param>
  /// <returns>True if the user chooses to save, false o.w.</returns>
  static bool SavePage(std::string file_name, int print_color, int select_color);

  /// <summary>
  /// Displays the first setting page (it has 4 option) and returns the user's choice.
  /// </summary>
  /// <returns>The user's choice or 27 if the user presses Esc.</returns>
  static int SettingPage1();

  /// <summary>
  /// Displays the second setting page(showing available colors for selected option in setting page 1) and returns the user's choice.
  /// </summary>
  /// <param name="message">The sample message to display.</param>
  /// <param name="start_color_number">The starting available color number to display.</param>
  /// <param name="end_color_number">The ending available color number to display.</param>
  /// <param name="current_print_color">The default print color in console.</param>
  /// <returns>The user's choice of color.</returns>
  static int SettingPage2(std::string message, int start_color_number, int end_color_number, int current);
};