#include "Cursor.h"
#include "Editor.h"
#include "File.h"

/*
Explanation: Default construnctor. it uses in editor.h to create an object of file to access memberfunctions of this class
in editor class. this variable will modifies in Editor::GetInputMenu() by a temporary variable.
*/
File::File() {
  return;
}

/*
Explanation: Default construnctor. it uses in editor.h to create an object of file to access memberfunctions of this class
in editor class. this variable will modifies in Editor::GetInputMenu() by a temporary variable.
Parameters:
    - open_type: It has 4 type for creating instance. and each of them has it's related function to call.
    - print_color: color for regular text in console. default color is white (number = 7).
*/
File::File(std::string open_type = "", int print_color = 7) {
  printing_color_ = print_color;
  if (open_type == "create") {
    CreateNewFile();
  }
  if (open_type == "open") {
    OpenFile();
  }
  if (open_type == "delete") {
    RemoveFile();
  }
  if (open_type == "copy") {
    CopyInDirectory();
  }
  return;
}

/*
Explanation: Adds a character at the specified position. is uses in Editor::OpenFileGetKey() when user presses keys to edit file.
Parameters:
    - value: The character to be added.
    - position: The position where the character should added to file (cursor position).
*/
void File::AddCharater(char value, COORD position) {
  std::string s(1, value);
  temp_content_[position.Y].insert(position.X, s);
  line_length_vector_[position.Y]++;
  return;
}

/*
Explanation: Handles the backspace keypress. is uses in Editor::KeyDownDetected() when user presses backspace to delete a character.
Parameters:
    - position: The position where it should remove character. (cursor position).
Return: True if backspace reduced a line, false o.w. it uses to change cursor position correctly after adding new line.
*/
bool File::Backspace(COORD position) {

  /* Cursor is at beggining of the line and function should return true. */
  if (position.X == 0) {

    /* move content of reduce line to end of the above line */
    temp_content_[position.Y - 1] += temp_content_[position.Y];
    line_length_vector_[position.Y - 1] = temp_content_[position.Y - 1].length();

    /* remove the reduce line index from temp_content_ and line_length_vector_ vectors */
    temp_content_.erase(temp_content_.begin() + position.Y);
    line_length_vector_.erase(line_length_vector_.begin() + position.Y);
    return true;
  }
  /* Cursor is not at beggining of the line so it just delete a character and update vectors. */
  if (position.X > 0) {
    temp_content_[position.Y].erase(temp_content_[position.Y].begin() + position.X - 1);
    line_length_vector_[position.Y]--;
    return false;
  }
  return false;
}

/*
Explanation: Handles the delete keypress. is uses in Editor::KeyDownDetected() when user presses delete to delete a character.
Parameters:
    - position: The position where it should remove character. (cursor position).
Return: True if backspace reduced a line, false o.w.
*/
bool File::DeleteKey(COORD position) {

  /* If it reduces a lone. */
  if (position.X == line_length_vector_[position.Y]) {
    if (position.Y < temp_content_.size() - 1) {
      COORD temp = { 0, position.Y + 1 };
      Backspace(temp);
      return true;
    }
    else
      return false;
  }

  /* If it deletes a character. */
  else {
    temp_content_[position.Y].erase(temp_content_[position.Y].begin() + position.X);
    line_length_vector_[position.Y]--;
    return false;
  }
  return false;
}


/*
Explanation: Handles pressing Enter and inserts a new line. is uses in Editor::OpenFileGetKey() when user presses keys to edit file.
Parameters:
    - position: The position where newline should added. (cursor position).
Return: The new cursor position after going to new line.
*/
COORD File::EnterNewLine(COORD position) {

  /* if the cursor is at the end of line, it just adds an empty line after it. */
  if (position.X == line_length_vector_[position.Y]) {
    temp_content_.insert(temp_content_.begin() + position.Y + 1, "");
    line_length_vector_.insert(line_length_vector_.begin() + position.Y + 1, 0);
  }

  /* if the cursor is at the middle of line, it adds a line after it and updates the contednt. */
  else {
    std::string current_line = temp_content_[position.Y];
    temp_content_.insert(temp_content_.begin() + position.Y + 1, current_line.substr(position.X));
    line_length_vector_.insert(line_length_vector_.begin() + position.Y + 1, temp_content_[position.Y + 1].length());

    temp_content_[position.Y] = current_line.substr(0, position.X);
    line_length_vector_[position.Y] = (int)(temp_content_[position.Y].length());
  }

  /* update positiotn to return it.*/
  position.X = 0;
  position.Y++;
  return position;
}

