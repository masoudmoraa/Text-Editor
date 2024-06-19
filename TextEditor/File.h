#pragma once
#include <Windows.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "Cursor.h"


class File {
public:

	/// <summary>
	/// Default constructor for the File class.
	/// </summary>
	File();

	/// <summary>
	/// Constructor for the File class.
	/// </summary>
	/// <param name="open_type">The type of file operation to perform (create, open, delete, copy).</param>
	/// <param name="print_color">The default color to use for regular text.</param>
	File(std::string, int);

	/// <summary>
	/// Adds a character at the specified position.
	/// </summary>
	/// <param name="value">The character to be added.</param>
	/// <param name="position">The position where the character is added.</param>
	void AddCharater(char value, COORD position);

	/// <summary>
	/// Handles the backspace keypress and returns whether it reduced a line.
	/// </summary>
	/// <param name="position">The current cursor position.</param>
	/// <returns>True if backspace reduced a line, false o.w.</returns>
	bool Backspace(COORD position);

	/// <summary>
	/// Handles the Delete keypress and returns whether it reduced a line.
	/// </summary>
	/// <param name="position">The current cursor position.</param>
	/// <returns>True if delete reduced a line, false o.w.</returns>
	bool DeleteKey(COORD position);


	/// <summary>
	/// Handles pressing Enter and inserts a new line.
	/// </summary>
	/// <param name="position">The current cursor position.</param>
	/// <returns>The new cursor position after going to new line.</returns>
	COORD EnterNewLine(COORD position);

	/// <summary>
	/// Deletes the selected text and returns the new cursor position.
	/// </summary>
	/// <param name="select_start_pos">The starting position of the selection.</param>
	/// <param name="select_end_pos">The ending position of the selection.</param>
	/// <returns>The new cursor position after deleting selected text.</returns>
	COORD DeleteSelected(COORD select_start_pos, COORD select_end_pos);

	/// <summary>
	/// Copies selected text or an entire line at the given position.
	/// </summary>
	/// <param name="position"> The current cursor position.</param>
	/// <param name="select">True if text is selected to copy selected text, false to copy an entire line.</param>
	/// <param name="select_start_pos">The start of the selected text (if select is true).</param>
	/// <param name="select_end_pos">The end of the selected text (if select is true).</param>
	void Copy(COORD position, bool select, COORD select_start_pos, COORD select_end_pos);

	/// <summary>
	/// Cuts the selected text and returns the new cursor position.
	/// </summary>
	/// <param name="select">True if text is selected.</param>
	/// <param name="select_start_pos">The start position of the selected text (if select is true).</param>
	/// <param name="select_end_pos">The end position of the selected text (if select is true).</param>
	/// <returns>The new cursor position.</returns>
	COORD CutSelected(COORD select_start_pos, COORD select_end_pos);

	/// <summary>
	/// Pastes copied text at the specified position and returns the new cursor position.
	/// </summary>
	/// <param name="position">The current cursor position.</param>
	/// <returns>The new cursor position after pasting text.</returns>
	COORD PasteCopiedText(COORD position);

	/// <summary>
	/// Duplicates selected text or an entire line.
	/// </summary>
	/// <param name="position"> The current cursor position.</param>
	/// <param name="select">True if user selects text to duplicate that, false to duplicate an entire line.</param>
	/// <param name="select_start_pos">The start of the selected text (if select is true).</param>
	/// <param name="select_end_pos">The end of the selected text (if select is true).</param>
	COORD Duplicate(COORD position, bool select, COORD select_start_pos, COORD select_end_pos);

	/// <summary>
	/// Deletes the line at the specified position and returns the new cursor position.
	/// </summary>
	/// <param name="position">The current cursor position. (help to delete the correct line.)</param>
	/// <returns>The new cursor position after deleting one line.</returns>
	COORD DeleteLine(COORD position);

	/// <summary>
	/// Updates quot_bool_vector_	content after user change the content of file.
	/// <param name="line_number">Number of line to start update data.</param>
	void UpdateQuotVector(int line_number);

	/// <summary>
	/// Saves temporary changes to the file.
	/// </summary>
	void SaveFile();

	/// <summary>
	/// Returns the name of the openned file.
	/// </summary>
	/// <returns>The file name.</returns>
	std::string GetFileName();

