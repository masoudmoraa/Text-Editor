#include "Editor.h"


/*
Explanation: Initializes a new instance of the Editor class and displays a welcome message. call GetInputMenu() to go to menu page.
*/
Editor::Editor() {
  std::cout << "Welcome !!!\n";

  /* 1 sec delay! */
  Delay(1000);
  GetInputMenu();
}

/*
Explanation: Creates a delay for a specified number of milliseconds. used in one case. after Welcome message.
Parameters:
    - milisec: The duration of the delay in milliseconds.
*/
void Editor::Delay(int milisec) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
}

/*
Explanation: Displays the main menu to the user and handles menu options. managing the selected menu option made by the user.
             prints menu (highlighting selected option) and get input from user (up/down arrow to change selected option,
             enter for choosing, ctrl+q for exit) in an infinite while loop.
*/
void Editor::GetInputMenu() {

  /* runs until user exit the program (main function of Text Editor). */
  while (true) {
    Cursor::ShowConsoleCursor(false);

    /* clears screen */
    system("cls");
    while (true) {

      /* print the menu again. */
      Console::PrintMenu(options, option_number_, printing_color_);

      /* Gets input from user. */
      key_ = _getch();

      /* arrow keys have 2 byte information (first is -32) */
      if ((int)key_ == -32) {

        /* if input is one of the arrowkeys, read the second byte of information from buffer. */
        key_ = _getch();

        /* 72 = up arrow key */
        if (key_ == 72 && (option_number_ >= 1 && option_number_ <= 5))
          --option_number_;

        /* 80 = down arrow key */
        if (key_ == 80 && (option_number_ >= 0 && option_number_ <= 4))
          ++option_number_;
        continue;
      }

      /* input key is ctrl+q, so exit the program. */
      if (key_ == ctrl('q')) {
        Exit();
        break;
      }

      /* user pressed enter, choosed one option. */
      if (key_ == '\r') {
        /* clear screen (menu) */
        system("cls");

        /* number 0 means 1st option : create file */
        if (option_number_ == 0) {
          Cursor::ShowConsoleCursor(true);
          File temp("create", printing_color_);
          file_ = temp;
          OpenFileGetKey();
        }

        /* number 1 means 2nd option : open file */
        else if (option_number_ == 1) {
          Cursor::ShowConsoleCursor(true);
          File temp("open", printing_color_);
          file_ = temp;
          OpenFileGetKey();
        }

        /* number 2 means 3rd option : delete file */
        else if (option_number_ == 2) {
          Cursor::ShowConsoleCursor(true);
          File temp("delete", printing_color_);
        }

        /* number 3 means 4th option : copy file */
        else if (option_number_ == 3) {
          Cursor::ShowConsoleCursor(true);
          File file("copy", printing_color_);
        }

        /* number 4 means 5th option : help */
        else if (option_number_ == 4)
          Help();

        /* number 5 means 6th option : setting */
        else if (option_number_ == 5)
          Setting();
      }
      break;
    }
  }
}