/*
Explanation: Deletes the selected text. it uses in Editor::KeyDownDetected() when user presses backspace to delete the selected text.
Parameters:
    - select_start_pos: The starting position of the selection.
    - select_end_pos: The ending position of the selection.
Return: The new cursor position after deleting selected text.
*/
COORD File::DeleteSelected(COORD select_start_pos, COORD select_end_pos) {
  COORD new_cursor_position = { 0,0 };

  /* Store the beginning and ending selection positions in temporary variables for potential swapping when necessary. */
  int temp_start_x = select_start_pos.X, temp_end_x = select_end_pos.X;
  int temp_start_y = select_start_pos.Y, temp_end_y = select_end_pos.Y;

  /* if user select a text by going to left, end position is before start position, so we swap them. */
  if ((select_start_pos.Y > select_end_pos.Y) || ((select_start_pos.Y == select_end_pos.Y) && select_start_pos.X > select_end_pos.X)) {
    temp_start_x = select_end_pos.X, temp_end_x = select_start_pos.X;
    temp_start_y = select_end_pos.Y, temp_end_y = select_start_pos.Y;
  }

  /* if selected text is in one line of file. so it updates that line's contents in temp_content_ and line_length_content. */
  if (temp_start_y == temp_end_y) {
    temp_content_[temp_start_y] = temp_content_[temp_start_y].substr(0, temp_start_x) + temp_content_[temp_start_y].substr(temp_end_x);
    line_length_vector_[temp_start_y] = temp_content_[temp_start_y].length();
  }

  /* if selected text is in multiple lines. it updates first and last line's contents and delete midlines contents. */
  else {
    temp_content_[temp_start_y] = temp_content_[temp_start_y].substr(0, temp_start_x);
    temp_content_[temp_start_y] = temp_content_[temp_start_y] + temp_content_[temp_end_y].substr(temp_end_x);
    line_length_vector_[temp_start_y] = temp_content_[temp_start_y].length();

    /* deleting midlines contents. */
    for (int i = temp_start_y + 1; i <= temp_end_y; i++) {
      temp_content_.erase(temp_content_.begin() + temp_start_y + 1);
      line_length_vector_.erase(line_length_vector_.begin() + temp_start_y + 1);
    }
  }
  new_cursor_position.X = temp_start_x;
  new_cursor_position.Y = temp_start_y;
  return new_cursor_position;
}

/*
Explanation: Copies selected text or an entire line at the given position. it use another functions to do that.
Parameters:
    - position: The position from which to copy text (The current cursor position).
    - select: True if text is selected to copy selected text, false to copy an entire line.
    - select_start_pos: The starting position of the selection.
    - select_end_pos: The ending position of the selection.
Return: The new cursor position after deleting selected text.
*/
void File::Copy(COORD position, bool select, COORD select_start_pos, COORD select_end_pos) {

  /* It should copy selected text. */
  if (select) {
    CopySelected(select_start_pos, select_end_pos);
    return;
  }

  /* select is false, so it should copy the entire line. */
  CopyLine(position);
  return;
}

/*
Explanation: Cuts the selected text. at first copy the selected text and save it to copied_text_, then delete the text.
Parameters:
    - select_start_pos: The starting position of the selection.
    - select_end_pos: The ending position of the selection.
Return: The new cursor position after deleting selected text.
*/
COORD File::CutSelected(COORD select_start_pos, COORD select_end_pos) {
  CopySelected(select_start_pos, select_end_pos);
  return DeleteSelected(select_start_pos, select_end_pos);
}

