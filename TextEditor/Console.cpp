#include "Console.h"

/*
/Explanation: Sets the text color in the console.
Parameters:
    - color: The color code to set.
*/
void Console::ColorFunction(int color) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/*
Explanation: Displays a message with specified colors and waits for user input.
it used for error in creating file or openning file or removing file. or remove file successfully
Parameters:
    - message: The message to display.
    - message_color: The color for the message text. red for error. blue for successfull action.
    - printing_color: The default color of text in console.
*/
void Console::ShowMessage(std::string message, int message_color, int printing_color) {
  std::cout << " ";
  ColorFunction(message_color);
  std::cout << message << "\n\n";

  /* change text color of console to default */
  ColorFunction(printing_color);
  std::cout << " ";
  /* waits for input */
  system("pause");
}

/*
Explanation: Prints a menu with specified options and highlights the selected option.
Parameters:
    - menu: An array of main menu options. each option is in one index of array. it has 6 options.
    - option_number: The index of the currently selected option. it used for changing color of this option.
    - printing_color: The default color for console text. (so it used for other options.(not selected options))
*/
void Console::PrintMenu(std::string menu[], int option_number, int printing_color) {

  /* showing 6 options of menu */
  for (int i = 0; i < 6; i++) {
    ColorFunction(printing_color);
    Cursor::GoToXY(0, i);

    /* if selected options by user is i_th option in menu */
    if (i == option_number) {

      /* change color for selected option */
      ColorFunction(6);
      std::cout << " ->" << menu[i];

      /* change color to default to show other options */
      ColorFunction(printing_color);
    }

    /* if this option is not selected */
    else
      std::cout << "   " << menu[i];
  }

  /* Showing to user how to choose options or exit the program. */
  std::cout <<
    "\n  Enter  : Select"
    "\n  ctrl+Q : Exit \n";
}

/*
Explanation: Prints whole content of opened file with regular text color.
Parameters:
    - temp_content: The content to display.
    - print_color: The color for printing text.
    - quotation_color: The color for quotation text.
*/
void Console::ShowContent(std::vector<std::string> content, int print_color, int quotation_color) {
  ColorFunction(print_color);
  system("cls");

  /* prints the whole content by iterating on temp_content_ vector.*/
  bool quotation_check = false;
  for (int i = 0; i < content.size(); i++) {
    Cursor::GoToXY(0, i);
    for (int j = 0; j < content[i].length(); j++) {
      /* detecting qoutation marks */
      if (content[i][j] == '"') {

        /* if it is true, so the current qoutation mark is a closing qoutation mark. */
        if (quotation_check) {
          quotation_check = false;
          std::cout << content[i][j];
          ColorFunction(print_color);
          continue;
        }

        /* if it is false, so the current qoutation mark is a openning qoutation mark. */
        else {
          quotation_check = true;
          ColorFunction(quotation_color);
          Cursor::GoToXY(j, i);
        }
      }
      std::cout << content[i][j];
    }
  }
  ColorFunction(print_color);
  COORD end_type = Cursor::GetConsoleCursorPosition();
  for (int i = content.size(); i < content.size() + 10; i++)
    std::cout << "\n~";
  Cursor::GoToXY(end_type.X, end_type.Y);
}


