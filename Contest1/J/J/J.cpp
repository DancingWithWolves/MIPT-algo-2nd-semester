
#include <iostream>
#include <vector>
#include <cstdlib>

typedef unsigned long long ull;
//const ull inf = std::numeric_limits<unsigned long long>::infinity();
const ull inf = 10000000;
const ull neg_inf = std::numeric_limits<unsigned long long>::min();


/*vector<Node*> vec;
vec = new vector<Node*>;
vec[0] = nullptr;*/

struct Node {
	std::vector<Node*> next;
	int height = 0;
	ull key = 0;
	Node(ull _key, int _height)
	{
		key = _key;
		height = _height;
	}
};
struct SkipList {
	int max_height = 16;
	Node *tail = nullptr, *head = nullptr;

	SkipList(int _max_height = 16)
	{
		max_height = _max_height;
		tail = new Node(inf, max_height);
		head = new Node(neg_inf, max_height);
		for (int i = 0; i < max_height; ++i) {
			head->next.push_back(tail);
		}
		//std::cout << (head->next)[0]->key << std::endl;
	}

	int CoinFlip()
	{
		int ret = 1;
		for (int i = 0; i < max_height - 1; ++i) {
			if (std::rand() % 2) {
				ret++;
			}
			else return ret;
		}
	}

	void Print()
	{
		std::cout << "----------------------------------\n";
		Node* cur = this->head;
		for (int i = max_height - 1; i >= 0; --i) {
			while (cur != this->tail) {
				std::cout << cur->key << " ";
				cur = (cur->next)[i];
			}
			cur = this->head;
			std::cout << "\n";
		}
		std::cout << "----------------------------------\n";
	}
	Node* Find(ull _key, Node** passed)
	{
		Node* cur = this->head;
		int i = max_height - 1;
		for (i; i >= 0; --i) {
			while ((cur->next)[i]->key < _key) {
				cur = (cur->next)[i];
			}
			if (passed) passed[i] = cur;
		}
		
		return cur;
	}
	void Delete(ull _key)
	{
		Node** passed = new Node*[16];
		Node* wanted = Find(_key, passed)->next[0];
		if (wanted->key != _key) {
			std::cout << "Didn't find!\n";
			return;
		}
		for (int i = 0; i < wanted->height; ++i) {
			passed[i]->next[i] = wanted->next[i];
		}
	}
	void Insert(ull _key)
	{
		int newHeight = CoinFlip();
		Node* _new = new Node(_key, newHeight);
		Node* cur = this->head;
		Node **passed = new Node*[16];

		Find(_key, passed); //заполняем passed
		

		/*if (newHeight > (cur->next).size()) {
			for (int i = 0; i < newHeight - cur->next.size(); ++i) {
				passed[i]->next[i] = _new;
			}
		}*/

		for (int i = 0; i < newHeight; ++i) {
			(_new->next).push_back( passed[i]->next[i] );
			(passed[i]->next)[i] = _new;
		}
	}
};


int main()
{
	std::cin.tie(nullptr);
	std::ios_base::sync_with_stdio(false);
	SkipList *sl = new SkipList();
	sl->Insert(1);
	sl->Insert(2);
	sl->Insert(4);
	sl->Insert(4);
	sl->Insert(4);
	sl->Insert(3);
	sl->Insert(13);
	sl->Insert(8);
	sl->Insert(9);
	sl->Insert(10);
	sl->Insert(11);
	sl->Insert(12);
	sl->Insert(13);
	sl->Insert(14);
	
	sl->Print();
	sl->Delete(4);
	sl->Delete(4);
	sl->Delete(4);
	sl->Delete(1);
	sl->Delete(4);
	sl->Print();
	return 0;
}
