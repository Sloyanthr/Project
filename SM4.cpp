#include<iostream>
#include<string>
#include<Windows.h>
#include<chrono>

using namespace std;

/*   PART0 加解密需要使用的置换表   */
const char S[16][16][3]{{"d6", "90", "e9", "fe", "cc", "e1", "3d", "b7", "16", "b6", "14", "c2", "28", "fb", "2c", "05"},
						{"2b", "67", "9a", "76", "2a", "be", "04", "c3", "aa", "44", "13", "26", "49", "86", "06", "99"},
						{"9c", "42", "50", "f4", "91", "ef", "98", "7a", "33", "54", "0b", "43", "ed", "cf", "ac", "62"},
						{"e4", "b3", "1c", "a9", "c9", "08", "e8", "95", "80", "df", "94", "fa", "75", "8f", "3f", "a6"},
						{"47", "07", "a7", "fc", "f3", "73", "17", "ba", "83", "59", "3c", "19", "e6", "85", "4f", "a8"},
						{"68", "6b", "81", "b2", "71", "64", "da", "8b", "f8", "eb", "0f", "4b", "70", "56", "9d", "35"},
						{"1e", "24", "0e", "5e", "63", "58", "d1", "a2", "25", "22", "7c", "3b", "01", "21", "78", "87"},
						{"d4", "00", "46", "57", "9f", "d3", "27", "52", "4c", "36", "02", "e7", "a0", "c4", "c8", "9e"},
						{"ea", "bf", "8a", "d2", "40", "c7", "38", "b5", "a3", "f7", "f2", "ce", "f9", "61", "15", "a1"},
						{"e0", "ae", "5d", "a4", "9b", "34", "1a", "55", "ad", "93", "32", "30", "f5", "8c", "b1", "e3"},
						{"1d", "f6", "e2", "2e", "82", "66", "ca", "60", "c0", "29", "23", "ab", "0d", "53", "4e", "6f"},
						{"d5", "db", "37", "45", "de", "fd", "8e", "2f", "03", "ff", "6a", "72", "6d", "6c", "5b", "51"},
						{"8d", "1b", "af", "92", "bb", "dd", "bc", "7f", "11", "d9", "5c", "41", "1f", "10", "5a", "d8"},
						{"0a", "c1", "31", "88", "a5", "cd", "7b", "bd", "2d", "74", "d0", "12", "b8", "e5", "b4", "b0"},
						{"89", "69", "97", "4a", "0c", "96", "77", "7e", "65", "b9", "f1", "09", "c5", "6e", "c6", "84"},
						{"18", "f0", "7d", "ec", "3a", "dc", "4d", "20", "79", "ee", "5f", "3e", "d7", "cb", "39", "48"}};
const char FK[4][9] = {"a3b1bac6", "56aa3350", "677d9197", "b27022dc"};
char Fk[33] = "a3b1bac656aa3350677d9197b27022dc";
char CK[32][9] = {"00070e15","1c232a31","383f464d","545b6269",
						"70777e85","8c939aa1","a8afb6bd","c4cbd2d9",
						"e0e7eef5","fc030a11","181f262d","343b4249",
						"50575e65","6c737a81","888f969d","a4abb2b9",
						"c0c7ced5","dce3eaf1","f8ff060d","141b2229",
						"30373e45","4c535a61","686f767d","848b9299",
						"a0a7aeb5","bcc3cad1","d8dfe6ed","f4fb0209",
						"10171e25","2c333a41","484f565d","646b7279" };

//32bits异或,A、B为32位输入，C为AB按位异或的32位输出(已验)
int XOR_(char* A, char* B, char* C, int length) {
	for (int i = 0; i < length; i++) {
		if (A[i] == B[i])
			C[i] = '0';
		else
			C[i] = '1';
	}
	return 1;
}

/*   PART1 字符类型的进制转换函数   */

