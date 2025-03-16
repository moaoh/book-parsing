#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json; // JSON 순서 유지

// TSV(탭 구분된 값) 파싱 함수
std::vector<std::string> parseTSVLine(const std::string &line) {
	std::vector<std::string> result;
	std::stringstream ss(line);
	std::string token;

	while (std::getline(ss, token, '\t')) { // 탭('\t')을 기준으로 분리
		// 앞뒤 공백 제거
		size_t start = token.find_first_not_of(" \t\"");
		size_t end = token.find_last_not_of(" \t\"");
		if (start == std::string::npos) {
			result.push_back("");
		}
		else {
			std::string data = token.substr(start, end - start + 1);
			result.push_back(data);
		}
	}
	return result;
}

// ISBN 데이터를 공백(` `) 단위로 분리하는 함수
// 일단은 보류
std::vector<std::string> parseISBN(const std::string &isbnField) {
	std::vector<std::string> isbnList;
	std::stringstream ss(isbnField);
	std::string isbnEntry;

	while (std::getline(ss, isbnEntry, ' ')) { // 공백 단위로 분리
		size_t start = isbnEntry.find_first_not_of(" \t\"");
		size_t end = isbnEntry.find_last_not_of(" \t\"");
		isbnList.push_back((start == std::string::npos) ? "" : isbnEntry.substr(start, end - start + 1));
	}
	return isbnList;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cerr << "./out_json <file_name>" << std::endl;
		return 1;
	}
	std::ifstream file(argv[1]);
	if (!file.is_open()) {
		std::cerr << "파일을 열 수 없습니다!" << std::endl;
		return 1;
	}

	std::string line;
	json bookArray = json::array(); // JSON 배열 생성

	while (std::getline(file, line)) {
		if (line.empty()) continue; // 빈 줄 무시

		std::vector<std::string> fields = parseTSVLine(line);

		if (fields[3].empty()) continue;
		json bookJson = {
			{"author", fields[4]},
			{"year", fields[7]},
			{"title", fields[3]},
			{"subtitle", ""},
			{"location", ""},
			{"publisher", fields[5]},
			{"value", ""},
			{"tag", fields[2]},
			{"workshopNo", fields[1]},
			{"workshopDate", fields[9]},
			{"type", fields[6]},
			{"isbn", fields[8]}
		};

		bookArray.push_back(bookJson); // JSON 배열에 추가
	}

	file.close(); // 파일 닫기

	// JSON 파일 저장
	std::ofstream outFile("books.json");
	if (!outFile.is_open()) {
		std::cerr << "JSON 파일을 저장할 수 없습니다!" << std::endl;
		return 1;
	}

	outFile << bookArray.dump(4); // JSON 보기 좋게 저장 (4칸 들여쓰기)
	outFile.close();

	std::cout << "JSON 변환 완료! books.json 파일에 저장되었습니다." << std::endl;

	return 0;
}