/*
Explanation: Opens a file, shows its content and allowing the user to edit its content.
*/
void Editor::OpenFileGetKey() {
  if (file_.check_file_ == false) return;
  /* this is a flag variable for changing content of file. if user change content (add character, delete character, enter new line,...) this variable will be true. */
  bool change = false;
  COORD cursor_position = Cursor::GetConsoleCursorPosition();

  /* shows file content to user. */
  Console::ShowContent(file_.GetTempContent(), printing_color_, quotation_color_);

  /* Gets input from user, one character at each loop (save the pressed key by user in key_ variable, then process it. and does it again). */
  while (true) {

    /* store current cursor position. used in other functions. */
    cursor_position = Cursor::GetConsoleCursorPosition();

    /* store current file content. used in other functions. */
    std::vector <std::string> content = file_.GetTempContent();

    /* highlight the founded search matches. */
    if (search_query_ != "") {
      Console::HighlightSearchText(search_found_position_, search_query_, printing_color_, highlight_matches_color_);
    }

    /* highlight the selected text, if "select_" is true. */
    if (select_) {
      Console::HighlightSelectedText(content, select_start_pos_, select_end_pos_, printing_color_, text_selecting_color_);
    }
    Cursor::GoToXY(cursor_position.X, cursor_position.Y);

    /* Get input from user. */
    Cursor::ShowConsoleCursor(true);
    key_ = _getch();
    Cursor::ShowConsoleCursor(false);

    /* detects shiftkey press. if user press shift key, it will be true. used in select detection. */
    int shiftkey = ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000);

    /* arrow keys/delet/home/end have 2 byte information (first is -32) */
    if ((int)key_ == -32) {

      /* if input is one of them, read the second byte of information from buffer. */
      key_ = _getch();

      /* 79 = end key */
      if (key_ == 79) {
        Cursor::GoToXY(file_.GetLineLengthVector()[cursor_position.Y], cursor_position.Y);
      }

      /* 71 = home key */
      if (key_ == 71) {
        Cursor::GoToXY(0, cursor_position.Y);
      }

      /* 83 = delete key */
      if (key_ == 83) {
        change = true;
        if (file_.DeleteKey(cursor_position)) {
          file_.UpdateQuotVector(cursor_position.Y);
          Console::UpdateFromLineI(cursor_position.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
        }
        else {
          file_.UpdateQuotVector(cursor_position.Y);
          Console::UpdateOneLine(cursor_position.Y, file_.GetTempContent()[cursor_position.Y], (int)(content[cursor_position.Y].size()), file_.GetQuotVector()[cursor_position.Y], printing_color_, quotation_color_);
        }
        Cursor::GoToXY(cursor_position.X, cursor_position.Y);
      }

      /* 72 = up arrow key */
      if (key_ == 72) {
        Cursor::MoveCursorUp(cursor_position, file_.GetLineLengthVector());
      }

      /* 80 = down arrow key */
      if (key_ == 80) {
        Cursor::MoveCursorDown(cursor_position, file_.GetLineLengthVector());
      }

      /* 75 = left arrow key */
      if (key_ == 75) {
        Cursor::MoveCursorLeft(cursor_position, file_.GetLineLengthVector());
      }

      /* 77 = down arrow key */
      if (key_ == 77) {
        Cursor::MoveCursorRight(cursor_position, file_.GetLineLengthVector());
      }
      COORD new_position = Cursor::GetConsoleCursorPosition();
      UpdateSelectVars(shiftkey, cursor_position, new_position);
      continue;
    }

    /* input key is ctrl+s, saves the temporary changes. */
    if (key_ == ctrl('s')) {
      file_.SaveFile();
      change = false;
      continue;
    }

    /* input key is enter, process enter key (new line). */
    if (key_ == '\r') {
      change = true;
      COORD new_cursor_position = file_.EnterNewLine(cursor_position);
      file_.UpdateQuotVector(new_cursor_position.Y - 1);
      Console::UpdateFromLineI(new_cursor_position.Y - 1, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      Cursor::GoToXY(new_cursor_position.X, new_cursor_position.Y);
      continue;
    }

    /* input key is escape, process escape key. */
    if (key_ == 27) {

      /* if select is true, it will deselect text. */
      if (select_) {
        Console::RemoveSelectHighlight(content, select_start_pos_, select_end_pos_, file_.GetQuotVector(), printing_color_, quotation_color_);
        Cursor::GoToXY(cursor_position.X, cursor_position.Y);
        DefaultSelectVars();
        continue;
      }

      /* if select is false, and user searched a text, it will remove highlight text. */
      if (search_query_ != "") {
        Console::RemoveSearchHighlight(search_found_position_, search_query_, printing_color_);
        Cursor::GoToXY(cursor_position.X, cursor_position.Y);
        search_query_ = "";
        search_found_position_.clear();
        continue;
      }

      /* if select is false, and user did not searched a text, it tries to back to menu. if change is true, it will ask for saving unsaved changes. */
      if (change) {

        /* user choose save option. ("YES") */
        if (Console::SavePage(file_.GetFileName(), printing_color_, text_selecting_color_)) {
          file_.SaveFile();
        }
      }
      break;
    }

    /* input key is ctrl + g, it will copy text. (selected text or one line of file) */
    if (key_ == ctrl('c')) {
      file_.Copy(cursor_position, select_, select_start_pos_, select_end_pos_);
      continue;
    }

    /* input key is ctrl + x, it will cut selected text. */
    if (key_ == ctrl('x')) {
      if (!select_) continue;
      change = true;
      COORD new_cursor_position = file_.CutSelected(select_start_pos_, select_end_pos_);

      /* deselect the text and update screen. */
      DefaultSelectVars();
      file_.UpdateQuotVector(new_cursor_position.Y);
      Console::UpdateFromLineI(new_cursor_position.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      Cursor::GoToXY(new_cursor_position.X, new_cursor_position.Y);
      continue;
    }

    /* input key is ctrl + v, it will paste the copied text. */
    if (key_ == ctrl('v')) {

      /* checks the copied text has a valid data. */
      if (file_.GetCopiedText().size() == 0)
        continue;

      change = true;

      /* if user selected text, at first it should delete the selected text. then paste the copied text. */
      if (select_) {
        cursor_position = file_.DeleteSelected(select_start_pos_, select_end_pos_);
        DefaultSelectVars();
      }
      COORD new_cursor_position = file_.PasteCopiedText(cursor_position);
      file_.UpdateQuotVector(cursor_position.Y);
      Console::UpdateFromLineI(cursor_position.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      Cursor::GoToXY(new_cursor_position.X, new_cursor_position.Y);
      continue;
    }

    /* input key is ctrl + d, it will delete one line of file. */
    if (key_ == ctrl('l')) {

      change = true;

      /* deselect the text.(if selected) */
      DefaultSelectVars();
      COORD new_cursor_position = file_.DeleteLine(cursor_position);
      file_.UpdateQuotVector(new_cursor_position.Y);
      Console::UpdateFromLineI(new_cursor_position.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      Cursor::GoToXY(new_cursor_position.X, new_cursor_position.Y);
      continue;
    }


    if (key_ == ctrl('d')) {
      change = true;

      COORD new_cursor_position = file_.Duplicate(cursor_position, select_, select_start_pos_, select_end_pos_);
      file_.UpdateQuotVector(cursor_position.Y);
      if (select_) {
        if (select_start_pos_.Y <= select_end_pos_.Y)
          Console::UpdateFromLineI(select_start_pos_.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
        else
          Console::UpdateFromLineI(select_end_pos_.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      }
      else
        Console::UpdateFromLineI(cursor_position.Y, content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      DefaultSelectVars();
      Cursor::GoToXY(new_cursor_position.X, new_cursor_position.Y);
      continue;
    }

    /* input key is ctrl + d, it will prumps user to enter a search query. */
    if (key_ == ctrl('f')) {

      COORD visible_pos = Cursor::ConsoleTopCharLocator();

      /* deselect the text (if select is on). */
      DefaultSelectVars();

      Cursor::ShowConsoleCursor(true);
      /* shows another screen to get the search query. */
      SearchTextPage();

      Cursor::ShowConsoleCursor(false);
      /* complete searching and prints the content of file again. */
      Console::ShowContent(content, printing_color_, quotation_color_);
      Cursor::GoToXY(visible_pos.X, visible_pos.Y);
      Cursor::GoToXY(cursor_position.X, cursor_position.Y);
      continue;
    }

    /* if the input key, was not above keys, it is a character or backspace. so it will change the content. (change = true). */
    if (((int)key_ > 31 && (int)key_ < 127) || (int)key_ == 8) {
      change = true;

      /* processes the inputed key. */
      KeyDownDetected(key_, cursor_position);
    }
  }
}

/*
Explanation: Handles key presses when editing a file (characters and back space).
Parameters:
    - value:
    - position:
*/
void Editor::KeyDownDetected(char value, COORD position) {

  /* stores content of file before modify it. */
  std::vector<std::string> previous_content = file_.GetTempContent();

  /* Handles backspace (backspace ascii code = 8). */
  if ((int)value == 8) {

    /* if select is on, it should remove selected part. */
    if (select_) {

      /* stores number of first line of selected text in "update_line" to update the content from there. */
      int update_line = select_start_pos_.Y;
      if (select_end_pos_.Y < update_line) update_line = select_end_pos_.Y;

      /* delete selected part. */
      position = file_.DeleteSelected(select_start_pos_, select_end_pos_);
      DefaultSelectVars();

      /* updates screen. */
      file_.UpdateQuotVector(update_line);
      Console::UpdateFromLineI(update_line, previous_content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      Cursor::GoToXY(position.X, position.Y);
      return;
    }

    /* stores cursor position after deleting the text. */
    int change_line_x = 0;
    int change_line_y = 0;

    /* cursor is at the beggining of the line currently. back space may reduce a line. */
    if (position.X == 0) {

      /* cursor is at the beggining of first line, it should not do anything. */
      if (position.Y == 0)
        return;

      /* save the correct position for cursor after reducing a line. used in else block (line 313). */
      change_line_x = file_.GetLineLengthVector()[position.Y - 1];
      change_line_y = position.Y - 1;

    }

    /* if backspace reduces a line, result is true, false o.w. */
    bool result = file_.Backspace(position);

    /* search again to finding search_query_ because of backspace (delete a character/reduce a line). */
    SearchText(search_query_);
    if (!result) {
      file_.UpdateQuotVector(position.Y);
      Console::UpdateOneLine(position.Y, file_.GetTempContent()[position.Y], (int)(previous_content[position.Y].size()), file_.GetQuotVector()[position.Y], printing_color_, quotation_color_);
      Cursor::GoToXY(position.X - 1, position.Y);
    }
    else {
      file_.UpdateQuotVector(position.Y - 1);
      Console::UpdateFromLineI(position.Y - 1, previous_content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      Cursor::GoToXY(change_line_x, change_line_y);
    }
  }

  /* Handles character. */
  else {

    /* if select is true, at first it removes selected text, then write the value. */
    if (select_) {
      int update_line = select_start_pos_.Y;
      if (select_end_pos_.Y < update_line) update_line = select_end_pos_.Y;
      position = file_.DeleteSelected(select_start_pos_, select_end_pos_);
      DefaultSelectVars();
      file_.AddCharater(value, position);
      file_.UpdateQuotVector(update_line);
      Console::UpdateFromLineI(update_line, previous_content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
    }
    else {
      file_.AddCharater(value, position);
      file_.UpdateQuotVector(position.Y);
      if (value == '"') {
        Console::UpdateFromLineI(position.Y, previous_content, file_.GetTempContent(), file_.GetQuotVector(), printing_color_, quotation_color_);
      }
      else
        Console::UpdateOneLine(position.Y, file_.GetTempContent()[position.Y], (int)(previous_content[position.Y].size()), file_.GetQuotVector()[position.Y], printing_color_, quotation_color_);
    }

    /* search again to finding search_query_ because of added character. */
    SearchText(search_query_);
    Cursor::GoToXY(position.X + 1, position.Y);
  }
  return;
}

/*
Explanation: Displays a search text input page when user click ctrl+f, and save the input in search_query_. search_query_ can not be empty.
*/
bool Editor::SearchTextPage() {
  COORD position = Cursor::GetConsoleCursorPosition();
  search_query_ = "";

  /* clear screen to get input. */
  system("cls");
  Cursor::GoToXY(0, 0);
  std::cout << "Find : ";

  /* Gets input to modifies seach_query_ */
  while (search_query_ == "")
    std::getline(std::cin, search_query_);

  /* true if the search was successful. */
  if (SearchText(search_query_)) {
    Cursor::GoToXY(position.X, position.Y);
    return true;
  }

  /* true if the search was not successful. */
  else {
    search_query_ = "";
    Console::ShowMessage("No matches were found!", 115, printing_color_);
    Cursor::GoToXY(position.X, position.Y);
    return false;
  }
}

/*
Explanation: Searches for a given text in the file's content. and saves the founded position in search_found_position_ vector. this function used in SearchTextPage().
Parameters:
    - search_query: The text to search for.
Return: True if the text is found, false o.w.
*/
bool Editor::SearchText(std::string search_query) {

  /* the function return this variable.  true if the text is found, false o.w. */
  bool is_found = false;

  /* clears the variable to modifies it again. */
  search_found_position_.clear();

  /* the elements of search_found_position_ vector will modify by this variable. */
  std::vector<int> i_th_element_vector;

  int last_found_pos = -1;

  /* search for search_query line by line. */
  for (size_t i = 0; i < file_.GetLineLengthVector().size(); i++) {
    i_th_element_vector.clear();

    /* iterate on characters of line and search for the search_query. it adds -1 to every line. -1 will remove later. */
    /* if the search query founded in the current line, at least one time. */
    do {
      i_th_element_vector.push_back(last_found_pos);

      /* search for the search_query in rest of line (after the last match). */
      last_found_pos = FindSubstringIndex(file_.GetTempContent()[i], search_query, last_found_pos + 1);
    } while (last_found_pos != -1);

    /* remove -1 from vector. */
    i_th_element_vector.erase(i_th_element_vector.begin());

    /* if the size of founded positions vector is more than 0 after removing -1, we found at least on match. is_found will be true. */
    if (i_th_element_vector.size() > 0) {
      is_found = true;
    }

    /* adds te founded positions in line i, to i-th element of search_found_position_ */
    search_found_position_.push_back(i_th_element_vector);
  }
  return is_found;
}

/*
Explanation: Finds the index of a substring within a string (one line of file), starting from a specified position. use in SearchText().
Parameters:
    - str: The string to search in.
    - search: The substring to find.
    - start_pos: The starting position for the search.
*/
int Editor::FindSubstringIndex(std::string str, std::string search, int start_pos) {
  size_t found = str.find(search, start_pos);
  if (found != std::string::npos) return (int)found;
  return -1;
}

/*
Explanation: Updates variables related to text selection. when user select/deselect text. use in OpenFileGetKey().
Parameters:
    - shiftkey: True if the Shift key is pressed, indicating text selection.
    - previous_position: The previous cursor position (before move it to right/left/up/down).
    - new_position: The new cursor position.
*/
void Editor::UpdateSelectVars(bool shiftkey, COORD previous_position, COORD new_position) {
  Console::RemoveSelectHighlight(file_.GetTempContent(), select_start_pos_, select_end_pos_, file_.GetQuotVector(), printing_color_, quotation_color_);
  if (shiftkey) {

    /* if cursor position has changed it will upodates the variables. o.w. does not update the variables. */
    if (previous_position.X != new_position.X || previous_position.Y != new_position.Y) {

      /* if select is on currently. */
      if (select_) {
        select_end_pos_ = new_position;

        /* if start position and end position of selection is equal, so selection will be off. */
        if (select_end_pos_.X == select_start_pos_.X && select_end_pos_.Y == select_start_pos_.Y) {
          Console::RemoveSelectHighlight(file_.GetTempContent(), select_start_pos_, select_end_pos_, file_.GetQuotVector(), printing_color_, quotation_color_);
          DefaultSelectVars();
        }
      }
      else {
        select_ = true;
        select_start_pos_ = previous_position;
        select_end_pos_ = new_position;
      }
    }
  }

  /* shiftkey is not pressed. */
  else {
    DefaultSelectVars();
  }
  Cursor::GoToXY(new_position.X, new_position.Y);
}

/*
Explanation: Resets text selection variables to their default state.
*/
void Editor::DefaultSelectVars() {
  select_ = false;
  COORD invalid = { -1,-1 };
  select_start_pos_ = invalid;
  select_end_pos_ = invalid;
}

/*
Explanation: Displays a help page with information about using the editor. calls when user choose Help option.
*/
void Editor::Help() {
  system("cls");
  Cursor::GoToXY(0, 0);
  std::string help_message;
  help_message = " WELCOME\n\n"
    " About main options in the menu :\n"
    " Create new file : It creates an empty file using the given name.\n"
    " Open file       : It opens a file.\n"
    " Remove file     : It removes a file.\n"
    " Copy file       : It copies a file with a new name on the specified path.\n"
    " Help            : It helps to have a better experience using this app.\n\n\n"
    " Important notes :\n"
    " - ctrl  + s           : Save changes to a file.\n"
    " - ctrl  + f           : Search on the current file (Remove highlights by pressing escape).\n"
    " - shift + arrow keys  : Select text.\n"
    " - ctrl  + c           : Copy the selected text by user (copy a line, if no text is selected).\n"
    " - ctrl  + x           : Cut the selected text by user.\n"
    " - ctrl  + v           : Paste the copied text.\n"
    " - ctrl  + l           : Delete a line.\n"
    " - ctrl  + d           : Duplicate the selected text by user (duplicate a line, if no text is selected).\n"
    " - escape              : Back to menu.\n\n";
  Console::ShowMessage(help_message, printing_color_, printing_color_);
  return;
}

/*
Explanation: Displays a settings page for customizing editor options. it calls Console::SettingPage1() (shows 4 options and waits for user choice)
             then process the user choice and calls Console::SettingPage2() to shows proper message and options.
             user can exit setting by pressing escape.
*/
void Editor::Setting() {
  while (true) {
    int option = Console::SettingPage1();

    /* user pressed escape. */
    if (option == 27) {
      return;
    }
    if (option == 1) {
      std::string example_message = " - To selecting this color as printing text color enter number ";
      int new_color = Console::SettingPage2(example_message, 1, 15, printing_color_);
      if (new_color != 0) {
        printing_color_ = new_color;
        Console::ColorFunction(printing_color_);
      }
    }
    if (option == 2) {
      std::string example_message = " - To selecting this color as qoutation text color enter number ";
      int new_color = Console::SettingPage2(example_message, 1, 15, printing_color_);
      if (new_color != 0) {
        quotation_color_ = new_color;
      }
    }
    if (option == 3) {
      std::string example_message = " - To selecting this color as selecting text color enter number ";
      int new_color = Console::SettingPage2(example_message, 128, 143, printing_color_);
      if (new_color != 0) {
        text_selecting_color_ = new_color;
      }
    }
    if (option == 4) {
      std::string example_message = " - To selecting this color as search matches color enter number ";
      int new_color = Console::SettingPage2(example_message, 112, 127, printing_color_);
      if (new_color != 0) {
        highlight_matches_color_ = new_color;
      }
    }
  }
}

/*
Explanation: Exits the editor program.
*/
void Editor::Exit() {
  exit(1);
}
