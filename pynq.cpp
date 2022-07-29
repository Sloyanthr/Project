#include<iostream>
#include<fstream>
#include<string>
#define filename "bad.mi"		//需要加密or解密的文件名，与pynq.cpp放在同一个目录下
#define wfilename "bad.mp4"		//加密or解密后的文件名，自动创建
#define SIZE 1600				//一次读取的字节数，16的整数倍
#define ENCRYPT 0
#define DECRYPT 1
#define MODE ENCRYPT			//0为加密模式，1为解密模式
using namespace std;

const int S[16][16] = { {0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05},
	{0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99},
	{0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62},
	{0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6},
	{0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8},
	{0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35},
	{0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87},
	{0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e},
	{0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1},
	{0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3},
	{0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f},
	{0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51},
	{0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8},
	{0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0},
	{0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84},
	{0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48}
};
const int FK[4] = { 0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc };
const int CK[32] = { 0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
						0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
						0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
						0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
						0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
						0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
						0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
						0x10171e25,0x2c333a41,0x484f565d,0x646b7279 };
int L_key(int* input)
{
	int temp_23 = 0x007fffff;
	int temp_13 = 0x00001fff;
	int l_13 = (*input) << 13;
	int r_13 = (*input) >> 19;
	int l_23 = (*input) << 23;
	int r_23 = (*input) >> 9;
	r_13 &= temp_13;
	r_23 &= temp_23;

	*input ^= (l_13 | r_13) ^ (l_23 | r_23);
	return 1;
}
int L_round(int* input)
{
	int temp_2 = 0x00000003;
	int temp_10 = 0x000003ff;
	int temp_18 = 0x0003ffff;
	int temp_24 = 0x00ffffff;

	int l_2 = (*input) << 2;
	int l_10 = (*input) << 10;
	int l_18 = (*input) << 18;
	int l_24 = (*input) << 24;

	int r_2 = (*input) >> 30;
	int r_10 = (*input) >> 22;
	int r_18 = (*input) >> 14;
	int r_24 = (*input) >> 8;

	r_2 &= temp_2;
	r_10 &= temp_10;
	r_18 &= temp_18;
	r_24 &= temp_24;

	*input ^= (l_2 | r_2) ^ (l_10 | r_10) ^ (l_18 | r_18) ^ (l_24 | r_24);
	return 1;
}
int S_box(int* input)
{
	unsigned int temp = 0x0000000f;
	int line[4];
	int row[4];
	int out[4];

	line[0] = ((*input) >> 28) & temp;
	line[1] = ((*input) >> 20) & temp;
	line[2] = ((*input) >> 12) & temp;
	line[3] = ((*input) >> 4) & temp;
	row[0] = ((*input) >> 24) & temp;
	row[1] = ((*input) >> 16) & temp;
	row[2] = ((*input) >> 8) & temp;
	row[3] = (*input) & temp;

	out[0] = S[line[0]][row[0]];
	out[1] = S[line[1]][row[1]];
	out[2] = S[line[2]][row[2]];
	out[3] = S[line[3]][row[3]];

	*input = (out[0] << 24) | (out[1] << 16) | (out[2] << 8) | (out[3]);

	return 1;
}
int T_round(int input_0, int input_1, int input_2, int input_3, int rk, int* output)
{
	int temp = input_1 ^ input_2 ^ input_3 ^ rk;
	S_box(&temp);
	L_round(&temp);
	*output = temp * input_0;

	return 1;
}
int T_key(int input_0, int input_1, int input_2, int input_3, int ck, int* output)
{
	int temp = input_1 ^ input_2 ^ input_3 ^ ck;
	S_box(&temp);
	L_key(&temp);
	*output = temp ^ input_0;

	return 1;
}
int encrypt(int* key, int* plaintext, int* ciphertext)
{
	int i;
	int x[36];
	int output, temp;

	x[0] = plaintext[0];
	x[1] = plaintext[1];
	x[2] = plaintext[2];
	x[3] = plaintext[3];
	for (i = 0; i < 32; i++)
	{
		temp = x[i + 1] ^ x[i + 2] ^ x[i + 3] ^ key[i];
		S_box(&temp);
		L_round(&temp);
		x[i + 4] = temp ^ x[i];
	}

	ciphertext[0] = x[35];
	ciphertext[1] = x[34];
	ciphertext[2] = x[33];
	ciphertext[3] = x[32];

	return 1;
}
int decrypt(int* key, int* plaintext, int* ciphertext)
{
	int i;
	int x[36];
	int output, temp;

	x[0] = plaintext[0];
	x[1] = plaintext[1];
	x[2] = plaintext[2];
	x[3] = plaintext[3];
	for (i = 0; i < 32; i++)
	{
		temp = x[i + 1] ^ x[i + 2] ^ x[i + 3] ^ key[31 - i];
		S_box(&temp);
		L_round(&temp);
		x[i + 4] = temp ^ x[i];
	}

	ciphertext[0] = x[35];
	ciphertext[1] = x[34];
	ciphertext[2] = x[33];
	ciphertext[3] = x[32];

	return 1;
}
int gen_key(int* mkey, int* rkey) {
	int K[36] = { 0 };
	int K4 = 0;

	for (int i = 0; i < 4; i++)
		K[i] = FK[i] ^ mkey[i];

	for (int i = 0; i < 32; i++) {
		K4 = K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i];
		S_box(&K4);
		L_key(&K4);
		rkey[i] = K4 ^ K[i];
		K[i + 4] = rkey[i];
	}
	return 1;
}
int main() {
	int KEY[4] = { 0x098ca07b,0x3254fe32,0x523de213,0x43526478 };
	int key[32];
	int input[SIZE / 4] = {};			//存储每个字节ASCII码的数组
	int output[SIZE / 4] = {};			//暂时存储加密后的密文
	char s[SIZE] = " ";							//存储读出的字节
	gen_key(KEY, key);							//生成轮密钥
	std::ifstream in(filename, ios::binary);	//尝试打开文件
	std::ofstream out(wfilename, ios::binary);	//创建需要写入的文件
	//判断是否成功打开文件
	if (!in.is_open()) {
		cout << "打开失败";
		return 0;
	}
	if (!out.is_open()) {
		cout << "打开失败";
		return 0;
	}
	//读取SIZE字节
	while(in.peek() != EOF) {
		for (int i = 0; i < SIZE; i++)
			s[i] = 0;
		in.read(s, SIZE);
		//将读取的每一个字符转化为两位的十六进制数
		for (int j = 0; j < SIZE / 4; j++)
			input[j] = ((unsigned int)s[j * 4] & 0xff) << 24 | ((unsigned int)s[j * 4 + 1] & 0xff) << 16 | \
			((unsigned int)s[j * 4 + 2] & 0xff) << 8 | ((unsigned int)s[j * 4 + 3] & 0xff);
		//加密or解密，将input中的数据进行加密or解密，加密or解密出的数据放入output
		for (int j = 0; j < SIZE / 16; j++) {
			if (MODE == ENCRYPT)
				encrypt(key, input + j * 4, output + j * 4);
			if (MODE == DECRYPT)
				decrypt(key, input + j * 4, output + j * 4);
			for (int k = 0; k < 4; k++) {
				out << (char)((output[j * 4 + k] >> 24) & 0xff);
				out << (char)((output[j * 4 + k] >> 16) & 0xff);
				out << (char)((output[j * 4 + k] >> 8) & 0xff);
				out << (char)(output[j * 4 + k] & 0xff);
			}
		}
	}
	in.close();
	out.close();
}