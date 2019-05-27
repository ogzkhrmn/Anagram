#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define TABLE_SIZE 8501
#define H1(key) key%TABLE_SIZE
#define H2(key) 1+(key%(TABLE_SIZE-1))
#define HASH(key, i) (H1(key) + i * H2(key))%TABLE_SIZE

char EOL = '\n';
struct HashMap* hashMap[TABLE_SIZE];
struct HashMap* wordMap[TABLE_SIZE];
char* selectedWord;

int getHorner(char* c)
{
	int i;
	int total = 0;
	for (i = 3; i >= 0; i--)
	{
		total += c[i] * pow(17, i);
	}
	return total;
}

struct HashMap
{
	char value[5];
	int key;
};

void insert(int key, char* value, struct HashMap** givenMap)
{
	int i = 0;
	struct HashMap* hash = (struct HashMap*)malloc(sizeof(struct HashMap));
	strncpy(hash->value, value, 4);
	hash->value[4] = '\0';
	hash->key = key;

	int hashKey = HASH(key, i);

	while (givenMap[hashKey] != NULL)
	{
		hashKey = HASH(key, ++i);
	}
	givenMap[hashKey] = hash;
}

int get(char* key, struct HashMap** givenMap)
{
	int i = 0;
	const int hash_key = getHorner(key);
	int hashed = HASH(hash_key, i);
	while (givenMap[hashed]->value != NULL && strncmp(givenMap[hashed]->value, key, 4) != 0)
	{
		hashed = HASH(hash_key, ++i);
	}
	if (givenMap[hashed]->value != NULL && strncmp(givenMap[hashed]->value, key, 4) == 0)
	{
		return i + 1;
	}
	return -1 * (i + 1);
}


void trim(char* s)
{
	char* p = s;
	int l = strlen(p);

	while (isspace(p[l - 1])) p[--l] = 0;
	while (*p && isspace(*p)) ++p, --l;

	memmove(s, p, l + 1);
}

void createHash()
{
	int i = 0;
	char c[6];
	FILE* fp = fopen("text.txt", "r");
	while (fgets(c, 6, fp) != NULL)
	{
		if (c[0] != EOL)
		{
			i++;
			trim(c);
			insert(getHorner(c), c, hashMap);
		}
	}
}

bool anagram(char word[5])
{
	int i = 0, j;
	bool isAnagram = true;
	char copyWord[4], copySelected[4];
	strncpy(copyWord, word, 4);
	strncpy(copySelected, selectedWord, 4);
	if (strncmp(copyWord, copySelected, 4) == 0)
	{
		return false;
	}
	while (i < 4 && isAnagram)
	{
		j = 0;
		while ((j < 4 && copyWord[j] != copySelected[i]) || copyWord[j] == -1)
			j++;
		if (j == 4)
		{
			isAnagram = false;
		}
		else
		{
			copyWord[j] = -1;
			copySelected[i] = -1;
		}
		i++;
	}
	return isAnagram;
}

int getScore(char* word)
{
	if (get(word, wordMap) > 0)
	{
		return -5;
	}
	if (anagram(word))
	{
		insert(getHorner(word), word, wordMap);
		return 5;
	}
	else
	{
		return -5;
	}
}

int main()
{
	int score = 0;
	int totalScore = 0;
	int exit = '0';
	char word[5];
	createHash();
	srand(time(0));
	int random = rand() % TABLE_SIZE;
	while (hashMap[random] == NULL)
	{
		random = rand() % TABLE_SIZE;
	}
	selectedWord = hashMap[random];
	printf("Given word: %s\n", selectedWord);
	while (exit != 1)
	{
		printf("Anagram: ");
		scanf("%s", word);
		score = getScore(word);
		totalScore += score;
		printf("Bu tur Skoru: %d\n", score);
		printf("Toplam Skor: %d\n", totalScore);
		printf("Cikmak icin 1 e basiniz. Devam etmek icin herhangi bir tusa basmaniz yeterli.\n");
		scanf("%d", &exit);
	}
	return 1;
}
