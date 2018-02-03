//decoder.h
#pragma once
WCHAR word;
inline void persianDecoder(byte b, byte b2) {
	//add support for bytes less than 127 here **********
	if (b == 32) {
		word = L'\u0020';
	}
	if (b > 127) {
		switch (b) {
		case 216:
			switch (b2)
			{
			case 161:
				word = L'\u0621';
				//OutputDebugString(L"\u0621"); //ء
				break;
			case 162:
				word = L'\u0622';
				//OutputDebugString(L"\u0622"); //آ
				break;
			case 167:
				word = L'\u0627';
				//OutputDebugString(L"\u0627"); //ا
				break;
			case 168:
				word = L'\u0628';
				//OutputDebugString(L"\u0628");//ب
				break;
			case 170:
				word = L'\u062A';
				//OutputDebugString(L"\u062A");//ت
				break;
			case 171:
				word = L'\u062B';
				//OutputDebugString(L"\u062B");//ث
				break;
			case 172:
				word = L'\u062C';
				//OutputDebugString(L"\u062C");//ج
				break;
			case 173:
				word = L'\u062D';
				//OutputDebugString(L"\u062D");//ح
				break;
			case 174:
				word = L'\u062E';
				//OutputDebugString(L"\u062E");//خ
				break;
			case 175:
				word = L'\u062F';
				//OutputDebugString(L"\u062F");//د
				break;
			case 176:
				word = L'\u0630';
				//OutputDebugString(L"\u0630");//ذ
				break;
			case 177:
				word = L'\u0631';
				//OutputDebugString(L"\u0631");//ر
				break;
			case 178:
				word = L'\u0632';
				//OutputDebugString(L"\u0632");//ز
				break;
			case 179:
				word = L'\u0633';
				//OutputDebugString(L"\u0633");//س
				break;
			case 180:
				word = L'\u0634';
				//OutputDebugString(L"\u0634");//ش
				break;
			case 181:
				word = L'\u0635';
				//OutputDebugString(L"\u0635");//ص
				break;
			case 182:
				word = L'\u0636';
				//OutputDebugString(L"\u0636");//ض
				break;
			case 183:
				word = L'\u0637';
				//OutputDebugString(L"\u0637");//ط
				break;
			case 184:
				word = L'\u0638';
				//OutputDebugString(L"\u0638");//ظ
				break;
			case 185:
				word = L'\u0639';
				//OutputDebugString(L"\u0639");//ع
				break;
			case 186:
				word = L'\u063A';
				//OutputDebugString(L"\u063A");//غ
				break;
			default:
				break;
			}

			break;
		case 217:
			switch (b2)
			{
			case 190:
				word = L'\u067E';
				//OutputDebugString(L"\u067E");//پ
				break;
			case 129:
				word = L'\u0641';
				//OutputDebugString(L"\u0641");//ف
				break;
			case 130:
				word = L'\u0642';
				//OutputDebugString(L"\u0642");//ق
				break;
			case 131:
				word = L'\u0643';
				//OutputDebugString(L"\u0643");//ل
				break;
			case 132:
				word = L'\u0644';
				//OutputDebugString(L"\u0644");//ل
				break;
			case 133:
				word = L'\u0645';
				//OutputDebugString(L"\u0645");//م
				break;
			case 134:
				word = L'\u0646';
				//OutputDebugString(L"\u0646");//ن
				break;
			case 135:
				word = L'\u0647';
				//OutputDebugString(L"\u0647");//ه
				break;
			case 136:
				word = L'\u0648';
				//OutputDebugString(L"\u0648");//و
				break;
			case 137:
				word = L'\u0649';
				//OutputDebugString(L"\u0649");//ی
				break;
			case 138:
				word = L'\u064A';
				//OutputDebugString(L"\u064A");//ی
				break;
			case 160:
				word = L'\u06F0';
				//OutputDebugString(L"\u06F0");//0
				break;
			case 161:
				word = L'\u06F1';
				//OutputDebugString(L"\u06F1");//1
				break;
			case 162:
				word = L'\u06F2';
				//OutputDebugString(L"\u06F2");//2
				break;
			case 163:
				word = L'\u06F3';
				//OutputDebugString(L"\u06F3");//3
				break;
			case 164:
				word = L'\u06F4';
				//OutputDebugString(L"\u06F4");//4
				break;
			case 165:
				word = L'\u06F5';
				//OutputDebugString(L"\u06F5");//5
				break;
			case 166:
				word = L'\u06F6';
				//OutputDebugString(L"\u06F6");//6
				break;
			case 167:
				word = L'\u06F7';
				//OutputDebugString(L"\u06F7");//7
				break;
			case 168:
				word = L'\u06F8';
				//OutputDebugString(L"\u06F8");//8
				break;
			case 169:
				word = L'\u06F9';
				//OutputDebugString(L"\u06F9");//9
				break;
			default:
				break;
			}

			break;
		case 218:
			switch (b2)
			{
			case 134:
				word = L'\u0686';
				//OutputDebugString(L"\u0686");//چ
				break;
			case 152:
				word = L'\u0698';
				//OutputDebugString(L"\u0698");//ژ
				break;
			case 169:
				word = L'\u06A9';
				//OutputDebugString(L"\u06A9");//ک
				break;
			case 175:
				word = L'\u06AF';
				//OutputDebugString(L"\u06AF");//گ
				break;
			case 190:
				word = L'\u06BE';
				//OutputDebugString(L"\u06BE");//ه
				break;
			default:
				break;
			}
			break;
		case 219:
			switch (b2)
			{
			case 140:
				word = L'\u06CC';
				//OutputDebugString(L"\u06CC");//ی
				break;
			case 176:
				word = L'\u06F0';
				//OutputDebugString(L"\u06F0");//0
				break;
			case 177:
				word = L'\u06F1';
				//OutputDebugString(L"\u06F1");//1
				break;
			case 178:
				word = L'\u06F2';
				//OutputDebugString(L"\u06F2");//2
				break;
			case 179:
				word = L'\u06F3';
				//OutputDebugString(L"\u06F3");//3
				break;
			case 180:
				word = L'\u06F4';
				//OutputDebugString(L"\u06F4");//4
				break;
			case 181:
				word = L'\u06F5';
				//OutputDebugString(L"\u06F5");//5
				break;
			case 182:
				word = L'\u06F6';
				//OutputDebugString(L"\u06F6");//6
				break;
			case 183:
				word = L'\u06F7';
				//OutputDebugString(L"\u06F7");//7
				break;
			case 184:
				word = L'\u06F8';
				//OutputDebugString(L"\u06F8");//8
				break;
			case 185:
				word = L'\u06F9';
				//OutputDebugString(L"\u06F9");//9
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		//OutputDebugString(L"\n");
	}
}