/*
Explanation: Pastes copied text at the given position.
Parameters:
    - position: Position where copied text will be pasted (The current cursor position).
Return: The new cursor position after pasting copied text.
*/
COORD File::PasteCopiedText(COORD position) {
  COORD temp = { -1,-1 };

  /* Checks the content of copied_text_ */
  if (copied_text_.size() == 0) {
    return temp;
  }

  /* Saves the last part of current line to add it to the end of pasted text. */
  std::string last_part_of_first_line = temp_content_[position.Y].substr(position.X);

  /* Adds the first line of copied text to the current line. */
  temp_content_[position.Y] = temp_content_[position.Y].substr(0, position.X) + copied_text_[0];
  line_length_vector_[position.Y] = temp_content_[position.Y].length();

  /* Adds other copied lines to the file and updates temp_content_ and line_length_vector_ indexes. */
  for (int i = 1; i < copied_text_.size(); i++) {
    temp_content_.insert(temp_content_.begin() + position.Y + i, copied_text_[i]);
    line_length_vector_.insert(line_length_vector_.begin() + position.Y + i, temp_content_[position.Y + i].length());
  }

  /* Adds the last line of copied text and updates the variables.
  If copied text is just 1 line. it adds last_part_of_first_line to first line. */
  if (copied_text_.size() == 1) {
    temp_content_[position.Y] = temp_content_[position.Y] + last_part_of_first_line;
    line_length_vector_[position.Y] = temp_content_[position.Y].length();
    temp.Y = position.Y;
    temp.X = line_length_vector_[position.Y] - last_part_of_first_line.length();
  }

  /*  If copied text is more than 1 line. */
  else {
    temp_content_[position.Y + copied_text_.size() - 1] = temp_content_[position.Y + copied_text_.size() - 1] + last_part_of_first_line;
    line_length_vector_[position.Y + copied_text_.size() - 1] = temp_content_[position.Y + copied_text_.size() - 1].length();
    temp.Y = position.Y + copied_text_.size() - 1;
    temp.X = line_length_vector_[position.Y + copied_text_.size() - 1] - last_part_of_first_line.length();
  }
  return temp;
}

/*
Explanation: Duplicates selected text or an entire line. it uses Copy() and PasteCopiedText() functions
             in a proper way to do that (it does not change copy buffer).
Parameters:
    - position: The position from which to copy text (The current cursor position).
    - select: True if user selects text to duplicate that, false to duplicate an entire line.
    - select_start_pos: The starting position of the selection.
    - select_end_pos: The ending position of the selection.
Return: The new cursor position after deleting selected text.
*/
COORD File::Duplicate(COORD position, bool select, COORD select_start_pos, COORD select_end_pos) {
  COORD new_pos = position;

  /* Duplicates selected text. */
  if (select) {
    std::vector<std::string> temp_copy = copied_text_;
    Copy(position, select, select_start_pos, select_end_pos);
    new_pos = PasteCopiedText(position);
    copied_text_ = temp_copy;
    return new_pos;
  }
  /* Duplicates one line. */
  else {
    temp_content_.insert(temp_content_.begin() + position.Y, temp_content_[position.Y]);
    line_length_vector_.insert(line_length_vector_.begin() + position.Y, line_length_vector_[position.Y]);
    ++new_pos.Y;
    return new_pos;
  }
  return COORD();
}

/*
Explanation: Deletes the line at the specified position.
Parameters:
    - position: Position should delete it's line (The current cursor position).
Return: The new cursor position after deleting the line.
*/
COORD File::DeleteLine(COORD position) {
  temp_content_.erase(temp_content_.begin() + position.Y);
  line_length_vector_.erase(line_length_vector_.begin() + position.Y);
  /* If the file is empty after deleting the line. it adds an empty line. */
  if (temp_content_.size() == 0) {
    temp_content_.push_back("");
    line_length_vector_.push_back(0);
  }
  else if (position.Y == line_length_vector_.size()) {
    position.Y = position.Y - 1;
  }

  Cursor::GoToXY(line_length_vector_[position.Y], position.Y);
  return Cursor::GetConsoleCursorPosition();
}

/*
Explanation: Updates quot_bool_vector_	content after user change the content of file.
Parameters:
    - line_number: Number of line to start update data.
*/
void File::UpdateQuotVector(int line_number) {
  int size_q = quot_bool_vector_.size();
  int size_c = temp_content_.size();
  if (size_q < size_c)
    for (int i = 0; i < size_c - size_q; i++)
      quot_bool_vector_.push_back(false);
  if (size_q > size_c)
    for (int i = 0; i < size_q - size_c; i++)
      quot_bool_vector_.pop_back();

  bool quot_check = quot_bool_vector_[line_number];
  for (int i = line_number; i < quot_bool_vector_.size(); i++) {
    quot_bool_vector_[i] = quot_check;
    for (int j = 0; j < temp_content_[i].length(); j++)
      if (temp_content_[i][j] == '"') quot_check = !quot_check;
  }
}

/*
Explanation: Saves temperory changes to the file.
*/
void File::SaveFile() {
  std::ofstream output_file(file_name_);
  if (!output_file.is_open()) {
    std::cerr << "Error : can not open the file to write!" << std::endl;
    return;
  }
  /* rewrites whole file. */
  for (size_t i = 0; i < temp_content_.size(); i++) {
    output_file << temp_content_[i] << "\n";
  }
  output_file.close();
}

