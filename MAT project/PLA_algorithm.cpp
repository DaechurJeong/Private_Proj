#include <iostream>
#include <vector>
#include <utility>

float dot(std::vector<int> v1, std::vector<int> v2)
{
	float result = 0;
	for (unsigned i = 0; i < v1.size(); ++i)
		result += (v1[i] * v2[i]);
	return result;
}
void push_to_list(std::vector<std::pair<std::vector<int>, int>>& list, int num1, int num2, int y)
{
	std::vector<int> vertex;
	vertex.push_back(1);
	vertex.push_back(num1);
	vertex.push_back(num2);
	list.push_back(std::make_pair(vertex, y));
}
std::vector<int> Update_w(std::vector<int>& w, std::vector<int> vertice, int y)
{
	std::vector<int> w_2;
	for (unsigned i = 0; i < w.size(); ++i)
	{
		w_2.push_back(w[i] + y * vertice[i]);
	}
	return w_2;
}
void CalculatePLA(std::vector<std::pair<std::vector<int>, int>> list, std::vector<int>& w)
{
	for (int i = 0; i < list.size(); ++i)
	{
		float new_y = dot(list[i].first, w); // sinf(w, x)
		if (new_y * list[i].second <= 0) // mislabeled
		{
			w = Update_w(w, list[i].first, list[i].second);
			for (int i = 0; i < w.size(); ++i)
			{
				std::cout << w[i] << " ";
			}
			std::cout << std::endl;
			i = -1;
		}
	}
}
int main(void)
{
	std::vector<std::pair<std::vector<int>, int>> list;

	for (int i = 0; i < 20; ++i)
	{
		int x_1 = rand() % 20 - 10;
		int x_2 = rand() % 20 - 10;
		int y = x_1 * 2 + 3 * x_2;

		if (y > 1)
			y = 1;
		else if (y < 1)
			y = -1;
		push_to_list(list, x_1, x_2, y);

		std::cout << "vertice " << i << ": " << x_1 << ", " << x_2 << ", " << y << std::endl;
	}
	
	std::vector<int> w_0(3, 0);

	CalculatePLA(list, w_0);

	for (unsigned i = 0; i < w_0.size(); ++i)
	{
		std::cout << w_0[i] << " ";
	}

	return 0;
}