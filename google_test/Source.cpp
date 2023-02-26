/*
	Задание:
		Дана строка типа xxx@[xxx]@[xxx@[x]], где @ - это целочисленное число, а x - любой символ латинского алфавита (a-z).
		Число @ означает - сколько раз нужно повторить строку, находящуюся в [].
	
		Строки всегда валидные, то есть ввод не может быть [xxx], @[], [@] и т.п. 

	Задача - раскодировать исходную строку.

	Пример:
		Ввод  - 2[ab2[c]]
		Вывод - abccabcc
*/


#include <iostream>
#include <cstdint>
#include <stack>
#include <string>


std::string decoder(std::string& source_string)
{
	std::string result_string;
	std::stack<std::pair<std::string, char*>> decoded_parts;
	std::stack<std::pair<int16_t, char*>> encoded_parts;

	auto is_number = [](char symbol) {
		if (symbol >= 47 && symbol <= 57)
			return true;
		return false;
	};

	// Создаем пары. В пару входят количество повторений строки и указатель на ее начало
	// Пример a2[b]
	// pair(2, b])
	int16_t i_number{};
	std::string s_number;
	s_number.reserve(3);
	for (int16_t i = 0; i < source_string.size(); ++i)
	{
		if (is_number(source_string[i]))
		{
			s_number += source_string[i];
		}
		else if (source_string[i] == '[')
		{
			i_number = std::stoi(s_number);
			s_number = "";
			encoded_parts.push(std::make_pair(i_number, &source_string[i + 1]));
		}
	}

	// Проходимся по всем закодированным строкам и постеменно расскадируем их
	while (!encoded_parts.empty())
	{
		auto& current_part = encoded_parts.top();
		std::string decoded_part{};
		std::string temp_string{};

		encoded_parts.pop();

		int16_t i = 0;
		while (*(current_part.second + i) != ']')
		{	
			// Так как мы берем из стека, то раскодирование будет с конца в начало, следовательно, 
			// если мы встретили число, то мы уже должны были его раскодировать и добавить в стек, поэтому просто берем оттуда
			if (is_number(*(current_part.second + i)))
			{
				temp_string += (decoded_parts.top()).first;
				decoded_parts.pop();
				
				// Пропускаем все символы до ]
				while (*(current_part.second + i) != ']')
					++i;
			}
			// Иначе у нас это символы еще не раскодированные, поэтому просто сохраняем
			else
				temp_string += *(current_part.second + i);
			++i;
		}

		// Повторяем раскодированную строку N раз и добавляем ее в стек раскодированных
		decoded_part.reserve(current_part.first * temp_string.size());
		for (i = 0; i < current_part.first; ++i)
		{
			decoded_part += temp_string;
		}
		decoded_parts.push(std::make_pair(decoded_part, current_part.second));
	}

	// Проходимся по всей строке, если символ вне [], то мы его должны просто добавить в результат, если символ в [], то мы берем результат
	// из стека декодированных
	std::stack<char> brackets;
	bool decoded_part_was_applied = false;
	for (int16_t i = 0; i < source_string.size(); ++i)
	{
		if (source_string[i] == '[')
			brackets.push(source_string[i]);
		else if (source_string[i] == ']')
		{
			brackets.pop();
			if (brackets.empty())
				decoded_part_was_applied = false;
		}
		else
		{
			if (!is_number(source_string[i]))
			{
				if (brackets.empty())
				{
					result_string += source_string[i];
				}
				else
				{
					if (!decoded_part_was_applied)
					{
						result_string += (decoded_parts.top()).first;
						decoded_parts.pop();
						decoded_part_was_applied = true;
					}
				}
			}
		}
	}
	return result_string;
}

void print_output(std::string& source, std::string& result, std::string& predict_result)
{
	std::cout << source << " - " << result << " - " << predict_result << std::endl;
	std::cout << (result == predict_result ? "same" : "different") << std::endl;
}

int main()
{
	std::string source_string1 = "3[a]2[bc]";
	std::string source_string2 = "2[a2[b]]";
	std::string source_string3 = "2[a2[b]2[c]]";
	std::string source_string4 = "2[a2[b2[c]]]";
	std::string source_string5 = "ab2[b2[cd]3[e]]f";

	std::string predict_result_string1 = "aaabcbc";
	std::string predict_result_string2 = "abbabb";
	std::string predict_result_string3 = "abbccabbcc";
	std::string predict_result_string4 = "abccbccabccbcc";
	std::string predict_result_string5 = "abbcdcdeeebcdcdeeef";

	std::string result_string1 = decoder(source_string1);
	std::string result_string2 = decoder(source_string2);
	std::string result_string3 = decoder(source_string3);
	std::string result_string4 = decoder(source_string4);
	std::string result_string5 = decoder(source_string5);

	print_output(source_string1, result_string1, predict_result_string1);
	print_output(source_string2, result_string2, predict_result_string2);
	print_output(source_string3, result_string3, predict_result_string3);
	print_output(source_string4, result_string4, predict_result_string4);
	print_output(source_string5, result_string5, predict_result_string5);

	return 0;
}