/*
Explanation: Returns the name of opened file. it uses in Editor class when needed.
Return: The file name.
*/
std::string File::GetFileName() {
  return file_name_;
}

/*
Explanation: Returns a vector containing the lengths of lines in the file. it uses in Editor class when needed.
Return: A vector of line lingths.
*/
std::vector<int> File::GetLineLengthVector() {
  return line_length_vector_;
}

/*
Explanation: Returns the temporary content of the file. it uses in Editor class when needed.
Return: A vector containing file's content.
*/
std::vector<std::string> File::GetTempContent() {
  return temp_content_;
}

/*
Explanation: Returns quotation check flags of each line to use in update lines.
Return: A vector containing quotation check flags.
*/
std::vector<bool> File::GetQuotVector() {
  return quot_bool_vector_;
}

/*
Explanation: Returns the temporary content of the saved copied text.
Return: A vector containing the copied text.
*/
std::vector<std::string> File::GetCopiedText() {
  return copied_text_;
}

/*
Explanation: Gets the name of file and check it. then creates an empty file and call OpenFile() function.
             Called in File constructor when user choose create file.
*/
void File::CreateNewFile() {

  /* Gets file name. */
  SetFileName();

  /* Checks if a file currently exists with the same name in same directory. */
  if (FileExists()) {
    Console::ShowMessage("A file already exists with the same name!", 116, printing_color_);
    return;
  }

  /* Creates an empty file. */
  std::ofstream output_file(file_name_);
  if (!output_file.is_open()) {
    Console::ShowMessage("Error : can not open the file to write!", 116, printing_color_);
    return;
  }
  output_file.close();

  Console::ShowMessage("File created successfully! continue to start edit the file.", 115, printing_color_);
  /* Opens file for edit. */
  OpenFile(file_name_);
  return;
}

/*
Explanation: Promps the user to enter a file name. this function is used to set file_name_ variable
             when user wants to open, create, remove or copy the file.
*/
void File::SetFileName() {
  std::cout << "Please enter the file name, including the suffix: ";
  std::cin >> file_name_;
  std::cin.get();
}

/*
Explanation: Checks if a file with the same name exists in the same directory.
             it uses when user wants to open, create, remove or copy the file.
*/
bool File::FileExists() {
  std::ifstream existing_file(file_name_);
  if (existing_file) {
    existing_file.close();
    return 1;
  }
  existing_file.close();
  return 0;
}

/*
Explanation: Deletes the line at the specified position.
Parameters:
    - temp: The name of the file to open it and prepare to make changes on it. but the method use file_name_ variable to do that.
            this variable is just a temporary parameter to make a differenc with OpenFile().
*/
void File::OpenFile(std::string temp) {
  check_file_ = true;
  MakeTempContent();
  Cursor::GoToXY(0, temp_content_.size() - 1);
  return;
}

/*
Explanation: Prompts the user for the file name and opens it for editing in OpenFile(std::string).
*/
void File::OpenFile() {
  SetFileName();
  if (FileExists()) {
    OpenFile(file_name_);
    return;
  }
  Console::ShowMessage("file doesn't exist!", 116, printing_color_);
  return;
}

/*
Explanation: Reads the content of the file and saves it in temp_content_ vector.
*/
void File::MakeTempContent() {

  /* clears variables, and modifies them with new file content that user want to open it. */
  temp_content_.clear();
  line_length_vector_.clear();
  quot_bool_vector_.clear();

  /* Double check! (for existing file) */
  if (!FileExists()) {
    std::cerr << "Error : file does not exist!" << std::endl;
    return;
  }
  std::ifstream input_file(file_name_);
  if (!input_file.is_open()) {
    std::cerr << "Error : can not open the file to read!" << std::endl;
    system("pause");
    return;
  }

  /* Modifying temp_content_ and line_length_vector_ line by line. */
  std::string line;
  bool quot_check = false;
  while (std::getline(input_file, line)) {
    temp_content_.push_back(line);
    line_length_vector_.push_back(line.length());
    quot_bool_vector_.push_back(quot_check);
    for (int i = 0; i < line.length(); i++) {
      if (line[i] == '"') quot_check = !quot_check;
    }
  }

  /* Checks that the file has at least 1 line. */
  if (line_length_vector_.size() == 0) {
    temp_content_.push_back("");
    line_length_vector_.push_back(0);
    quot_bool_vector_.push_back(false);
    return;
  }

  /* Adds an empty line to the end of file. */
  if (line_length_vector_[line_length_vector_.size() - 1] != 0) {
    temp_content_.push_back("");
    line_length_vector_.push_back(0);
    quot_bool_vector_.push_back(false);
  }
}