//十六进制明文转二进制，plain为16进制，bin为2进制，length为16进制数的长度
int hex2bin(char* plain, char* bin, int length) {
	for (int i = 0; i < length; i++) {
		switch (plain[i])
		{
		case'0':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '0';
			break;
		case'1':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '1';
			break;
		case'2':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '0';
			break;
		case'3':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '1';
			break;
		case'4':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '0';
			break;
		case'5':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '1';
			break;
		case'6':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '0';
			break;
		case'7':
			bin[4 * i] = '0';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '1';
			break;
		case'8':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '0';
			break;
		case'9':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '1';
			break;
		case'a':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '0';
			break;
		case'b':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '0';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '1';
			break;
		case'c':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '0';
			break;
		case'd':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '0';
			bin[4 * i + 3] = '1';
			break;
		case'e':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '0';
			break;
		case'f':
			bin[4 * i] = '1';
			bin[4 * i + 1] = '1';
			bin[4 * i + 2] = '1';
			bin[4 * i + 3] = '1';
			break;
		}
	}
	return 1;
}

//二进制转十六进制明文，bin为2进制，plain为16进制，length为2进制数的长度
int bin2hex(char* bin, char* plain, int length) {
	int roll = length / 4;
	for (int i = 0; i < roll; i++) {
		if (bin[4 * i] == '0') {
			if (bin[4 * i + 1] == '0') {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = '0';
					}
					else {
						plain[i] = '1';
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = '2';
					}
					else {
						plain[i] = '3';
					}
				}
			}
			else {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = '4';
					}
					else {
						plain[i] = '5';
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = '6';
					}
					else {
						plain[i] = '7';
					}
				}
			}
		}
		else {
			if (bin[4 * i + 1] == '0') {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = '8';
					}
					else {
						plain[i] = '9';
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 'a';
					}
					else {
						plain[i] = 'b';
					}
				}
			}
			else {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 'c';
					}
					else {
						plain[i] = 'd';
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 'e';
					}
					else {
						plain[i] = 'f';
					}
				}
			}
		}
	}
	return 1;
}


/*   PART2 密钥生成算法中需要的函数   */
//用于S盒，32比特输入分8组得到输出结果，即将32比特的字符串输出为8个元素的int数组
int bin2hex_int(char* bin, int* plain) {
	for (int i = 0; i < 8; i++) {
		if (bin[4 * i] == '0') {
			if (bin[4 * i + 1] == '0') {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 0;
					}
					else {
						plain[i] = 1;
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 2;
					}
					else {
						plain[i] = 3;
					}
				}
			}
			else {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 4;
					}
					else {
						plain[i] = 5;
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 6;
					}
					else {
						plain[i] = 7;
					}
				}
			}
		}
		else {
			if (bin[4 * i + 1] == '0') {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 8;
					}
					else {
						plain[i] = 9;
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 10;
					}
					else {
						plain[i] = 11;
					}
				}
			}
			else {
				if (bin[4 * i + 2] == '0') {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 12;
					}
					else {
						plain[i] = 13;
					}
				}
				else {
					if (bin[4 * i + 3] == '0') {
						plain[i] = 14;
					}
					else {
						plain[i] = 15;
					}
				}
			}
		}
	}
	return 1;
}

//将字符串str左移length位(已验)
int left(char* str, int length)
{
	int k = strlen(str);
	char* A = new char[length];
	for (int i = 0; i < length; i++)
		A[i] = str[i];
	for (int i = 0; i < k - length; i++)
		str[i] = str[i + length];
	for (int i = 0; i < length; i++)
		str[k + i - length] = A[i];
	return 1;
}

//密钥生成中的线性变换L
int L_key (char* A){
	char A_[33] = " ";
	strcpy_s(A_, A);
	left(A_, 13);		//13
	XOR_(A_, A, A, 32);
	left(A_, 10);		//13+10=23
	XOR_(A_, A, A, 32);
	return 0;
}

//非线性变换Tau，32bits输入,32bits输出(已验)
int Tau(char* mess) {
	int add[8] = { 0 };
	bin2hex_int(mess, add);
	char X[3] = "00";
	for (int i = 0; i < 4; i++) {
		X[0] = S[add[2 * i]][add[2 * i + 1]][0];
		X[1] = S[add[2 * i]][add[2 * i + 1]][1];
		hex2bin(X, (mess + 8 * i), 2);
	}
	return 1;
}

