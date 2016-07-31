#include <iostream>
#include <map>
#include <string>
#include <locale> 
#include <codecvt> 
using namespace std::string_literals;
static const auto CRLF = L"\r\n"s;
std::size_t skip_CRLF(const std::wstring& s, std::size_t pos = 0) {
	return 
		(std::wstring::npos == CRLF.find_first_of(s[pos])) ? pos
		: (s.length() < pos + 1) ? std::wstring::npos
		: (std::wstring::npos == CRLF.find_first_of(s[pos + 1]) || s[pos] == s[pos + 1])? pos + 1
		: (s.length() < pos + 2) ? std::wstring::npos
		: pos + 2;
}
std::wstring parse(const std::wstring& s) {

	std::wstring re;
	std::size_t pos, pre, crlf_front;
	bool pre_empty_line = false;
	bool in_code_block = false;
	//一行ずつ見ていく
	for (
		pre = 0;
		std::wstring::npos != (crlf_front = s.find_first_of(CRLF, pre)) && std::wstring::npos != (pos = skip_CRLF(s, crlf_front));
		pre = pos
		) {
		//in code block
		if (in_code_block) {
			re.append(s, pre, pos - pre);
			//find code block end
			if (4 <= pos - pre && L'`' == s[pre] && L'`' == s[pre + 1] && L'`' == s[pre + 2]) {
				in_code_block = false;
			}
		}
		//find code block : requirement of line length must be longer than 3 character except CRLF.
		else if (pre_empty_line && 4 <= pos - pre && L'`' == s[pre] && L'`' == s[pre + 1] && L'`' == s[pre + 2]) {
			re.append(s, pre, pos - pre);
			in_code_block = true;
			pre_empty_line = false;
		}
		//find empty line
		else if (pos - pre <= 2 && pre == s.find_first_of(CRLF, pre)) {
			//remove paragraph-end <br>
			const std::size_t re_last_crlf_front = re.find_last_not_of(CRLF);
			if (
				std::string::npos != re_last_crlf_front && 3 <= re_last_crlf_front
				&& '<' == re[re_last_crlf_front - 3]
				&& 'b' == re[re_last_crlf_front - 2]
				&& 'r' == re[re_last_crlf_front - 1]
				&& '>' == re[re_last_crlf_front]
				) {
				re.erase(re_last_crlf_front - 3, 4);
			}
			re.append(s, pre, pos - pre);
			pre_empty_line = true;
		}
		//find newline mark and replace : requirement of line length must be longer than 2 character except CRLF
		else if (3 <= pos - pre && 1 < crlf_front && L' ' == s[crlf_front - 2] && L' ' == s[crlf_front - 1]) {
			re.append(s, pre, crlf_front - 2 - pre);
			re += L"<br>";
			re.append(s, crlf_front, pos - crlf_front);//add CRLF
			pre_empty_line = false;
		}
		else {
			re.append(s, pre, pos - pre);
			pre_empty_line = false;
		}
	}
	re.append(s, pre, std::wstring::npos);
	return re;
}
int main()
{
	std::wstring s = LR"(どこからどう見ても小学生の自分、『江戸川コナン』。  
それがまさか、妙な組織に妙な薬を飲まされた所為で身体が小さくなっただなんて。  
本当は——『高校生探偵』 工藤新一なんだってことを知るものは、ほんのわずかなのだから。

```
「『名探偵』 殿へ  

この度、我が古橋家の別荘において 『宝捜し』 を行うこととなりました。つきましては名探偵として名高い貴方様にもぜひご参加いただきたく、お手紙を差し上げました。  
隠されているのは古橋家先祖の遺産、その価値は数十億にのぼるとも言われておりますが、見事発見されたなら、その全てを貴方に差し上げます。  
もし参加していただけるなら、下記の日時に同封のプレートを持って別荘にお越しくださるよう——」
```

「——プレート?」  
蘭は眉を寄せた。そんなもの一緒に入っていただろうか…？)";
	auto re = parse(s);
    std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> cv;
	std::cout << cv.to_bytes(re) << std::endl;
}