/*
Explanation: Removes the given file from the directory.
*/
void File::RemoveFile() {

  /* Gets file name from user. */
  SetFileName();

  /* Checks file exist. */
  if (!FileExists()) {
    Console::ShowMessage("File does not exist!", 116, printing_color_);
    return;
  }

  /* Removes file. */
  if (remove(file_name_.c_str()) != 0) {
    Console::ShowMessage("File does not exist!", 116, printing_color_);
    return;
  }
  Console::ShowMessage("File removed successfully!", 115, printing_color_);
}

/*
Explanation: Prompts the user to enter a directory path for copying the file.
*/
void File::GetPath() {
  std::cout << "Please enter directory path to copy (leave it empty to select working directory): ";
  std::getline(std::cin, copy_path_);
}

/*
Explanation: Copies the file to a specified directory.
*/
void File::CopyInDirectory() {

  /* Gets file name. */
  SetFileName();
  if (!FileExists()) {
    Console::ShowMessage("File does not exist!", 116, printing_color_);
    return;
  }

  /* Gets a directory path. */
  GetPath();

  /* Gets file name to save the copy. */
  GetCopyFileName();

  /* If user does not input a directory path, file will be copied in same directory. */
  if (copy_path_ == "") {
    CopyInSameDirectory();
    return;
  }

  /* Creates a new file in the given directory. */
  std::ifstream input_file(file_name_);
  std::ofstream output_file(copy_path_ + "\\" + copy_name_);
  std::string line;

  /* Copies the content line by line. */
  while (std::getline(input_file, line)) {
    output_file << line << "\n";
  }
  input_file.close();
  output_file.close();
  return;
}

/*
Explanation: Copies the file to the same directory.
*/
void File::CopyInSameDirectory() {

  /* Creates an empty file. */
  std::ifstream input_file(file_name_);
  std::ofstream output_file(copy_name_);
  std::string line;

  /* Copies the content line by line. */
  while (std::getline(input_file, line)) {
    output_file << line << "\n";
  }
  input_file.close();
  output_file.close();
  return;
}

/*
Explanation: Promps the user to enter a namefor the copied file (this is the name for creating copy file).
*/
void File::GetCopyFileName() {
  std::cout << "Please enter the copy file name, including the suffix: ";
  std::cin >> copy_name_;
  std::cin.get();
}

/*
Explanation: Copies an entire line (saves the content in copied_text_ variable).
*/
void File::CopyLine(COORD position) {
  copied_text_.clear();
  copied_text_.push_back(temp_content_[position.Y]);
  copied_text_.push_back("");
  return;
}

/*
Explanation: Copies an entire line (saves the content in copied_text_ variable).
Parameters:
    - select_start_pos: The start position of the selected text.
    - select_end_pos: The end position of the selected text.
*/
void File::CopySelected(COORD select_start_pos, COORD select_end_pos) {

  /* Clears copied_text_ vector, then modifies it with new content. */
  copied_text_.clear();

  /* Store the beginning and ending selection positions in temporary variables for potential swapping when necessary. */
  int temp_start_x = select_start_pos.X, temp_end_x = select_end_pos.X;
  int temp_start_y = select_start_pos.Y, temp_end_y = select_end_pos.Y;

  /* If user select a text by going to left, end position is before start position, so we swap them. */
  if ((select_start_pos.Y > select_end_pos.Y) || ((select_start_pos.Y == select_end_pos.Y) &&
    select_start_pos.X > select_end_pos.X)) {
    temp_start_x = select_end_pos.X, temp_end_x = select_start_pos.X;
    temp_start_y = select_end_pos.Y, temp_end_y = select_start_pos.Y;
  }

  /* If selected text is in one line of file. */
  if (temp_start_y == temp_end_y) {
    copied_text_.push_back(temp_content_[temp_start_y].substr(temp_start_x, temp_end_x - temp_start_x));
    return;
  }

  /* If selected text is in multiple lines of file. */
  copied_text_.push_back(temp_content_[temp_start_y].substr(temp_start_x));
  for (int i = temp_start_y + 1; i < temp_end_y; i++)
    copied_text_.push_back(temp_content_[i]);
  copied_text_.push_back(temp_content_[temp_end_y].substr(0, temp_end_x));
  return;
}