int gen_key_(char* mkey, char* rkey){
	char K[129] = " ";
	char K4[33] = " ";
	char ck[33] = " ";
	hex2bin(Fk, K, 32);
	XOR_(K, mkey, K, 128);
	for (int i = 0; i < 32; i++) {
		hex2bin(CK[i], ck, 8);
		XOR_(K + 32, K + 64, rkey + i * 32, 32);
		XOR_(rkey + i * 32, K + 96, rkey + i * 32, 32);
		XOR_(rkey + i * 32, ck, rkey + i * 32, 32);
		Tau(rkey + i * 32);
		L_key(rkey + i * 32);
		XOR_(rkey + i * 32, K, rkey + i * 32, 32);
		left(K, 32);
		for (int j = 0; j < 32; j++) 
			K[96 + j] = rkey[i * 32 + j];
	}
	return 1;
}

/*   PART3 轮函数F中需要的函数   */
//线性变换L，32bits输入,32bits输出(已验)
int L(char* A){

	char x[9] = " ";
	bin2hex(A, x, 32);

	char A_[129] = " ";
	strcpy_s(A_, A);
	left(A_, 2);		//2
	XOR_(A_, A, A, 32);
	left(A_, 8);		//2+8=10
	XOR_(A_, A, A, 32);
	left(A_, 8);		//2+8+8=18
	XOR_(A_, A, A, 32);
	left(A_, 6);		//2+8+8+6=24
	XOR_(A_, A, A, 32);
	return 0;
}

//轮函数F
void f_XOR(char* rkey,char* X)
{
	char x[33] = " ";
	//char X4[33] = " ";
	XOR_((X + 32), (X + 64), x, 32);
	XOR_(x, (X + 96), x, 32);
	XOR_(x, rkey, x, 32);
	Tau(x);
	L(x);
	XOR_(x, X, x, 32);
	left(X, 32);
	for (int i = 0; i < 32; i++)
		X[96 + i] = x[i];
}

/*   PART4 加解密函数   */

//加密函数
int encrypt(char* key, char* bin, char* outbin) {
	char X[129] = " ";
	char key_[32 * 32 + 1] = " ";
	strcpy_s(X, bin);
	gen_key_(key, key_);
	for (int i = 0; i < 32; i++)
		f_XOR(key_ + i * 32, X);
	//反序变换
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 32; j++)
			outbin[i * 32 + j] = X[(3 - i) * 32 + j];
	return 1;
}

//解密函数(与加密过程相同，轮密钥反向使用)
int decrypt(char* key, char* bin, char* outbin) {
	char X[129] = " ";
	char key_[32 * 32 + 1] = " ";
	char rkey[33] = " ";
	strcpy_s(X, bin);
	gen_key_(key, key_);
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++)
			rkey[j] = key_[(31 - i) * 32 + j];
		f_XOR(rkey, X);
	}
	//反序变换
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 32; j++)
			outbin[i * 32 + j] = X[(3 - i) * 32 + j];
	return 1;
}

char KEY[] = "098ca07b3254fe32523de21343526478";
char plaintext[] = "c7b6a9bcfb8709b780de973d120987ea";

const int model = 0;
int main()
{
	char bin[129] = " ";		//二进制明文
	char key[129] = " ";		//二进制加密密钥
	char key_[129] = " ";	//二进制解密密钥
	char outbin[129] = " ";  //二进制密文
	char outhex[33] = " ";  //十六进制密文
	char debin[129] = " ";	//解密出的二进制明文
	char dehex[33] = " ";	//解密出的十六进制明文
	char plain[129] = " ";

	hex2bin(KEY, key, 32);
	cout << "密钥为:" << KEY << endl;
	//Encrypt
	hex2bin(plaintext, bin, 32);
	cout << "二进制明文为:\n" << bin << endl << "十六进制明文为:\n" << plaintext << endl;
	//std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	//for (int i = 0; i < 10000; i++)
	cout << "开始加密\n";
	encrypt(key, bin, outbin);
	//std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	bin2hex(outbin, outhex, 128);
	cout << "二进制密文为:\n" << outbin << endl << "十六进制密文为:\n" << outhex << endl;
	//std::chrono::duration<double> elapsed = end - start;
	//cout<<"耗时\n"<< elapsed.count()<<"s";
	//Decrypt
	decrypt(key, outbin, debin);
	bin2hex(debin, dehex, 128);
	cout << "解密出的二进制明文为:\n" << debin << endl << "解密出的十六进制明文为:\n" << dehex << endl;
	hex2bin(plaintext, plain, 32);
	cout << "原明文" << plain;
}