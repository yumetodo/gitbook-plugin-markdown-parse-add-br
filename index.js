"use strict";
function parse(s) {
	const CRLF = "\r\n";
	const skip_CRLF = function (s, pos) {
		if(typeof(pos)==="undefined") pos = 0;
		const CRLF = "\r\n";
		return  (-1 == CRLF.indexOf(s[pos])) ? pos
			: (s.length < pos + 1) ? -1
			: (-1 == CRLF.indexOf(s[pos + 1]) || s[pos] == s[pos + 1]) ? pos + 1
			: (s.length < pos + 2) ? -1
			: pos + 2;
	};
	var re = "";
	var pos, pre, crlf_front;
	var pre_empty_line = false;
	var in_code_block = false;
	//一行ずつ見ていく
	for (
		pre = 0;
		-1 != (crlf_front = s.indexOf(CRLF, pre)) && -1 != (pos = skip_CRLF(s, crlf_front));
		pre = pos
	) {
		//in code block
		if (in_code_block) {
			re += s.substr(pre, pos - pre);
			//find code block end
			if (4 <= pos - pre && "`" == s[pre] && "`" == s[pre + 1] && "`" == s[pre + 2]) {
				in_code_block = false;
			}
		}
		//find code block
		else if (pre_empty_line && 4 <= pos - pre && "`" == s[pre] && "`" == s[pre + 1] && "`" == s[pre + 2]) {
			re += s.substr(pre, pos - pre);
			in_code_block = true;
			pre_empty_line = false;
		}
		//find empty line
		else if (pos - pre <= 2 && pre == s.indexOf(CRLF, pre)) {
			//remove paragraph-end <br>
			const re_last_crlf_front = re.lastIndexOf(CRLF);
			if (3 <= re_last_crlf_front && "<br>" == re.substr(re_last_crlf_front - 3, 4)) {
				re.erase(re_last_crlf_front - 3, 4);
			}
			re += s.substr(pre, pos - pre);
			pre_empty_line = true;
		}
		//find newline mark and replace
		else if (3 <= pos - pre && 1 < crlf_front && " " == s[crlf_front - 2] && " " == s[crlf_front - 1]) {
			re += s.substr(pre, crlf_front - 2 - pre);
			re += "<br>";
			re += s.substr(crlf_front, pos - crlf_front);//add CRLF
			pre_empty_line = false;
		}
		else {
			re += s.substr(pre, pos - pre);
			pre_empty_line = false;
		}
	}
	re += s.substr(pre);
	return re;
}
module.exports = {
	hooks: {
		"page:before": function(page) {
			const parse_ = parse;
			var content = page.content;
			content = parse_(content);
			page.content = content;
			return page;
		}
	}
};