	/// <summary>
	/// Returns a vector containing the lengths of lines in the file.
	/// </summary>
	/// <returns>A vector of line lengths.</returns>
	std::vector <int> GetLineLengthVector();

	/// <summary>
	/// Returns the temporary content of the file.
	/// </summary>
	/// <returns>A vector containing the file's content.</returns>
	std::vector <std::string> GetTempContent();

	/// <summary>
	/// Returns quotation check flags of each line to use in update lines.
	/// </summary>
	/// <returns>A vector containing quotation check flags.</returns>
	std::vector <bool> GetQuotVector();

	/// <summary>
	/// Returns the temporary content of the saved copied text.
	/// </summary>
	/// <returns>A vector containing the copied text.</returns>
	std::vector <std::string> GetCopiedText();

	/// <summary>
	/// check the file object created correctly. false if the given file does not exist.
	/// </summary>
	bool check_file_ = false;

private:

	/// <summary>
	/// Creates a new file with the specified name.
	/// </summary>
	void CreateNewFile();

	/// <summary>
	/// Prompts the user to enter a file name.
	/// </summary>
	void SetFileName();

	/// <summary>
	/// Checks if a file with the same name exists in the same directory.
	/// </summary>
	/// <returns>True if a file with the same name exists, false o.w.</returns>
	bool FileExists();

	/// <summary>
	/// Opens the file for editing and saves its content into the temp_content_ vector.
	/// </summary>
	/// <param name="temp">The name of the file to open. it is unused. class store file name in a private variable named file_name_</param>
	void OpenFile(std::string temp);

	/// <summary>
	/// Prompts the user for the file name and opens it for editing in OpenFile(std::string)
	/// </summary>
	void OpenFile();

	/// <summary>
	/// Reads the content of the file and saves it in temp_content_ vector.
	/// </summary>
	void MakeTempContent();

	/// <summary>
	/// Removes the file from the directory.
	/// </summary>
	void RemoveFile();

	/// <summary>
	/// Prompts the user to enter a directory path for copying the file.
	/// </summary>
	void GetPath();

	/// <summary>
	/// Copies the file to a specified directory.
	/// </summary>
	void CopyInDirectory();

	/// <summary>
	/// Copies the file to the same directory.
	/// </summary>
	void CopyInSameDirectory();

	/// <summary>
	/// Prompts the user to enter a name for the copied file.
	/// </summary>
	void GetCopyFileName();

	/// <summary>
	/// Copies an entire line(saves the content in copied_text_ variable.).
	/// </summary>
	/// <param name="position">The current cursor position to copy the line.</param>
	void CopyLine(COORD position);

	/// <summary>
	/// Copies the selected text(saves the content in copied_text_ variable.).
	/// </summary>
	/// <param name="select_start_pos">The start position of the selected text.</param>
	/// <param name="select_end_pos">The end position of the selected text.</param>
	void CopySelected(COORD select_start_pos, COORD select_end_pos);

	/// <summary>
	/// The color used for regular text printing. (default is white, number 7)
	/// </summary>
	int printing_color_ = 7;

	/// <summary>
	/// The name of the file that user want to do actions (open, create, remove) on it.
	/// </summary>
	std::string file_name_;

	/// <summary>
	/// The path for copying the file (used during copy operations).
	/// </summary>
	std::string copy_path_;

	/// <summary>
	/// The name for the copied file (used during copy operations).
	/// </summary>
	std::string copy_name_;

	/// <summary>
	/// Vector storing the lengths of lines in the file. size of vector equals to line's number of file.
	/// each line's length is stored in the vector at the corresponding index.
	/// </summary>
	std::vector <int> line_length_vector_;

	/// <summary>
	/// Vector storing the temporary content of the file. 
	/// each line's content is stored in the vector at the corresponding index.
	/// </summary>
	std::vector <std::string> temp_content_;

	/// <summary>
	/// Vector storing the quotation check flags for each line. it means if a line start with a character 
	/// inside quotations it will be true. false o.w.
	/// </summary>
	std::vector <bool> quot_bool_vector_;

	/// <summary>
	/// Vector storing the selected text.
	/// </summary>
	std::vector <std::string> selected_text_;

	/// <summary>
	/// Vector storing the copied text.
	/// each line of copied text being stored in an individual element of the vector.
	/// </summary>
	std::vector <std::string> copied_text_;

};