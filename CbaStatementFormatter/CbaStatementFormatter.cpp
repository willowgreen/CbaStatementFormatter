/*
 CbaStatementFormatter
 Version 1.0
 --------------------------------
 Copyright 2019, Willowgreen Group Pty Limited ABN 83 629 424 862 LEI 254900RD2A6LV44ZYE61
 --------------------------------
 Licensed under Willowgreen Internal License 2.0
*/

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
	std::vector<std::string>   result;
	std::string                line;
	std::getline(str, line);

	std::stringstream          lineStream(line);
	std::string                cell;

	while (std::getline(lineStream, cell, ','))
	{
		result.push_back(cell);
	}
	if (!lineStream && cell.empty()) // checks for trailing commas with no data afterwards
	{
		result.push_back(""); // if trailing comma, add empty element
	}
	return result;
}

class CSVRow
{
public:
	std::string const& operator[](std::size_t index) const
	{
		return m_data[index];
	}
	std::size_t size() const
	{
		return m_data.size();
	}
	void readNextRow(std::istream& str)
	{
		std::string line;
		std::getline(str, line);

		std::stringstream lineStream(line);
		std::string cell;

		m_data.clear();
		while (std::getline(lineStream, cell, ','))
		{
			m_data.push_back(cell);
		}
		if (!lineStream && cell.empty()) // checks for trailing commas with no data afterwards
		{
			m_data.push_back(""); // if trailing comma, add empty element
		}
	}
private:
	std::vector<std::string> m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
	data.readNextRow(str);
	return str;
}
int main(int argc, char* argv[]) {

	std::string filename(argv[1]);
	std::cout << "Parsing " + filename + "..." << std::endl;

	std::ifstream inFile(filename);

	std::string outFilename = filename.substr(0, filename.length() - 4) + "_FormattedForDynamics.csv";
	std::ofstream outFile(outFilename);

	if (outFile.is_open())
	{
		CSVRow row;
		int rowc = 1;
		while (inFile >> row)
		{
			if (rowc != 1) {
				long debit;
				try {
					debit = std::stol(row[3]);
				}
				catch (std::invalid_argument ia) {
					debit = 0;
				}

				long credit;
				try {
					credit = std::stol(row[4]);
				}
				catch (std::invalid_argument ia) {
					credit = 0;
				}

				long amount = 0;
				if (debit == 0)
					amount = credit;
				else if (credit == 0)
					amount = 0 - debit;

				std::string str = row[0] + "," + row[1] + "," + std::to_string(amount);
				outFile << str << std::endl;
			}
			rowc++;
		}
		outFile.close();

		system("pause");
	}
	else std::cerr << "Unable to open " + outFilename;

}