/*
Explanation: Updates the screen for one line of file after user modifies it (delete/add character).
Parameters:
    - line_number: The number of line to update it.
    - temp_content: The whole content of file.
    - previous_size: Previous size of the line to update.
    - quot: Quotation check of the beggining of the line.
    - print_color: The color for printing text.
    - quot_color: The color for quotation text.
*/
void Console::UpdateOneLine(int line_number, std::string temp_content, int previous_size, bool quot, int print_color, int quot_color) {
  if (quot) ColorFunction(quot_color);
  Cursor::GoToXY(0, line_number);


  for (int i = 0; i < temp_content.length(); i++) {
    if (temp_content[i] == '"') {

      /* if it is true, so the current qoutation mark is a closing qoutation mark. */
      if (quot) {
        quot = false;
        std::cout << temp_content[i];
        ColorFunction(print_color);
        continue;
      }

      /* if it is false, so the current qoutation mark is a openning qoutation mark. */
      else {
        quot = true;
        ColorFunction(quot_color);
      }
    }
    std::cout << temp_content[i];
  }
  ColorFunction(print_color);

  /* clear the remain content of line. */
  int space = previous_size - temp_content.size();
  if (space > 0) {
    for (int i = 0; i < space; i++)
      std::cout << ' ';
  }

  /* to remove ~ mark */
  if (temp_content == "") {
    std::cout << ' ';
    return;
  }
  return;
}

/*
Explanation: Updates the screen from line "line_number" to the end of file after user modifies it (paste/delete selected text).
Parameters:
    - line_number: The number of line to start updating the screen.
    - previous_content: Previous content of the file.
    - new_content: New content of the file.
    - quot_vector: Quotation check of the beggining of the lines.
    - print_color: The color for printing text.
    - quot_color: The color for quotation text.
*/
void Console::UpdateFromLineI(int line_number, std::vector<std::string> previous_content, std::vector<std::string> new_content,
  std::vector <bool> quot_vector, int print_color, int quot_color) {
  COORD visible_pos = Cursor::ConsoleTopCharLocator();
  for (size_t i = previous_content.size(); i < new_content.size(); i++) {
    previous_content.push_back("");
  }
  for (int i = line_number; i < new_content.size(); i++) {
    UpdateOneLine(i, new_content[i], previous_content[i].size(), quot_vector[i], print_color, quot_color);
  }
  for (int i = new_content.size(); i < previous_content.size(); i++) {
    Cursor::GoToXY(0, i);
    for (int j = 0; j < previous_content[i].size(); j++) {
      std::cout << ' ';
    }
  }
  for (int i = new_content.size(); i < new_content.size() + 10; i++) {
    Cursor::GoToXY(0, i);
    std::cout << '~';
  }
  Cursor::GoToXY(visible_pos.X, visible_pos.Y);
}

/*
Explanation: Changes the color of the selected text.
Parameters:
    - temp_content: The whole content of file.
    - select_start_pos: Start position of selection by user.
    - select_end_pos: End position of selection by user.
    - print_color: The color for regular text.
    - select_color: The color for selected text.
*/
void Console::HighlightSelectedText(std::vector <std::string> temp_content, COORD select_start_pos, COORD select_end_pos,
  int print_color, int select_color) {

  /* Store the beginning and ending selection positions in temporary variables for potential swapping when necessary. */
  int temp_start_x = select_start_pos.X, temp_end_x = select_end_pos.X;
  int temp_start_y = select_start_pos.Y, temp_end_y = select_end_pos.Y;

  /* if user select a text by going to left, end position is before start position, so we swap them. */
  if ((select_start_pos.Y > select_end_pos.Y) || ((select_start_pos.Y == select_end_pos.Y) &&
    select_start_pos.X > select_end_pos.X)) {
    temp_start_x = select_end_pos.X, temp_end_x = select_start_pos.X;
    temp_start_y = select_end_pos.Y, temp_end_y = select_start_pos.Y;
  }

  /* if selected text is in one line of file */
  if (select_end_pos.Y == select_start_pos.Y) {
    ColorFunction(select_color);
    Cursor::GoToXY(temp_start_x, temp_start_y);
    std::cout << temp_content[temp_end_y].substr(temp_start_x, (temp_end_x - temp_start_x));
    ColorFunction(print_color);
  }

  /* if selected text is in multiple lines */
  else {
    ColorFunction(select_color);
    Cursor::GoToXY(temp_start_x, temp_start_y);
    for (int i = 0; i <= temp_end_y - temp_start_y; i++) {
      if (i == 0) {
        std::cout << temp_content[temp_start_y].substr(temp_start_x);
        continue;
      }
      if (i == temp_end_y - temp_start_y) {
        Cursor::GoToXY(0, temp_start_y + i);
        std::cout << temp_content[temp_end_y].substr(0, temp_end_x);
        continue;
      }
      Cursor::GoToXY(0, temp_start_y + i);
      std::cout << temp_content[temp_start_y + i];
    }
    ColorFunction(print_color);
  }
}

/*
Explanation: Removes highlights when user deselect the text. it updates the screen line by line using UpdateOneLine() function.
Parameters:
    - temp_content: The whole content of file.
    - select_start_pos: Start position of selection by user.
    - select_end_pos: End position of selection by user.
    - quot_vector: Quotation check of the beggining of the lines.
    - print_color: The color for printing text.
    - quot_color: The color for quotation text.
*/
void Console::RemoveSelectHighlight(std::vector<std::string> temp_content, COORD select_start_pos, COORD select_end_pos,
  std::vector<bool> quot_vector, int print_color, int quot_color) {
  if (select_start_pos.X == select_end_pos.X && select_start_pos.Y == select_end_pos.Y && select_start_pos.Y == -1) {
    return;
  }

  /* Store the beginning and ending selection positions in temporary variables for potential swapping when necessary. */
  int temp_start_y = select_start_pos.Y, temp_end_y = select_end_pos.Y;

  /* if user select a text by going to left, end position is before start position, so we swap them. */
  if (select_start_pos.Y > select_end_pos.Y) {
    temp_start_y = select_end_pos.Y, temp_end_y = select_start_pos.Y;
  }
  for (int i = temp_start_y; i < temp_end_y + 1; i++) {
    UpdateOneLine(i, temp_content[i], temp_content[i].size(), quot_vector[i], print_color, quot_color);
  }
}

/*
Explanation: start highlighting the founded search query in file by iterating the search_found_position vector
             (founded match positions of each line is in corresponding index) then iterate on each element and
             print again the search query with the highlighting color.
Parameters:
    - search_found_position: Positions of found search query in each line.
    - search_query: The search query that user inputed. it used for searching in the file and highlight them.
    - print_color: The color for regular text.
    - match_color: The color for matched search text.
*/
void Console::HighlightSearchText(std::vector <std::vector<int>> search_found_position, std::string search_query, int print_color, int match_color) {
  for (int i = 0; i < search_found_position.size(); i++) {
    for (int j = 0; j < search_found_position[i].size(); j++) {
      Cursor::GoToXY(search_found_position[i][j], i);
      ColorFunction(match_color);
      std::cout << search_query;
      ColorFunction(print_color);
    }
  }
}

/*
Explanation:
Parameters:
    - search_found_position: Positions of found search query in each line.
    - search_query: The search query that user inputed. it used for searching in the file and remove highlight.
    - print_color: The color for regular text.
*/
void Console::RemoveSearchHighlight(std::vector<std::vector<int>> search_found_position, std::string search_query, int print_color) {
  ColorFunction(print_color);
  for (int i = 0; i < search_found_position.size(); i++) {
    for (int j = 0; j < search_found_position[i].size(); j++) {
      Cursor::GoToXY(search_found_position[i][j], i);
      std::cout << search_query;
    }
  }
}

/*
Explanation: Asks the user to confirm saving the changes to the file and returns the user's choice. it shows a message and
             user have two choices to save the change or not. in any case after choosing option it close the file and back to menu;
Parameters:
    - file_name: The name of the file to save.
    - print_color: The color for regular text.
    - select_color: The color for selected option. (Yes/No)
Return: True if the user chooses to save, false otherwise.
*/
bool Console::SavePage(std::string file_name, int print_color, int select_color)
{
  /* this is for showing the options and highlighting selected option. (1 is Yes and 2 is No) */
  int temp_option = 1;

  /* in while loop it gets a key input, until user press the enter key and make a selection. */
  while (true) {

    /* clear screen to show message */
    system("cls");
    Cursor::GoToXY(0, 0);
    std::cout << "Do you want to save changes to " << file_name << "?\n";

    /* this is used to highlight the selected option. */
    if (temp_option == 1) {
      Console::ColorFunction(select_color);
      std::cout << "YES\n";
      Console::ColorFunction(print_color);
      std::cout << "NO\n";
    }
    else {
      Console::ColorFunction(print_color);
      std::cout << "YES\n";
      Console::ColorFunction(select_color);
      std::cout << "NO\n";
      Console::ColorFunction(print_color);
    }
    char key;

    /* get input from user and process it. */
    key = _getch();

    /* if the key is arrow key. arrow keys have 2 byte information (first is -32) */
    if ((int)key == -32) {
      key = _getch();

      /*  72 = up arrow key */
      if (key == 72 && temp_option == 2) {
        temp_option = 1;
      }

      /* 80 = down arrow key */
      if (key == 80 && temp_option == 1) {
        temp_option = 2;
      }
      continue;
    }

    /* if user press enter key */
    if (key == '\r') {
      if (temp_option == 1)
        return true;
      return false;
    }
  }
}

/*
Explanation: Displays the first setting page (it has 4 option) and returns the user's choice.
Parameters: None
Return: The user's choice or 27 if user presses Esc.
*/
int Console::SettingPage1() {

  /* clear screen to showing setting options. */
  system("cls");
  Cursor::GoToXY(0, 0);
  std::cout << " Settings \n \n"
    " 1. Change text printing color \n"
    " 2. Change qoutation color \n"
    " 3. Change text selecting color \n"
    " 4. Change search matches color \n \n \n"
    " Select the option by pressing it's number \n \n"
    " Escape : back to menu \n \n ";
  char key = ' ';
  int option = 0;

  /* get input from user until it press an available key! */
  while (true) {
    key = _getch();

    /* user presses Esc. */
    if (key == 27) {
      return 27;
    }
    else if (key > 48 && key < 53) {
      option = key - 48;
      break;
    }
  }
  return option;
}

/*
Explanation: Displays the second setting page (showing available colors for selected option in setting page 1) and returns the user's choice.
             if there are more than 9 colors, they are assigned characters starting with 'a', allowing the user to select them using the designated character.
Parameters:
    - message: The sample message to display.
    - start_color_number: The starting color number to display. (first number of color in available range)
    - end_color_number: The ending color number to display. (last number of color in available range)
    - current_print_color: The current print color for regular text.
Return: The user's choice of color.
*/
int Console::SettingPage2(std::string message, int start_color_number, int end_color_number, int current_print_color) {

  /* clear screen to show colors */
  system("cls");
  Cursor::GoToXY(0, 1);

  /* printing the sample text with available colors and user can choose one of them. */
  for (int i = start_color_number; i < end_color_number + 1; i++) {
    ColorFunction(i);
    int color_showing_number = i - start_color_number + 1;
    if (color_showing_number <= 9) {
      std::cout << message << i - start_color_number + 1 << "  " << std::endl;
    }
    else {
      std::cout << message << (char)(color_showing_number + 87) << "  " << std::endl;
    }
  }
  ColorFunction(current_print_color);
  std::cout << " \n \n Press any other key to back to setting manu.";
  char key = _getch();
  int range = end_color_number - start_color_number;

  /* The user presses a number, and at this point, we perform range checking. */
  if (key >= 49 && key <= 57 && key - 49 < range) {
    return key - 49 + start_color_number;
  }

  /* The user presses a character, and at this point, we perform range checking. */
  if (key >= 97 && key < 123 && key - 88 <= range) {
    return key - 88 + start_color_number;
  }

  /* user presses an invalid character so return 0 and go to setting page 1 (4 main options). */
  return 0;
}